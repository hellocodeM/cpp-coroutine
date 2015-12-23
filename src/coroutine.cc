#include "coroutine.hpp"
#include "CoroutineScheduler.hpp"

namespace ming {
namespace coroutine {

Coroutine::Coroutine(co_function_t f, co_id_t id): fn(f), cid(id), state(kReady) {
    context.uc_stack.ss_sp = &stack;
    context.uc_stack.ss_size = kStackSize;
    context.uc_stack.ss_flags = 0;
}

void Coroutine::operator() () {
    get_scheduler().Resume(this->cid);
}

Coroutine& coroutine(co_function_t fn) { return get_scheduler().Create(fn); }

void yield() { get_scheduler().Yield(); }

} /* end of namespace coroutine */
} /* end of namespace ming */
