#include <iostream>
#include "utaha.h"
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

namespace utaha {

using v8::Isolate;

Utaha::Utaha() {
  Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  isolate = Isolate::New(params);
}

int Utaha::Run() {
  Isolate::Scope isolate_scope(isolate);

  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::Context> context = v8::Context::New(isolate);

  v8::Context::Scope context_scope(context);

  v8::Local<v8::String> source =
    v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                            v8::NewStringType::kNormal)
      .ToLocalChecked();

  v8::Local<v8::Script> script =
    v8::Script::Compile(context, source).ToLocalChecked();

  v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

  v8::String::Utf8Value utf8(isolate, result);

  std::cout << *utf8 << std::endl;

  return 0;
}

int Start(int argc, char* argv[]) {
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  Utaha utaha;

  return utaha.Run();
}

}