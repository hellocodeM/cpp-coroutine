#ifndef MING_COROUTINE_COROUTINE_HPP
#define MING_COROUTINE_COROUTINE_HPP

#include <ucontext.h>
#include <cassert>

#include "types.hpp"

namespace ming {
namespace coroutine {

class Coroutine;

/* current running coroutine */
extern Coroutine* g_current;

class Coroutine {
public:
    Coroutine() = delete;
    Coroutine(const Coroutine&) = delete;

    Coroutine(co_function_t);

    ~Coroutine() = default;

/**
 * the macro for resume function, this function has many version,
 * so we need to overload it.
 */
#define RESUME_UPPER_HALF                                   \
    union {                                                 \
        void (Coroutine::*fn)();                            \
        co_function_ptr ptr;                                \
    } ptr_converter;                                        \
    ptr_converter.fn = &Coroutine::CoroutineGuard;          \
    g_current = this;                                       \
    switch (state_) {                                       \
        case kReady:                                        \
            getcontext(&ctx_);                              \
            makecontext(&ctx_, ptr_converter.ptr, 1, this); \
        case kYield:                                        \
            state_ = kRunning;                              \
            swapcontext(&main_ctx_, &ctx_);

#define RESUME_LOWER_HALF \
    break;                \
    case kRunning:        \
    case kFinish:         \
    default:              \
        assert(false);    \
        }

    /**
     * Resume the coroutine, and expect the return value of the coroutine.
     */
    template <class T>
    auto Resume() -> std::enable_if_t<!std::is_pointer<T>::value, T> {
        RESUME_UPPER_HALF {
            T* ret_ptr = reinterpret_cast<T*>(ctx_.uc_mcontext.gregs[REG_RAX]);
            T ret = *ret_ptr;
            delete ret_ptr;
            return ret;
        }
        RESUME_LOWER_HALF
    }

    /**
     * Resume, return a pointer.
     */
    template <class T>
    auto Resume() -> std::enable_if_t<std::is_pointer<T>::value, T> {
        RESUME_UPPER_HALF {
            T ret = reinterpret_cast<T>(ctx_.uc_mcontext.gregs[REG_RAX]);
            return ret;
        }
        RESUME_LOWER_HALF
    }

    /**
     * Resume, return nothing.
     */
    void Resume();

    /**
     * Yield the coroutine, and return a value.
     */
    template <class T>
    void Yield(T&& x) {
        // transmit a variable to main_ctx_ by register rax
        using tt = typename std::decay<T>::type;
        ctx_.uc_mcontext.gregs[REG_RAX] = reinterpret_cast<greg_t>(new tt(x));
        state_ = kYield;
        swapcontext(&ctx_, &main_ctx_);
    }

    /**
     * Yield, return a pointer.
     */
    template <class T>
    void Yield(T* x) {
        ctx_.uc_mcontext.gregs[REG_RAX] = reinterpret_cast<greg_t>(x);
        state_ = kYield;
        swapcontext(&ctx_, &main_ctx_);
    }

    /**
     * Yield, return nothing.
     */
    void Yield();

private:
    /**
     * To invoke a lambda by the function pointer,
     * we need to wrap the std::function by a plain function.
     * Its' another job is going back the main context.
     */
    void CoroutineGuard();

    /* data members */
    ucontext_t ctx_, main_ctx_;
    co_function_t fn_;
    co_stack_t stack_;
    CoroutineState state_ = kReady;
};

/**
 * Yield a coroutine
 */
template <class T>
void yield(T&& x) {
    assert(g_current && "no coroutine is running!");
    g_current->Yield(std::forward<T>(x));
}

void yield();

} /* end of namespace coroutine */
} /* end of namespace ming */
#endif
