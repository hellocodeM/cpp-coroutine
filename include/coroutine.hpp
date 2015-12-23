#ifndef MING_COROUTINE_COROUTINE_HPP
#define MING_COROUTINE_COROUTINE_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdint.h>
#include <ucontext.h>

namespace ming {
namespace coroutine {

using co_function_t = std::function<void()>;
using co_function_ptr = void (*)();
using co_id_t = std::size_t;

enum CoState {
    kReady,
    kRunning,
    kYield,
    kFinish
};

struct Coroutine {
    Coroutine() = delete;
    Coroutine(const Coroutine&) = default;

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
};

/**
 * Create a coroutine.
 */
Coroutine coroutine(co_function_t fn);

/**
 * Yield a coroutine.
 */
void yield();

}/* end of namespace coroutine */
}/* end of namespace ming */
#endif
