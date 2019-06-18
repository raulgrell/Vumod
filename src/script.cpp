#include "common.h"

#include "script/lib.h"

// note that according to the "vector" module name, our Vec3 definition
// now lies in the file vector.wren
std::map<std::string, WrenForeignMethodFn> boundForeignMethods {
        {"vectorVec3norm()",   vec3fNorm},
        {"vectorVec3dot(_)",   vec3fDot},
        {"vectorVec3cross(_)", vec3fCross},
        {"vectorVec3x",        vec3fGetX},
        {"vectorVec3x=(_)",    vec3fSetX},
        {"vectorVec3y",        vec3fGetY},
        {"vectorVec3y=(_)",    vec3fSetY},
        {"vectorVec3z",        vec3fGetZ},
        {"vectorVec3z=(_)",    vec3fSetZ}
};

// WrenForeignClassMethods is just a struct containing a
// WrenForeignMethodFn, and a WrenFinalizerFn
std::map<std::string, WrenForeignClassMethods> boundForeignClasses {
        {"vectorVec3", {vec3fAllocate, vec3fFinalize}}
};

WrenForeignMethodFn bindForeignMethod(WrenVM *, const char *module, const char *className, bool isStatic, const char *signature) {
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

WrenForeignClassMethods bindForeignClass(WrenVM *, const char *module, const char *className) {
    std::string identifier{module};
    identifier += className;
    auto it = boundForeignClasses.find(identifier);
    if (it != boundForeignClasses.end()) {
        return it->second;
    }
    return WrenForeignClassMethods{nullptr, nullptr};
}

template<typename T>
void allocateType(WrenVM* vm) {
    void* bytes = wrenSetSlotNewForeign(vm, 0, 0, sizeof(T));
    new (bytes) T();
}

template<typename T>
void finalizeType(void* bytes) {
    T* type = (T*)bytes;
    type->~T();
}

static void write(WrenVM *vm, const char *text) {
    printf("%s", text);
}

VuScript::VuScript() {
    wrenInitConfiguration(&config);
    config.bindForeignMethodFn = bindForeignMethod;
    config.writeFn= write;
    config.loadModuleFn = loadModule;
    config.writeFn = write;

    WrenVM *vm = wrenNewVM(&config);

    WrenInterpretResult result = wrenInterpret(vm, "vumod", command_text);

    if (result != WREN_RESULT_SUCCESS)
        fatal_error("Could not run script.");
}


VuScript::~VuScript() {
    wrenFreeVM(vm);
};

void VuScript::InterpretCommands() {
    for (auto &&command : commands)
        wrenInterpret(vm, "vumod", command.c_str());
    commands.clear();
}




