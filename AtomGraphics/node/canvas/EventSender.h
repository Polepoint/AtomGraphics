//
// Created by neo on 2020/4/29.
//

#ifndef ATOMGRAPHICS_EVENTSENDER_H
#define ATOMGRAPHICS_EVENTSENDER_H

#include <set>
#include <mutex>
#include "thread/Timer.h"
#include "thread/GraphicsThread.h"
#include "Element.h"

namespace AtomGraphics {

enum EventType {
    EventTypeOnLoad,
    EventTypeOnError,
    EventTypeOnAbort,
    EventTypePageScopeWillBeDestroyed
};

struct ElementEvent {

    ElementEvent(EventType type, Element *element)
            : type(type), element(element) {
    }

    ElementEvent(const ElementEvent &other) {
        type = other.type;
        element = other.element;
    }

    bool operator<(const ElementEvent &rhs) const {
        if (type != rhs.type) {
            return type < rhs.type;
        }

        if (element != rhs.element) {
            return element < rhs.element;
        }

        return false;
    }

    bool operator==(const ElementEvent &rhs) const {
        return type == rhs.type && element == rhs.element;
    }

    EventType type;
    Element *element;
};


class EventListener {

public:

    virtual void onEvent(EventType type, Element *element) = 0;

};


class EventSender {

public:

    static EventSender *sender() {
        static EventSender *sender;
        static std::once_flag onceFlag;
        std::call_once(onceFlag, [] {
            sender = new EventSender;
        });
        return sender;
    }

    EventSender()
            : m_derefEventTimer(GraphicsThread::GraphicsThreadTaskRunner(),
            *this, &EventSender::timerFired) {

    }

    void dispatchEventSoon(EventType eventType, Element *element);

    void dispatchEventImmediately(EventType eventType, Element *element);

    void cancelEvent(EventType eventType, Element *element);

    void listenToEvent(EventType eventType, EventListener *listener, Element *element);

    void removeListener(EventType eventType, EventListener *listener);

private:


    struct ListenerEntry {
        EventListener *listener;
        Element *element;

        ListenerEntry(EventListener *listener, Element *element)
                : listener(listener), element(element) {
        }

        ListenerEntry(const ListenerEntry&other){
            listener = other.listener;
            element = other.element;
        }

        bool operator<(const ListenerEntry &rhs) const {
            if (listener != rhs.listener) {
                return listener < rhs.listener;
            }
            if (element != rhs.element) {
                return element < rhs.element;
            }
            return false;
        }

        bool operator==(const ListenerEntry &rhs) const {
            return listener == rhs.listener &&
                    element == rhs.element;
        }
    };

    std::set<ListenerEntry> m_loadEventListener;
    std::set<ListenerEntry> m_errorEventListener;
    std::set<ListenerEntry> m_abortEventListener;
    std::set<ListenerEntry> m_pageScopeDestroyListener;

    std::set<ElementEvent> m_pendingLoadEvents;
    std::set<ElementEvent> m_pendingErrorEvents;
    std::set<ElementEvent> m_pendingAbortEvents;


    Timer m_derefEventTimer;

    void timerFired();

    void dispatchPendingEvent(EventType eventType);

    void removeListener(std::set<ListenerEntry> &listeners, EventListener *listener);
};


}


#endif //ATOMGRAPHICS_EVENTSENDER_H
