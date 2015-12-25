#ifndef MING_COROUTINE_COROUTINE_HPP
#define MING_COROUTINE_COROUTINE_HPP

#include <ucontext.h>
#include <cassert>
#include <cstring>
#include <memory>

#include "types.hpp"

namespace ming {
namespace coroutine {

class Coroutine;

/* current running coroutine */
extern Coroutine* g_current;

/* common stack used by all coroutines */
extern co_stack_t g_common_stack[kStackSize];

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
            PrepareStack();                                 \
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

#define YIELD_COMMON   \
    getcontext(&ctx_); \
    RestoreStack();    \
    state_ = kYield;   \
    swapcontext(&ctx_, &main_ctx_);
    /**
     * Yield the coroutine, and return a value.
     */
    template <class T>
    void Yield(T&& x) {
        // transmit a variable to main_ctx_ by register rax
        using tt = typename std::decay<T>::type;
        ctx_.uc_mcontext.gregs[REG_RAX] = reinterpret_cast<greg_t>(new tt(x));
        YIELD_COMMON
    }

    /**
     * Yield, return a pointer.
     */
    template <class T>
    void Yield(T* x) {
        ctx_.uc_mcontext.gregs[REG_RAX] = reinterpret_cast<greg_t>(x);
        YIELD_COMMON
    }

    /**
     * Yield, return nothing.
     */
    void Yield();

private:
    /**
     * Copy the coroutine stack to the common stack
     */
    void PrepareStack();

    /**
     * Store the coroutine stack.
     */
    void RestoreStack();

    /**
     * To invoke a lambda by the function pointer,
     * we need to wrap the std::function by a plain function.
     * Its' another job is going back the main context.
     */
    void CoroutineGuard();

    /**
     * Some helper functions to copy the stack
     */
    inline size_t stack_used() const { return g_common_stack + kStackSize - stack_top_; }

    inline co_stack_t* stack() { return stack_.get(); }

    inline co_stack_t* common_stack_top() const {
        return reinterpret_cast<co_stack_t*>(ctx_.uc_mcontext.gregs[REG_RSP]);
    }

    inline size_t common_stack_used() const {
        return g_common_stack + kStackSize - common_stack_top();
    }

    /* data members */
    ucontext_t ctx_, main_ctx_;
    co_function_t fn_;
    std::unique_ptr<co_stack_t> stack_;
    co_stack_t* stack_top_;
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
