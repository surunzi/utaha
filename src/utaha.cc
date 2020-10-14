#include <iostream>
#include "utaha.h"
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

using v8::Context;
using v8::Isolate;
using v8::Local;
using v8::String;
using v8::V8;
using v8::Value;

namespace utaha
{
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

  Local<String> Utaha::ReadFromStdin()
  {
    static const int kBufferSize = 256;
    char buffer[kBufferSize];
    Local<String> accumulator = String::NewFromUtf8Literal(isolate, "");
    int length;
    while (true)
    {
      char *input = nullptr;
      input = fgets(buffer, kBufferSize, stdin);
      if (input == nullptr)
        return Local<String>();
      length = static_cast<int>(strlen(buffer));
      if (length == 0)
      {
        return accumulator;
      }
      else if (buffer[length - 1] != '\n')
      {
        accumulator = String::Concat(
            isolate, accumulator,
            String::NewFromUtf8(isolate, buffer, v8::NewStringType::kNormal, length)
                .ToLocalChecked());
      }
      else if (length > 1 && buffer[length - 2] == '\\')
      {
        buffer[length - 2] = '\n';
        accumulator =
            String::Concat(isolate, accumulator,
                           String::NewFromUtf8(isolate, buffer,
                                               v8::NewStringType::kNormal, length - 1)
                               .ToLocalChecked());
      }
      else
      {
        return String::Concat(
            isolate, accumulator,
            String::NewFromUtf8(isolate, buffer, v8::NewStringType::kNormal,
                                length - 1)
                .ToLocalChecked());
      }
    }
  }

  void Utaha::RunShell()
  {
    v8::HandleScope outer_scope(isolate);

    Local<v8::ObjectTemplate> global_template = CreateGlobalTemplate();

    Local<Context> context = Context::New(isolate, nullptr, global_template);

    Context::Scope context_scope(context);

    while (true)
    {
      v8::HandleScope inner_scope(isolate);
      printf("> ");
      Local<String> input = ReadFromStdin();
      if (input.IsEmpty())
      {
        break;
      }
      Local<Value> result = ExecuteString(input);

      if (!result->IsUndefined())
      {
        String::Utf8Value str(isolate, result);
        fwrite(*str, sizeof(**str), str.length(), stdout);
        printf("\n");
      }
    }
  }

  Local<Value> Utaha::ExecuteString(Local<String> source)
  {
    Local<Context> context(isolate->GetCurrentContext());

    Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    return script->Run(context).ToLocalChecked();
  }

  Local<v8::ObjectTemplate> Utaha::CreateGlobalTemplate()
  {
    Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New(isolate);

    global_template->Set(isolate, "version", v8::FunctionTemplate::New(isolate, Version));

    return global_template;
  }

  void Utaha::Version(const v8::FunctionCallbackInfo<Value> &args)
  {
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), "0.0.1").ToLocalChecked());
  }

  int Start(int argc, char *argv[])
  {
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    V8::InitializePlatform(platform.get());
    V8::Initialize();

    int result;
    {
      Utaha utaha;
      result = utaha.Run();
    }

    V8::Dispose();
    V8::ShutdownPlatform();

    return result;
  }
} // namespace utaha