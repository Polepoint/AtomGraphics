//
// Created by neo on 2019/3/17.
//

#include <mutex>
#include "thread/UIThread.h"
#include "AndroidTaskRunner.h"

namespace AtomGraphics {

static pthread_t uiThread_ = 0;

static AndroidTaskRunner *uiTaskRunner_;

void UIThread::InitUIThread() {
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        uiThread_ = pthread_self();
        uiTaskRunner_ = new AndroidTaskRunner();
    });
}

TaskRunner *UIThread::UIThreadTaskRunner() {
    return uiTaskRunner_;
}

bool UIThread::IsMainThread() {
    return pthread_equal(pthread_self(), uiThread_) != 0;
}

}