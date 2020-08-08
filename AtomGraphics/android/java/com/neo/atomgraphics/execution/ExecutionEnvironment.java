package com.neo.atomgraphics.execution;

import android.content.Context;

public interface ExecutionEnvironment {

    void onExecution(Context context, long pageID);

    void destroy();

}
