//
// Created by neo on 2019/3/31.
//

#include "JSPageScopeTracer.h"

namespace AtomGraphics {

namespace JS {

static std::stack<JSPageScope *> s_pageScopeStack;

JSPageScope *JSPageScopeTracer::CurrentPageScope() {
    if (!s_pageScopeStack.empty()) {
        return s_pageScopeStack.top();
    } else {
        return nullptr;
    }
}

JSPageScopeTracer::JSPageScopeTracer(JSPageScope *pageScope) : m_pageScope(pageScope) {
    s_pageScopeStack.push(m_pageScope);
}

JSPageScopeTracer::~JSPageScopeTracer() {
    s_pageScopeStack.pop();
}

}

}
