#ifndef _UTAHA_H_
#define _UTAHA_H_

#include "v8.h"

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
    v8::Local<v8::String> ReadFromStdin();
    v8::Local<v8::Value> ExecuteString(v8::Local<v8::String> source);
  };

  int Start(int argc, char *argv[]);

} // namespace utaha

#endif