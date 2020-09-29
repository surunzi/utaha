#include "utaha.h"
#include "v8.h"

namespace utaha {

using v8::Isolate;

Utaha::Utaha() {
  
}

int Utaha::Run() {
  return 0;
}

int Start(int argc, char* argv[]) {
  Utaha utaha;

  return utaha.Run();
}

}