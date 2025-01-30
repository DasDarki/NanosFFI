//
// Created by DasDarki on 29.01.2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

// -- Hooking --
#define SIG_SCRIPTING_ENV_CTOR "48 89 5C 24 10 48 89 4C 24 08 55 56 57 48 83 EC 30 49 8B D9"

// -- Console --
#define S_CONSOLE_LOG "48 83 EC 58 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40 80 3D ? ? ? ? ? 74 6A 48 83 7A 18 0F"
#define N_CONSOLE_LOG "Console::Log"

#define S_CONSOLE_ERROR "48 89 5C 24 18 57 48 81 EC 80 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 70 48 8B DA 48 8B F9 80 3D ? ? ? ? ? 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 48 8D 54 24 50 FF 50 40 90 48 83 78 18 0F 76 03 48 8B 00 4C 8B C3 48 83 7B 18 0F 76 03 4C 8B 03 4C 8B C8 48 8D 15 ? ? ? ? 48 8D 4C 24 30 E8 ? ? ? ? 90 48 8B D0 B1 07"
#define N_CONSOLE_ERROR "Console::Error"

#define S_CONSOLE_WARN "48 89 5C 24 18 57 48 81 EC 80 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 70 48 8B DA 48 8B F9 80 3D ? ? ? ? ? 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 48 8D 54 24 50 FF 50 40 90 48 83 78 18 0F 76 03 48 8B 00 4C 8B C3 48 83 7B 18 0F 76 03 4C 8B 03 4C 8B C8 48 8D 15 ? ? ? ? 48 8D 4C 24 30 E8 ? ? ? ? 90 48 8B D0 B1 06"
#define N_CONSOLE_WARN "Console::Warn"

#define S_CONSOLE_RUN_COMMAND "40 53 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40 48 8B DA 80 3D ? ? ? ? ?"
#define N_CONSOLE_RUN_COMMAND "Console::RunCommand"

#endif //SIGNATURES_H
