//
// Created by Administrator on 2020/10/13 0013.
//


#include "GraphParam.h"
#include "demos.h"

int GraphParam::getImageSize() {
    return channels * width * height;
}

void GraphParam::getDims(int *arr) {
    arr[0] = n;
    arr[1] = channels;
    arr[2] = width;
    arr[3] = height;
}

#define JAVA_PREFIX                                     com_heaven7_android_tenginestudy
#define CONCAT(prefix, class, func)                     Java_ ## prefix ## _ ## class ## _ ## func
#define CONCAT_SURFACE(prefix, func)                    CONCAT(prefix, GraphParam, func)
#define SURFACE_VIEW_JAVA_API1(func, p1)                CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass, p1)
#define SURFACE_VIEW_JAVA_API(func)                     CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass)

#define SURFACE_VIEW_JAVA_API_o(func)                     CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jobject jo, jlong ptr)
#define SURFACE_VIEW_JAVA_API1_o(func, p1)                 CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jobject jo, jlong ptr, p1)

EC_JNIEXPORT jlong SURFACE_VIEW_JAVA_API(_nCreate){
    GraphParam * gp = new GraphParam();
    return reinterpret_cast<jlong>(gp);
}

EC_JNIEXPORT void SURFACE_VIEW_JAVA_API1(_nRelease, jlong ptr){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    delete gp;
}

EC_JNIEXPORT jint SURFACE_VIEW_JAVA_API_o(_getN){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->n;
}

EC_JNIEXPORT jint SURFACE_VIEW_JAVA_API_o(_getC){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->channels;
}
EC_JNIEXPORT jint SURFACE_VIEW_JAVA_API_o(_getW){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->width;
}
EC_JNIEXPORT jint SURFACE_VIEW_JAVA_API_o(_getH){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->height;
}

EC_JNIEXPORT void SURFACE_VIEW_JAVA_API1_o(_setN, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->n = val;
}

EC_JNIEXPORT void SURFACE_VIEW_JAVA_API1_o(_setC, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->channels = val;
}

EC_JNIEXPORT void SURFACE_VIEW_JAVA_API1_o(_setW, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->width = val;
}
EC_JNIEXPORT void SURFACE_VIEW_JAVA_API1_o(_setH, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->height = val;
}
