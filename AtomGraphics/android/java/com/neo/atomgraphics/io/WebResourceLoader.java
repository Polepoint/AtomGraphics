package com.neo.atomgraphics.io;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.neo.atomgraphics.GraphicsConfiguration;
import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.thread.GraphicsThread;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class WebResourceLoader {

    @CalledByNative
    static void loadImage(final String url, final long callbackPtr) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                final Bitmap bitmap = getImage(url);
                GraphicsThread.dispatch(new Runnable() {
                    @Override
                    public void run() {
                        onImageLoaded(callbackPtr, bitmap);
                    }
                });
            }
        }).start();
    }

    private static Bitmap getImage(String urlString) {
        GraphicsConfiguration configuration = GraphicsConfiguration.globalConfiguration();
        HttpURLConnection urlConnection = null;
        URL url = null;
        try {
            url = new URL(urlString);
        } catch (MalformedURLException e) {
            try {
                url = new URL(GraphicsConfiguration.globalConfiguration().getBaseURL(), urlString);
            } catch (MalformedURLException ee) {
                Log.e("ATG", ee.getMessage(), ee);
            }
        }

        if (url == null) {
            return null;
        }

        try {
            urlConnection = (HttpURLConnection) url.openConnection();
            //设置请求方法
            urlConnection.setRequestMethod("GET");
            //设置超时时间
            urlConnection.setConnectTimeout(configuration.getNetworkConnectionTimeout());
            urlConnection.setReadTimeout(configuration.getNetworkReadTimeout());

            //获取响应的状态码
            int responseCode = urlConnection.getResponseCode();
            if (responseCode == 200) {
                InputStream in = urlConnection.getInputStream();

                Bitmap bitmap = BitmapFactory.decodeStream(in);
                close(in);
                return bitmap;
            } else {
                return null;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
        }

        return null;
    }

    private static void close(Closeable stream) {
        if (stream != null) {
            try {
                stream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static native void onImageLoaded(long nativePtr, Bitmap result);

}
