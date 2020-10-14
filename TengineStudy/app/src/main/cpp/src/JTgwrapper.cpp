//
// Created by Administrator on 2020/10/14 0014.
//

#include "tgwrapper.h"
#include "openpose.h"

#include <jni.h>
#include <android/bitmap.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "log.h"
#include "demos.h"

#define OPENPOSE 1

extern "C"
JNIEXPORT jlong JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nCreate(JNIEnv *env, jclass clazz, jint type) {
    TgWrapper* ptr = 0;
    switch(type){
        case OPENPOSE:
            ptr = new Openpose();
            break;
        default:
            LOGW("unknown type = %d", type);
    }
    return reinterpret_cast<jlong>(ptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nRelease(JNIEnv *env, jclass clazz, jlong ptr, jint type) {
    switch(type){
        case OPENPOSE:
            Openpose* tw = reinterpret_cast<Openpose *>(ptr);
            delete(tw);
            break;

        default:
            LOGW("unknown type = %d", type);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_ndestroy(JNIEnv *env, jobject thiz, jlong ptr) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    tw->destroy();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_npreRunGraph(JNIEnv *env, jobject thiz, jlong ptr) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    tw->preRunGraph();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_npostProcess(JNIEnv *env, jobject thiz, jlong ptr) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    tw->postProcess();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_npostRunGraph(JNIEnv *env, jobject thiz,
                                                              jlong ptr) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    tw->postRunGraph();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_ngetInputTensor(JNIEnv *env, jobject thiz,
                                                                jlong ptr, jint input_node_idx,
                                                                jint tensor_idx) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    return tw->getInputTensor(input_node_idx, tensor_idx);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_ngetOutputTensor(JNIEnv *env, jobject thiz,
                                                                 jlong ptr, jint input_node_idx,
                                                                 jint tensor_idx) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    return tw->getOutputTensor(input_node_idx, tensor_idx);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nsetTensorShape(JNIEnv *env, jobject thiz,
                                                                jlong ptr, jlong gp_ptr) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    GraphParam* gp = reinterpret_cast<GraphParam *>(gp_ptr);
    tw->setTensorShape(gp);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_ncreateGraph(JNIEnv *env, jobject thiz, jlong ptr,
                                                             jstring format, jstring module_file) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    OP_JSTRING2(env, format, module_file, tw->createGraph(str1, str2))
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nsetInputBuffer(JNIEnv *env, jobject thiz,
                                                                jlong ptr, jobject bitmap,
                                                                jstring id) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);

    AndroidBitmapInfo info;
    memset(&info, 0, sizeof(info));
    AndroidBitmap_getInfo(env, bitmap, &info);

    if (info.width <= 0 || info.height <= 0 ||(info.format != ANDROID_BITMAP_FORMAT_RGB_565 && info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)) {
        LOGE("invalid bitmap\n");
        return;
    }
    void * pixels = NULL;
    //lock
    int res = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (pixels == NULL) {
        LOGE("fail to lock bitmap: %d\n", res);
        return ;
    }
    //cv::Mat frame = cv::Mat(info.height,info.width,CV_8UC4,pixels);

    cv::Mat sample = cv::Mat(info.height,info.width,CV_8UC4,pixels);
    cv::Mat img;
    if (sample.channels() == 4){
        cv::cvtColor(sample, img, cv::COLOR_BGRA2BGR);
    }
    else if (sample.channels() == 1){
        cv::cvtColor(sample, img, cv::COLOR_GRAY2BGR);
    }
    else{
        img = sample;
    }
    OP_JSTRING(env, id, tw->setInputBuffer(img, str))
    //unlock
    AndroidBitmap_unlockPixels(env, bitmap);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nsetInputBuffer2(JNIEnv *env, jobject thiz,
                                                                 jlong ptr, jstring img_file,
                                                                 jstring id) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    OP_JSTRING2(env, img_file, id, tw->setInputBuffer(str1, str2))
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nsetInputBuffer3(JNIEnv *env, jobject thiz,
                                                                 jlong ptr, jbyteArray data,
                                                                 jint width, jint height, jstring id) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
//YUV420SP to BGR
    jbyte * pBuf = env->GetByteArrayElements(data, 0);
    cv::Mat image(height + height/2, width, CV_8UC1, (unsigned char *)pBuf);
    cv::Mat mBgr;
    cvtColor(image, mBgr, CV_YUV2BGR_NV21);
    OP_JSTRING(env, id, tw->setInputBuffer(mBgr, str))

    env->ReleaseByteArrayElements(data, pBuf, 0);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nrunGraph(JNIEnv *env, jobject thiz, jlong ptr,
                                                          jint repeat_count, jboolean block) {
    TgWrapper* tw = reinterpret_cast<TgWrapper *>(ptr);
    return tw->runGraph(repeat_count, block);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_ninitEngine(JNIEnv *env, jclass clazz) {
    TgWrapper::initEngine();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_TgWrapper_nDestroyEngine(JNIEnv *env, jclass clazz) {
    TgWrapper::destroyEngine();
}