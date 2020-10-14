#ifndef _UTAHA_H_
#define _UTAHA_H_

#include "v8.h"

using v8::Local;
using v8::String;
using v8::Value;

namespace utaha
{

  class Utaha
  {
  public:
    Utaha();
    ~Utaha();
    int Run();

  private:
    v8::Isolate *isolate;
    void RunShell();
    Local<String> ReadFromStdin();
    Local<v8::ObjectTemplate> CreateGlobalTemplate();
    Local<Value> ExecuteString(Local<String> source);
    static void Version(const v8::FunctionCallbackInfo<Value> &args);
  };

  int Start(int argc, char *argv[]);

} // namespace utaha

#endif