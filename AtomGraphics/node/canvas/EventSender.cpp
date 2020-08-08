//
// Created by neo on 2020/4/29.
//

#include "EventSender.h"

namespace AtomGraphics {

void EventSender::dispatchEventSoon(EventType eventType, Element *element) {
    switch (eventType) {
        case EventTypeOnLoad:
            m_pendingLoadEvents.insert(ElementEvent(eventType, element));
            break;
        case EventTypeOnError:
            m_pendingErrorEvents.insert(ElementEvent(eventType, element));
            break;
        case EventTypeOnAbort:
            m_pendingAbortEvents.insert(ElementEvent(eventType, element));
            break;
        default:
            break;
    }

    if (!m_derefEventTimer.isActive()) {
        m_derefEventTimer.startOneShot(0);
    }
}

void EventSender::dispatchEventImmediately(EventType eventType, Element *element) {
    std::set<ListenerEntry> listeners;
    switch (eventType) {
        case EventTypePageScopeWillBeDestroyed:
            listeners = m_pageScopeDestroyListener;
            break;
        default:
            break;
    }

    for (auto &listener : listeners) {
        listener.listener->onEvent(eventType, element);
    }
}

void EventSender::cancelEvent(EventType eventType, Element *element) {
    switch (eventType) {
        case EventTypeOnLoad:
            m_pendingLoadEvents.erase(ElementEvent(eventType, element));
            break;
        case EventTypeOnError:
            m_pendingErrorEvents.erase(ElementEvent(eventType, element));
            break;
        case EventTypeOnAbort:
            m_pendingAbortEvents.erase(ElementEvent(eventType, element));
            break;
        default:
            break;
    }
}

void EventSender::listenToEvent(EventType eventType, EventListener *listener, Element *element) {
    switch (eventType) {
        case EventTypeOnLoad:
            m_loadEventListener.insert(ListenerEntry(listener, element));
            break;
        case EventTypeOnError:
            m_errorEventListener.insert(ListenerEntry(listener, element));
            break;
        case EventTypeOnAbort:
            m_abortEventListener.insert(ListenerEntry(listener, element));
            break;
        case EventTypePageScopeWillBeDestroyed:
            m_pageScopeDestroyListener.insert(ListenerEntry(listener, element));
            break;
        default:
            break;
    }
}

inline void EventSender::removeListener(std::set<ListenerEntry> &listeners, EventListener *listener) {
    for (auto &li:listeners) {
        if (li.listener == listener) {
            listeners.erase(li);
            break;
        }
    }
}

void EventSender::removeListener(EventType eventType, EventListener *listener) {
    switch (eventType) {
        case EventTypeOnLoad:
            removeListener(m_loadEventListener, listener);
            break;
        case EventTypeOnError:
            removeListener(m_errorEventListener, listener);
            break;
        case EventTypeOnAbort:
            removeListener(m_abortEventListener, listener);
            break;
        case EventTypePageScopeWillBeDestroyed:
            removeListener(m_pageScopeDestroyListener, listener);
            break;
        default:
            break;
    }
}

void EventSender::timerFired() {
    dispatchPendingEvent(EventTypeOnLoad);
    dispatchPendingEvent(EventTypeOnError);
    dispatchPendingEvent(EventTypeOnAbort);
}

void EventSender::dispatchPendingEvent(EventType eventType) {
    std::set<ElementEvent> eventToBeSend;
    std::set<ListenerEntry> listeners;
    switch (eventType) {
        case EventTypeOnLoad:
            eventToBeSend.swap(m_pendingLoadEvents);
            listeners = m_loadEventListener;
            break;
        case EventTypeOnError:
            eventToBeSend.swap(m_pendingErrorEvents);
            listeners = m_errorEventListener;
            break;
        case EventTypeOnAbort:
            eventToBeSend.swap(m_pendingAbortEvents);
            listeners = m_abortEventListener;
            break;
        default:
            break;
    }

    for (auto &event : eventToBeSend) {
        Element *element = event.element;
        for (auto &listener : listeners) {
            if (listener.element == element) {
                listener.listener->onEvent(eventType, element);
            }
        }
    }
}

}