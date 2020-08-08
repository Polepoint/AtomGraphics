
#import "UIThread.h"
#import "TaskRunnerCF.h"


namespace AtomGraphics {

static pthread_t s_uiThread = 0;

static TaskRunnerCF *s_uiTaskRunner;

void UIThread::InitUIThread() {
    s_uiThread = pthread_self();
    s_uiTaskRunner = new TaskRunnerCF();
}

TaskRunner *UIThread::UIThreadTaskRunner() {
    return s_uiTaskRunner;
}

bool UIThread::IsMainThread() {
    return pthread_equal(pthread_self(), s_uiThread) != 0;
}

}