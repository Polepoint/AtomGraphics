//
// Created by neo on 2018/10/23.
//

#include <v8/v8.h>
#include <v8/libplatform/libplatform.h>
#include <bits/sysconf.h>
#include <linux/resource.h>
#include <sys/resource.h>
#include "thread/UIThread.h"
#include "V8Platform.h"
#include "V8ReferenceMarkingVisitor.h"
#include "V8GCController.h"
#include "V8IsolateData.h"


using namespace v8;

namespace AtomGraphics {

namespace JS {

V8Platform *V8Platform::SharedPlatform() {
    static V8Platform *sharedPlatform;
    if (!sharedPlatform) {
        sharedPlatform = new V8Platform();
    }
    return sharedPlatform;
}

V8Platform::V8Platform() {
    m_platform = v8::platform::NewDefaultPlatform(3);
    v8::V8::InitializePlatform(m_platform.get());
    v8::V8::Initialize();

    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    create_params.constraints.ConfigureDefaults(AmountOfPhysicalMemory(), AmountOfVirtualMemory());
    create_params.allow_atomics_wait = UIThread::IsMainThread();
    create_params.only_terminate_in_safe_scope = true;
    m_isolate = Isolate::New(create_params);

    V8IsolateData::Initialize(m_isolate);

    m_isolate->AddGCPrologueCallback(V8GCController::GcPrologue);
    m_isolate->AddGCEpilogueCallback(V8GCController::GcEpilogue);
}

Isolate *V8Platform::getIsolate() const {
    return m_isolate;
}

struct SystemMemoryInfo {
    uint64_t physicalMemorySize = 0;
    uint64_t virtualMemorySize = 0;
};

static SystemMemoryInfo *systemMemoryInfo = nullptr;

void InitialSystemMemoryInfo(SystemMemoryInfo **systemMemoryInfo) {
    *systemMemoryInfo = new SystemMemoryInfo();

    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGESIZE);
    if (pages == -1 || page_size == -1) {
        return;
    }

    (*systemMemoryInfo)->physicalMemorySize = static_cast<uint64_t>(pages) * page_size;

    struct rlimit limit;
    int result = getrlimit(RLIMIT_DATA, &limit);
    if (result != 0) {
        return;
    }

    (*systemMemoryInfo)->virtualMemorySize = limit.rlim_cur == RLIM_INFINITY ? 0 : limit.rlim_cur;

}

uint64_t V8Platform::AmountOfPhysicalMemory() {
    if (!systemMemoryInfo) {
        InitialSystemMemoryInfo(&systemMemoryInfo);
    }

    return systemMemoryInfo->physicalMemorySize;
}

uint64_t V8Platform::AmountOfVirtualMemory() {
    if (!systemMemoryInfo) {
        InitialSystemMemoryInfo(&systemMemoryInfo);
    }

    return systemMemoryInfo->virtualMemorySize;
}

} //namespace JS

} //namespace GraphicsEnvironment