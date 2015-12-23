#ifndef MING_COROUTINE_SCHEDULER_HPP
#define MING_COROUTINE_SCHEDULER_HPP

#include <map>
#include <memory>
#include <ucontext.h>

#include "types.hpp"
#include "coroutine.hpp"

namespace ming {
namespace coroutine {

struct Coroutine;
/**
 * Singleton scheduler.
 */
class CoroutineScheduler {
public:
    Coroutine& Create(co_function_t);

    void Resume(co_id_t id);

    void Yield(co_id_t id);
    
    void Yield();

private:
    void CoroutineGuard(co_id_t id);

    /* data members */
    std::map<co_id_t, std::shared_ptr<Coroutine>> coroutines_;
    ucontext_t main_ctx_;
    static co_id_t count_;
    co_id_t current_;
};

CoroutineScheduler& get_scheduler(); 

} /* end of namespace coroutine */
} /* end of namespace ming */
#endif
