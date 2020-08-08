//
// Created by neo on 2019-09-18.
//

#import "JSPageScopeTracer.h"
#import <stack>

namespace AtomGraphics {

namespace JS {

static std::stack<JSPageScope *> s_pageScopeStack;

JSPageScope *JSPageScopeTracer::CurrentPageScope() {
    if (!s_pageScopeStack.empty()) {
        return s_pageScopeStack.top();
    } else {
        return NULL;
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
