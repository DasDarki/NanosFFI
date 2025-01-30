//
// Created by DasDarki on 29.01.2025.
//

#include "nanosffi/loader.h"

#include "hijacker.h"
#include "binder.h"

#define BIND(name) Binder::bind(S_##name, N_##name);

void NanosFFI_Loader::inject() {
#if defined(_WIN32) || defined(_WIN64)
    if (MH_Initialize() != MH_OK) {
        return;
    }
#endif

    std::cout << "[NANOSFFI-INF] Injecting NanosFFI..." << std::endl;

    hook::find_addr_set();

    if (!Hijacker::hijack()) {
        return;
    }

    std::cout << "[NANOSFFI-INF] Injected NanosFFI." << std::endl;
    std::cout << "[NANOSFFI-INF] Binding functions..." << std::endl;

    bindAll();

    if (bound_failed > 0) {
        std::cerr << "[NANOSFFI-ERR] Failed to bind " << bound_failed << " functions. " << "Successfully bound " << bound_success << " functions. (" << bound_total << " total)" << std::endl;

        std::cerr << "[NANOSFFI-ERR] Failed bindings:" << std::endl;
        for (const auto &name : mFailedBindings) {
            std::cerr << name << std::endl;
        }
    } else {
        std::cout << "[NANOSFFI-INF] Successfully bound " << bound_success << " functions. (" << bound_total << " total)" << std::endl;
    }
}

void NanosFFI_Loader::bindAll() {
    BIND(CONSOLE_LOG);
    BIND(CONSOLE_ERROR);
    BIND(CONSOLE_WARN);
    BIND(CONSOLE_RUN_COMMAND);
}


