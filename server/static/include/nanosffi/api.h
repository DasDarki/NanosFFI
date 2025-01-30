//
// Created by DasDarki on 29.01.2025.
//

#ifndef API_H
#define API_H

#if defined(_WIN32) || defined(_WIN64)
#define EXPORT extern "C" __declspec(dllexport)
#elif defined(__linux__)
#define EXPORT extern "C"
#endif

using NanosScriptingEnv = void*;

// Console
EXPORT void NanosFFI_Console_Log(NanosScriptingEnv env, const char* message);
EXPORT void NanosFFI_Console_Warn(NanosScriptingEnv env, const char* message);
EXPORT void NanosFFI_Console_Error(NanosScriptingEnv env, const char* message);
EXPORT void NanosFFI_Console_RunCommand(NanosScriptingEnv env, const char* command);

#endif //API_H
