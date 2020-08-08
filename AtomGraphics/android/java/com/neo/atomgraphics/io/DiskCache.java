package com.neo.atomgraphics.io;

import android.util.Log;

import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.base.ContextUtils;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class DiskCache {

    private static File sCacheFolder;

    @CalledByNative
    private static boolean checkCachePath(String cacheFolderName) {

        if (sCacheFolder == null) {
            sCacheFolder = new File(ContextUtils.getApplicationContext().getCacheDir(), cacheFolderName);
        }

        if (sCacheFolder.exists()) {
            deleteFolder(sCacheFolder);
        }

        return sCacheFolder.mkdirs();
    }

    private static void deleteFolder(File dir) {
        if (dir == null || !dir.exists() || !dir.isDirectory())
            return;
        for (File file : dir.listFiles()) {
            if (file.isFile()) {
                if (!file.delete()) {
                    Log.e("ATG", "failed to delete file: " + file.getName());
                }
            } else if (file.isDirectory())
                deleteFolder(file);
        }
        if (!dir.delete()) {
            Log.e("ATG", "failed to delete folder: " + dir.getName());
        }
    }


    @CalledByNative
    private static void readCachedFile(String fileName, long nativeBuffer) {
        File cacheFile = new File(sCacheFolder, fileName);
        if (cacheFile.exists()) {
            try {
                FileInputStream is = new FileInputStream(cacheFile);

                BufferedInputStream bufferedInputStream = new BufferedInputStream(is);
                byte[] buffer = new byte[bufferedInputStream.available()];
                int bufferSize = bufferedInputStream.read(buffer);
                bufferedInputStream.close();
                is.close();
                writeBufferData(nativeBuffer, buffer, bufferSize);
            } catch (IOException e) {
                Log.e("ATG", e.getMessage(), e);
            }
        }
    }

    @CalledByNative
    private static void writeFile(String fileName, byte[] buffer) {
        File cacheFile = new File(sCacheFolder, fileName);
        try {
            cacheFile.createNewFile();
            FileOutputStream outputStream = new FileOutputStream(cacheFile);
            BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(outputStream);
            bufferedOutputStream.write(buffer);
            bufferedOutputStream.close();
            outputStream.close();
        } catch (IOException e) {
            Log.e("ATG", e.getMessage(), e);
        }
    }

    private static native void writeBufferData(long nativeBuffer, byte[] buffer, long bufferSize);
}
