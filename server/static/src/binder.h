//
// Created by DasDarki on 29.01.2025.
//

#ifndef BINDER_H
#define BINDER_H

#include <map>

#include "pattern.h"

static int bound_success = 0;
static int bound_failed = 0;
static int bound_total = 0;
inline std::map<std::string, void*> mBoundFunctions;
inline std::vector<std::string> mFailedBindings;

class Binder {
public:

    static void bind(const std::string &sig, const std::string &name) {
        bound_total++;

        auto func = hook::find_pattern(sig);
        if (func == 0) {
            bound_failed++;
            mFailedBindings.push_back(name);
            return;
        }

        auto ptr = reinterpret_cast<void *>(func);

        mBoundFunctions[std::string(name.c_str())] = ptr;
        bound_success++;
    }

    template<typename T>
    static T get(const std::string &name) {
        return reinterpret_cast<T>(mBoundFunctions[name]);
    }
};

#endif //BINDER_H
