#include "common.h"

// a map between the foreign method signature and the callback function
std::map<std::string, WrenForeignMethodFn> boundForeignMethods{};

// this function has the signature corresponding to bindForeignMethodFn
WrenForeignMethodFn bindForeignMethod(WrenVM* vm,
                                      const char* module,
                                      const char* className,
                                      bool isStatic,
                                      const char* signature
                                      ) {
  std::string fullSignature{ module };
  fullSignature += className;
  fullSignature += signature;
  if (isStatic) {
      fullSignature += "s";
  }
  auto it = boundForeignMethods.find(fullSignature);
  if (it != boundForeignMethods.end()) {
      return it->second;
  }
  return nullptr;
}

char* loadModule(WrenVM* vm, const char* name) {
  std::string path( name );
  path += ".wren";
  std::ifstream fin;
  fin.open( path, std::ios::in );
  std::stringstream buffer;
  buffer << fin.rdbuf() << '\0';
  std::string source = buffer.str();
  
  char* cbuffer = (char*) malloc( source.size() );
  memcpy( cbuffer, source.c_str(), source.size() );
  return cbuffer;
}

static void write(WrenVM* vm, const char* text)
{
  printf("%s", text);
}

