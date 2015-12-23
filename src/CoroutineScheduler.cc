#include <cassert>

#include "CoroutineScheduler.hpp"

namespace ming {
namespace coroutine {    

Coroutine& CoroutineScheduler::Create(co_function_t fn) {
    auto co = std::make_shared<Coroutine>(fn, ++count_);
    coroutines_.emplace(count_, co);
    return *co.get();
}

void CoroutineScheduler::Resume(co_id_t id) {
    auto iter = coroutines_.find(id);
    if (iter != coroutines_.end()) {
        auto co = iter->second;
        auto& ctx = co->context;
        current_ = co->cid;
        switch (co->state) {
            case kReady:
                getcontext(&ctx);
                makecontext(&ctx, reinterpret_cast<co_function_ptr>(&CoroutineScheduler::CoroutineGuard), 2, this, id);
            case kYield:
                co->state = kRunning;
                swapcontext(&main_ctx_, &ctx);
                break;
            case kRunning:
            case kFinish:
            default:
                assert(false);
        }
    }
}

void CoroutineScheduler::Yield(co_id_t id) {
    auto iter = coroutines_.find(id);
    if (iter != coroutines_.end()) {
        auto co = iter->second;
        if (co->state == kRunning) {
            co->state = kYield;
            swapcontext(&co->context, &main_ctx_);
        }
    }
}

void CoroutineScheduler::Yield() {
    Yield(current_);
}

void CoroutineScheduler::CoroutineGuard(co_id_t id) {
    auto co = coroutines_[id];
    co->fn();
    coroutines_.erase(id);
    setcontext(&main_ctx_);
}

co_id_t CoroutineScheduler::count_ = 0;

CoroutineScheduler& get_scheduler() {
    static CoroutineScheduler cs;
    return cs;
}

} /* end of namespace coroutine */
} /* end of namespace ming */
