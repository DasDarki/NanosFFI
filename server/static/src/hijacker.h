//
// Created by DasDarki on 29.01.2025.
//

#ifndef HIJACKER_H
#define HIJACKER_H

#if defined(_WIN32) || defined(_WIN64)
    #include <MinHook.h>
#endif

#include <iostream>
#include <map>

#include "pattern.h"
#include "_signatures.h"
#include "nanosffi/api.h"

std::map<std::string, void*> hijackedEnvs;

typedef void (__fastcall* ScriptingEnvCtor)(void* inst, const std::string* name, void* compatVers, void* engine);
ScriptingEnvCtor oScriptingEnvCtor = nullptr;

void __fastcall hkScriptingEnvCtor(void* inst, const std::string* name, void* compatVers, void* engine) {
    if (name == nullptr) {
        std::cout << "[NANOSFFI-INF] ScriptingEnvCtor called with nullptr name." << std::endl;
    } else {
        std::cout << "[NANOSFFI-INF] ScriptingEnvCtor called: " << *name << std::endl;

        hijackedEnvs[*name] = inst;

        NanosFFI_Console_Log(inst, "Hello from NanosFFI!");
    }

    oScriptingEnvCtor(inst, name, compatVers, engine);
}

class Hijacker {
public:
    static bool hijack() {
        auto funcAddr = hook::find_pattern(SIG_SCRIPTING_ENV_CTOR);
        if (funcAddr == 0) {
            std::cerr << "[NANOSFFI-ERR] Failed to find ScriptingEnvCtor." << std::endl;
            return false;
        }

#if defined(_WIN32) || defined(_WIN64)
        if (MH_CreateHook((LPVOID)funcAddr, &hkScriptingEnvCtor, reinterpret_cast<LPVOID*>(&oScriptingEnvCtor)) != MH_OK) {
            std::cerr << "[NANOSFFI-ERR] Failed to create hook for ScriptingEnvCtor." << std::endl;
            return false;
        }

        if (MH_EnableHook((LPVOID)funcAddr) != MH_OK) {
            std::cerr << "[NANOSFFI-ERR] Failed to enable hook for ScriptingEnvCtor." << std::endl;
            return false;
        }

#endif

        std::cout << "[NANOSFFI-INF] Hooked ScriptingEnvCtor." << std::endl;

        return true;
    }
};


#endif //HIJACKER_H
