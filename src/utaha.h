#ifndef _UTAHA_H_
#define _UTAHA_H_

#include "v8.h"

namespace utaha {

class Utaha {
  public:
    Utaha();
    int Run();
  private:
    v8::Isolate* isolate;  
};

int Start(int argc, char* argv[]);

}

#endif