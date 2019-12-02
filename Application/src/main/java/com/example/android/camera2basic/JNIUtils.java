package com.example.android.camera2basic;

import android.media.Image;
import android.view.Surface;
import java.nio.ByteBuffer;

public class JNIUtils {
    // TAG for JNIUtils class
    private static final String TAG = "JNIUtils";

    // Load native library.
    static {
        System.loadLibrary("native-lib");
    }

    public static native void YUVtoRGBADisplay(int srcWidth, int srcHeight, int Y_rowStride, ByteBuffer Y_Buffer, ByteBuffer U_Buffer, Surface surface);
    public static native void GrayScaleDisplay(int srcWidth, int srcHeight, int rowStride, ByteBuffer srcBuffer, Surface surface);
    public static native void RegionColorDisplay(int mtouchX, int mtouchY, int srcWidth, int srcHeight, int Y_rowStride, ByteBuffer Y_Buffer, ByteBuffer U_Buffer, Surface surface);
    public static native void ColorPickDisplay(int mtouchX, int mtouchY, int srcWidth, int srcHeight, int Y_rowStride, ByteBuffer Y_Buffer, ByteBuffer U_Buffer, Surface surface);
}
