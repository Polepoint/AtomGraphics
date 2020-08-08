package com.neo.atomgraphics;

import java.net.URL;

public class GraphicsConfiguration {

    private static GraphicsConfiguration sGlobalConfiguration;
    private URL mBaseURL;
    private int mConnectionTimeout = 5000;
    private int mReadTimeout = 10000;

    public static GraphicsConfiguration globalConfiguration() {
        if (sGlobalConfiguration == null) {
            sGlobalConfiguration = new GraphicsConfiguration();
        }

        return sGlobalConfiguration;
    }

    public URL getBaseURL() {
        return mBaseURL;
    }

    public void setBaseURL(URL baseURL) {
        this.mBaseURL = baseURL;
    }

    public int getNetworkConnectionTimeout() {
        return mConnectionTimeout;
    }

    public void setNetworkConnectionTimeout(int connectionTimeout) {
        mConnectionTimeout = connectionTimeout;
    }

    public int getNetworkReadTimeout() {
        return mReadTimeout;
    }

    public void setNetworkReadTimeout(int readTimeout) {
        mReadTimeout = readTimeout;
    }
}
