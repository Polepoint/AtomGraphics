package com.neo.atomgraphics.reflect;

import android.graphics.RenderNode;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ReflectUtils {

    private static Method sHiddenForName;
    private static Method sHiddenGetMethod;
    private static Class sRenderNode$PositionUpdateListenerClass;
    private static Method sRenderNode$AddPositionUpdateListener;
    private static Method sRenderNode$RemovePositionUpdateListener;

    public static void addPositionUpdateListener(RenderNode renderNode, PositionUpdateListener listener) {
        try {

            initHiddenClassesAndMethods();

            if (sRenderNode$PositionUpdateListenerClass == null) {
                return;
            }

            PositionUpdateListenerImpl impl = new PositionUpdateListenerImpl(listener);
            Object listenerProxy = Proxy.newProxyInstance(sRenderNode$PositionUpdateListenerClass.getClassLoader(),
                    new Class[]{sRenderNode$PositionUpdateListenerClass}, impl);

            if (sRenderNode$AddPositionUpdateListener == null) {
                sRenderNode$AddPositionUpdateListener = (Method) sHiddenGetMethod.invoke(
                        renderNode.getClass(),
                        "addPositionUpdateListener",
                        new Class[]{sRenderNode$PositionUpdateListenerClass});
            }

            if (sRenderNode$AddPositionUpdateListener != null) {
                sRenderNode$AddPositionUpdateListener.invoke(renderNode, listenerProxy);
            }

            listener.setHiddenListener(listenerProxy);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void removePositionUpdateListener(RenderNode renderNode, PositionUpdateListener listener) {
        try {

            initHiddenClassesAndMethods();

            if (sRenderNode$RemovePositionUpdateListener == null) {
                sRenderNode$RemovePositionUpdateListener = (Method) sHiddenGetMethod.invoke(
                        renderNode.getClass(),
                        "removePositionUpdateListener",
                        new Class[]{sRenderNode$PositionUpdateListenerClass});
            }

            if (sRenderNode$RemovePositionUpdateListener != null) {
                sRenderNode$RemovePositionUpdateListener.invoke(renderNode, listener.getHiddenListener());
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void initHiddenClassesAndMethods() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        if (sHiddenForName == null) {
            sHiddenForName = Class.class.getMethod("forName", String.class);
        }

        if (sHiddenGetMethod == null) {
            sHiddenGetMethod = Class.class.getMethod("getMethod", String.class, Class[].class);
        }

        if (sRenderNode$PositionUpdateListenerClass == null) {
            sRenderNode$PositionUpdateListenerClass = (Class) sHiddenForName.invoke(null, "android.graphics.RenderNode$PositionUpdateListener");
        }
    }

    private static class PositionUpdateListenerImpl implements InvocationHandler {

        private PositionUpdateListener mListener;

        PositionUpdateListenerImpl(PositionUpdateListener listener) {
            mListener = listener;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

            if ("positionChanged".equals(method.getName()) && args.length == 5) {
                Long frameNumber = (Long) args[0];
                Integer left = (Integer) args[1];
                Integer top = (Integer) args[2];
                Integer right = (Integer) args[3];
                Integer bottom = (Integer) args[4];
                mListener.positionChanged(frameNumber, left, top, right, bottom);
            } else if ("positionLost".equals(method.getName()) && args.length == 1) {
                Long frameNumber = (Long) args[0];
                mListener.positionLost(frameNumber);
            }

            return proxy;
        }
    }

}
