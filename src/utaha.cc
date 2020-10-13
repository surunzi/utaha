#include <iostream>
#include "utaha.h"
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

namespace utaha
{

  using v8::Isolate;

  Utaha::Utaha()
  {
    Isolate::CreateParams params;
    params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = Isolate::New(params);
  }

  Utaha::~Utaha()
  {
    isolate->Dispose();
  }

  int Utaha::Run()
  {
    Isolate::Scope isolate_scope(isolate);

    RunShell();

    return 0;
  }

  v8::Local<v8::String> Utaha::ReadFromStdin()
  {
    static const int kBufferSize = 256;
    char buffer[kBufferSize];
    v8::Local<v8::String> accumulator = v8::String::NewFromUtf8Literal(isolate, "");
    int length;
    while (true)
    {
      char *input = nullptr;
      input = fgets(buffer, kBufferSize, stdin);
      if (input == nullptr)
        return v8::Local<v8::String>();
      length = static_cast<int>(strlen(buffer));
      if (length == 0)
      {
        return accumulator;
      }
      else if (buffer[length - 1] != '\n')
      {
        accumulator = v8::String::Concat(
            isolate, accumulator,
            v8::String::NewFromUtf8(isolate, buffer, v8::NewStringType::kNormal, length)
                .ToLocalChecked());
      }
      else if (length > 1 && buffer[length - 2] == '\\')
      {
        buffer[length - 2] = '\n';
        accumulator =
            v8::String::Concat(isolate, accumulator,
                               v8::String::NewFromUtf8(isolate, buffer,
                                                       v8::NewStringType::kNormal, length - 1)
                                   .ToLocalChecked());
      }
      else
      {
        return v8::String::Concat(
            isolate, accumulator,
            v8::String::NewFromUtf8(isolate, buffer, v8::NewStringType::kNormal,
                                    length - 1)
                .ToLocalChecked());
      }
    }
  }

  void Utaha::RunShell()
  {
    v8::HandleScope outer_scope(isolate);

    v8::Local<v8::ObjectTemplate> global_template = CreateGlobalTemplate();

    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global_template);

    v8::Context::Scope context_scope(context);

    while (true)
    {
      v8::HandleScope inner_scope(isolate);
      printf("> ");
      v8::Local<v8::String> input = ReadFromStdin();
      if (input.IsEmpty())
      {
        break;
      }
      v8::Local<v8::Value> result = ExecuteString(input);

      if (!result->IsUndefined())
      {
        v8::String::Utf8Value str(isolate, result);
        fwrite(*str, sizeof(**str), str.length(), stdout);
        printf("\n");
      }
    }
  }

  v8::Local<v8::Value> Utaha::ExecuteString(v8::Local<v8::String> source)
  {
    v8::Local<v8::Context> context(isolate->GetCurrentContext());

    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    return script->Run(context).ToLocalChecked();
  }

  v8::Local<v8::ObjectTemplate> Utaha::CreateGlobalTemplate()
  {
    v8::Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New(isolate);

    global_template->Set(isolate, "version", v8::FunctionTemplate::New(isolate, Version));

    return global_template;
  }

  void Utaha::Version(const v8::FunctionCallbackInfo<v8::Value> &args)
  {
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "0.0.1").ToLocalChecked());
  }

  int Start(int argc, char *argv[])
  {
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    int result;
    {
      Utaha utaha;
      result = utaha.Run();
    }

    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return result;
  }
} // namespace utaha