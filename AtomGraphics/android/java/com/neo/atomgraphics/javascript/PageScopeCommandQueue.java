package com.neo.atomgraphics.javascript;

import com.neo.atomgraphics.thread.GraphicsThread;

import java.util.ArrayList;
import java.util.List;


public class PageScopeCommandQueue {

    public static final int Type_EvaluateScript = 0;
    public static final int Type_CallJSHandler = 1;
    public static final int Type_RegisterNativeHandler = 2;

    class Command {
        private int mType;
        private Object data0;
        private Object data1;

        Command(int type, Object data0, Object data1) {
            this.mType = type;
            this.data0 = data0;
            this.data1 = data1;
        }
    }

    private List<Command> mCommandQueue;

    public PageScopeCommandQueue() {
        mCommandQueue = new ArrayList<>();
    }

    public void addCommand(int type, Object data0, Object data1) {
        mCommandQueue.add(new Command(type, data0, data1));
    }

    public void flushCommand(final JSPageScope pageScope) {
        final List<Command> pendingFlushCommand = new ArrayList<>(mCommandQueue);
        mCommandQueue.clear();
        GraphicsThread.dispatch(new Runnable() {
            @Override
            public void run() {
                for (Command command : pendingFlushCommand) {
                    switch (command.mType) {
                        case Type_EvaluateScript:
                            pageScope.evaluateScript((String) command.data0);
                            break;
                        case Type_CallJSHandler:
                            pageScope.callJSHandler((String) command.data0, (String) command.data1);
                            break;
                        case Type_RegisterNativeHandler:
                            pageScope.registerNativeHandler((String) command.data0, (JSMessageHandler) command.data1);
                            break;
                    }
                }
            }
        });
    }

    public boolean isEmpty() {
        return mCommandQueue.isEmpty();
    }
}
