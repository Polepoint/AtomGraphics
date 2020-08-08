
#import <pthread/pthread.h>
#include "MainThread.h"


namespace AtomGraphics {

//bool isMainThread() {
//    if (mainThreadEstablishedAsPthreadMain) {
//        ASSERT(!mainThreadPthread);
//        return pthread_main_np();
//    }
//
//    ASSERT(mainThreadPthread);
//    return pthread_equal(pthread_self(), mainThreadPthread);
//}
//
//void callOnMainThread(std::function<void()> &&function) {
//    ASSERT(function);
//
//    bool needToSchedule = false;
//
//    {
//        std::lock_guard<Lock> lock(mainThreadFunctionQueueMutex);
//        needToSchedule = functionQueue().size() == 0;
//        functionQueue().append(WTFMove(function));
//    }
//
//    if (needToSchedule)
//        scheduleDispatchFunctionsOnMainThread();
//
//}

}