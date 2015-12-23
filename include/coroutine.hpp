#ifndef MING_COROUTINE_COROUTINE_HPP
#define MING_COROUTINE_COROUTINE_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdint.h>
#include <ucontext.h>

#include "types.hpp"
#include "CoroutineScheduler.hpp"

namespace ming {
namespace coroutine {

enum CoState { kReady, kRunning, kYield, kFinish };

struct Coroutine {
    Coroutine() = delete;
    Coroutine(const Coroutine&) = delete;

    Coroutine(co_function_t, co_id_t);

    /**
     * Resume coroutine.
     */
    void operator()();

    constexpr static size_t kStackSize = 10240;
    uint8_t stack[kStackSize];
    co_function_t fn;
    co_id_t cid;
    ucontext_t context;
    CoState state;
};

/**
 * Create a coroutine.
 */
Coroutine& coroutine(co_function_t fn);

/**
 * Yield the coroutine.
 */
void yield();
} /* end of namespace coroutine */
} /* end of namespace ming */
#endif
