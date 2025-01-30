//
// Created by DasDarki on 29.01.2025.
//

#include "nanosffi/api.h"

#include <iostream>
#include <string>

#include "binder.h"
#include "_signatures.h"

#define CALL_BINDED_FUNC(FType, Name, ...)                         \
    do {                                                           \
        auto func = Binder::get<FType>(Name);                      \
        if (!func) {                                               \
            std::cerr << "[NANOS-WRN] Function not found: " Name << std::endl; \
        } else {                                                   \
            func(__VA_ARGS__);                                     \
        }                                                          \
    } while (0)


EXPORT void NanosFFI_Console_Log(NanosScriptingEnv env, const char *message) {
    typedef void (*F)(NanosScriptingEnv, const std::string*);

    CALL_BINDED_FUNC(F, N_CONSOLE_LOG, env, new std::string(message));
}

EXPORT void NanosFFI_Console_Warn(NanosScriptingEnv env, const char *message) {
    typedef void (*F)(NanosScriptingEnv, const std::string*);

    CALL_BINDED_FUNC(F, N_CONSOLE_WARN, env, new std::string(message));
}

EXPORT void NanosFFI_Console_Error(NanosScriptingEnv env, const char *message) {
    typedef void (*F)(NanosScriptingEnv, const std::string*);

    CALL_BINDED_FUNC(F, N_CONSOLE_ERROR, env, new std::string(message));
}

EXPORT void NanosFFI_Console_RunCommand(NanosScriptingEnv env, const char *command) {
    typedef void (*F)(NanosScriptingEnv, const std::string*);

    CALL_BINDED_FUNC(F, N_CONSOLE_RUN_COMMAND, env, new std::string(command));
}