#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#define  JNICALL


#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Camera2Demo", __VA_ARGS__)

//convert Y Plane from YUV_420_888 to RGBA and display
extern "C" {
JNIEXPORT void JNICALL Java_com_example_android_camera2basic_JNIUtils_YUVtoRGBADisplay(
        JNIEnv *env, jclass obj, jint srcWidth, jint srcHeight, jint Y_rowStride, jobject Y_Buffer,
        jobject U_Buffer, jobject surface){

    uint8_t *srcYPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(Y_Buffer));
    uint8_t *srcUPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(U_Buffer));

    ANativeWindow * window = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_acquire(window);
    ANativeWindow_Buffer buffer;
    ANativeWindow_setBuffersGeometry(window, srcWidth, srcHeight, WINDOW_FORMAT_RGBA_8888);
    if (int32_t err = ANativeWindow_lock(window, &buffer, NULL)) {
        ANativeWindow_release(window);
    }
    uint8_t * outPtr = reinterpret_cast<uint8_t *>(buffer.bits);
    for (size_t y = 0; y < srcHeight; y++)
    {
        uint8_t * Y_rowPtr = srcYPtr + y * Y_rowStride;
        uint8_t * UV_rowPtr = srcUPtr + (y >> 1) * Y_rowStride;
        for (size_t x = 0; x < srcWidth; x++)
        {
            uint8_t Y = Y_rowPtr[x];
            size_t uIndex = x & 0xfffffffe;
            uint8_t U = UV_rowPtr[uIndex];
            uint8_t V = UV_rowPtr[uIndex + 1];
            double R = ((Y-16) * 1.164 + (V-128) * 1.596);
            double G = ((Y-16) * 1.164 - (U-128) * 0.392 - (V-128) * 0.813);
            double B = ((Y-16) * 1.164 + (U-128) * 2.017);
            *(outPtr++) = (uint8_t) (R > 255 ? 255 : (R < 0 ? 0 : R));
            *(outPtr++) = (uint8_t) (G > 255 ? 255 : (G < 0 ? 0 : G));
            *(outPtr++) = (uint8_t) (B > 255 ? 255 : (B < 0 ? 0 : B));
            *(outPtr++) = 255; // gamma for RGBA_8888
        }
    }
    ANativeWindow_unlockAndPost(window);
    ANativeWindow_release(window);
}

JNIEXPORT void JNICALL Java_com_example_android_camera2basic_JNIUtils_GrayScaleDisplay(
        JNIEnv *env, jclass obj, jint srcWidth, jint srcHeight, jint rowStride, jobject srcBuffer, jobject surface){

    uint8_t *srcLumaPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(srcBuffer));
    ANativeWindow * window = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_acquire(window);
    ANativeWindow_Buffer buffer;
    ANativeWindow_setBuffersGeometry(window, srcWidth, srcHeight, WINDOW_FORMAT_RGBA_8888);
    if (int32_t err = ANativeWindow_lock(window, &buffer, NULL)) {
        ANativeWindow_release(window);
    }
    uint8_t * outPtr = reinterpret_cast<uint8_t *>(buffer.bits);
    for (size_t y = 0; y < srcHeight; y++)
    {
        uint8_t * rowPtr = srcLumaPtr + y * rowStride;
        //LOGE("rowptr : %d",rowPtr);
        for (size_t x = 0; x < srcWidth; x++)
        {
            //for grayscale output, just duplicate the Y channel into R, G, B channels
            *(outPtr++) = *rowPtr; //R
            *(outPtr++) = *rowPtr; //G
            *(outPtr++) = *rowPtr; //B
            *(outPtr++) = 255; // gamma for RGBA_8888
            ++rowPtr;
        }
    }
    ANativeWindow_unlockAndPost(window);
    ANativeWindow_release(window);
}
JNIEXPORT void JNICALL Java_com_example_android_camera2basic_JNIUtils_RegionColorDisplay(
        JNIEnv *env, jclass obj, jint touchX, jint touchY, jint srcWidth, jint srcHeight,
        jint Y_rowStride, jobject Y_Buffer, jobject U_Buffer, jobject surface){

    uint8_t *srcYPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(Y_Buffer));
    uint8_t *srcUPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(U_Buffer));

    ANativeWindow * window = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_acquire(window);
    ANativeWindow_Buffer buffer;
    ANativeWindow_setBuffersGeometry(window, srcWidth, srcHeight, WINDOW_FORMAT_RGBA_8888);
    if (int32_t err = ANativeWindow_lock(window, &buffer, NULL)) {
        ANativeWindow_release(window);
    }
    uint8_t * outPtr = reinterpret_cast<uint8_t *>(buffer.bits);
    for (size_t y = 0; y < srcHeight; y++)
    {
        uint8_t * Y_rowPtr = srcYPtr + y * Y_rowStride;
        uint8_t * UV_rowPtr = srcUPtr + (y >> 1) * Y_rowStride;
        for (size_t x = 0; x < srcWidth; x++)
        {
            uint8_t Y = Y_rowPtr[x];
            size_t uIndex = x & 0xfffffffe;
            uint8_t U = UV_rowPtr[uIndex];
            uint8_t V = UV_rowPtr[uIndex + 1];
            double R = ((Y-16) * 1.164 + (V-128) * 1.596);
            double G = ((Y-16) * 1.164 - (U-128) * 0.392 - (V-128) * 0.813);
            double B = ((Y-16) * 1.164 + (U-128) * 2.017);
            if(y > touchY-110 && y < touchY+110 && x > touchX-150 && x < touchX+150){
                *(outPtr++) = (uint8_t) (R > 255 ? 255 : (R < 0 ? 0 : R));
                *(outPtr++) = (uint8_t) (G > 255 ? 255 : (G < 0 ? 0 : G));
                *(outPtr++) = (uint8_t) (B > 255 ? 255 : (B < 0 ? 0 : B));
                *(outPtr++) = 255; // gamma for RGBA_8888
            }else{
                *(outPtr++) = Y;
                *(outPtr++) = Y;
                *(outPtr++) = Y;
                *(outPtr++) = 255;
            }
        }
    }
    ANativeWindow_unlockAndPost(window);
    ANativeWindow_release(window);
}
JNIEXPORT void JNICALL Java_com_example_android_camera2basic_JNIUtils_ColorPickDisplay(
        JNIEnv *env, jclass obj, jint touchX, jint touchY, jint srcWidth, jint srcHeight,
        jint Y_rowStride, jobject Y_Buffer,jobject U_Buffer, jobject surface){
    uint8_t *srcYPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(Y_Buffer));
    uint8_t *srcUPtr = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(U_Buffer));

    ANativeWindow * window = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_acquire(window);
    ANativeWindow_Buffer buffer;
    ANativeWindow_setBuffersGeometry(window, srcWidth, srcHeight, WINDOW_FORMAT_RGBA_8888);
    if (int32_t err = ANativeWindow_lock(window, &buffer, NULL)) {
        ANativeWindow_release(window);
    }
    uint8_t * outPtr = reinterpret_cast<uint8_t *>(buffer.bits);
    int count = 0;
    double meanR = 0;
    double meanG = 0;
    double meanB = 0;
    double color_dis = 0;
    for (size_t y = 0; y < srcHeight; y++)
    {
        uint8_t * Y_rowPtr = srcYPtr + y * Y_rowStride;
        uint8_t * UV_rowPtr = srcUPtr + (y >> 1) * Y_rowStride;
        for (size_t x = 0; x < srcWidth; x++)
        {
            uint8_t Y = Y_rowPtr[x];
            size_t uIndex = x & 0xfffffffe;
            uint8_t U = UV_rowPtr[uIndex];
            uint8_t V = UV_rowPtr[uIndex + 1];
            if(y > touchY-15 && y < touchY+15 && x > touchX-30 && x < touchX+30){
                count += 1;
                meanR += ((Y-16) * 1.164 + (V-128) * 1.596);
                meanG += ((Y-16) * 1.164 - (U-128) * 0.392 - (V-128) * 0.813);
                meanB += ((Y-16) * 1.164 + (U-128) * 2.017);
            }
        }
    }
    meanR /= count;
    meanG /= count;
    meanB /= count;
    for (size_t y = 0; y < srcHeight; y++)
    {
        uint8_t * Y_rowPtr = srcYPtr + y * Y_rowStride;
        uint8_t * UV_rowPtr = srcUPtr + (y >> 1) * Y_rowStride;
        for (size_t x = 0; x < srcWidth; x++)
        {
            uint8_t Y = Y_rowPtr[x];
            size_t uIndex = x & 0xfffffffe;
            uint8_t U = UV_rowPtr[uIndex];
            uint8_t V = UV_rowPtr[uIndex + 1];
            double R = ((Y-16) * 1.164 + (V-128) * 1.596);
            double G = ((Y-16) * 1.164 - (U-128) * 0.392 - (V-128) * 0.813);
            double B = ((Y-16) * 1.164 + (U-128) * 2.017);
            color_dis = (meanR-R)*(meanR-R) + (meanG-G)*(meanG-G) + (meanB-B)*(meanB-B);

            if(color_dis < 8000){
                *(outPtr++) = (uint8_t) (R > 255 ? 255 : (R < 0 ? 0 : R));
                *(outPtr++) = (uint8_t) (G > 255 ? 255 : (G < 0 ? 0 : G));
                *(outPtr++) = (uint8_t) (B > 255 ? 255 : (B < 0 ? 0 : B));
                *(outPtr++) = 255; // gamma for RGBA_8888
            }else{
                *(outPtr++) = Y;
                *(outPtr++) = Y;
                *(outPtr++) = Y;
                *(outPtr++) = 255;
            }
        }
    }
    ANativeWindow_unlockAndPost(window);
    ANativeWindow_release(window);
}
}
