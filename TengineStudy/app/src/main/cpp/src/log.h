//
// Created by Administrator on 2020/7/7 0007.
//

#ifndef FFMPEGOPENGLDEMO_LOG_H
#define FFMPEGOPENGLDEMO_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define TAG "tengine_study"

    //J->ThrowNew(env, J->FindClass(env, "java/io/IOException"), "invalid bitmap");
#ifdef ANDROID
    #include "android/log.h"
    #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
    #define DBG(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else
    //empty. later will dosomething
    #define LOGV(...) static_cast<void>(0)
    #define LOGD(...) static_cast<void>(0)
    #define LOGW(...) static_cast<void>(0)
    #define LOGE(...) static_cast<void>(0)
    #define DBG(...) static_cast<void>(0)
#endif


#ifdef __cplusplus
}
#endif

#endif //FFMPEGOPENGLDEMO_LOG_H
