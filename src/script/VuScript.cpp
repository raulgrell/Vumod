#include "VuScript.h"

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>

extern "C" {
#include <wren/wren.h>
}

#include "script/lib.h"

static WrenConfiguration config = {};
static WrenVM *vm = nullptr;

std::vector<std::string> VuScript::commands;
char VuScript::command_text[4096] = "System.print(\"Scripting enabled!\")";

std::unordered_map<std::string, WrenForeignMethodFn> boundForeignMethods {
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

std::unordered_map<std::string, WrenForeignClassMethods> boundForeignClasses {
        {"vectorVec3", {vec3fAllocate, vec3fFinalize}}
};

WrenForeignMethodFn
bindForeignMethod(WrenVM *, const char *module, const char *className, bool isStatic, const char *signature)
{
    std::string fullSignature {module};
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

char *loadModule(WrenVM *vm, const char *name)
{
    std::string path(name);
    path += ".wren";
    std::ifstream fin;
    fin.open(path, std::ios::in);
    std::stringstream buffer;
    buffer << fin.rdbuf() << '\0';
    std::string source = buffer.str();

    char *cbuffer = (char *) malloc(source.size());
    memcpy(cbuffer, source.c_str(), source.size());
    return cbuffer;
}

WrenForeignClassMethods bindForeignClass(WrenVM *, const char *module, const char *className)
{
    std::string identifier {module};
    identifier += className;
    auto it = boundForeignClasses.find(identifier);
    if (it != boundForeignClasses.end()) {
        return it->second;
    }
    return WrenForeignClassMethods {nullptr, nullptr};
}

static void write(WrenVM *vm, const char *text)
{
    std::cout << text;
};

VuScript::VuScript()
{
    wrenInitConfiguration(&config);
    config.bindForeignMethodFn = bindForeignMethod;
    config.bindForeignClassFn = bindForeignClass;
    config.writeFn = write;
    config.loadModuleFn = loadModule;
    config.writeFn = write;

    vm = wrenNewVM(&config);
    WrenInterpretResult result = wrenInterpret(vm, "script", command_text);

    if (result != WREN_RESULT_SUCCESS)
        fatal_error("Could not run script.");
}

VuScript::~VuScript()
{
    wrenFreeVM(vm);
}

void VuScript::InterpretCommands()
{
    for (auto &command : commands) {
        WrenInterpretResult result = wrenInterpret(vm, "script", command.c_str());
        if (result != WREN_RESULT_SUCCESS)
            fatal_error("Could not run command.");
    }
    commands.clear();
}