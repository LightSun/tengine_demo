//
// Created by Administrator on 2020/10/13 0013.
//


#include <stdlib.h>
#include <string.h>
#include "GraphParam.h"
#include "demos.h"

GraphParam::~GraphParam() {
    if(outDir != nullptr){
        free(outDir);
        outDir = nullptr;
    }
}
int GraphParam::getImageSize() {
    return channels * width * height;
}

int GraphParam::getDims(int *arr) {
    arr[0] = n;
    arr[1] = channels;
    arr[2] = width;
    arr[3] = height;
    return 4;
}
void GraphParam::setOutDir(const char *od) {
   outDir = static_cast<char *>(malloc(strlen(od) + 1));
   strcpy(outDir, od);
}
char* GraphParam::getOutDir() {
    return outDir;
}

#define JAVA_PREFIX                                     com_heaven7_android_tenginestudy
#define CONCAT(prefix, class, func)                     Java_ ## prefix ## _ ## class ## _ ## func
#define CONCAT_SURFACE(prefix, func)                    CONCAT(prefix, GraphParam, func)
#define SURFACE_VIEW_JAVA_API1(func, p1)                CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass, p1)
#define SURFACE_VIEW_JAVA_API(func)                     CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass)

#define SURFACE_VIEW_JAVA_API_o(func)                     CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jobject, jlong ptr)
#define SURFACE_VIEW_JAVA_API1_o(func, p1)                 CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jobject, jlong ptr, p1)

extern "C" JNIEXPORT jlong JNICALL Java_com_heaven7_android_tenginestudy_GraphParam_nCreate(JNIEnv
                                                                                             *env,jclass){
    GraphParam * gp = new GraphParam();
    return reinterpret_cast<jlong>(gp);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nRelease(JNIEnv *env, jclass clazz, jlong ptr) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    delete gp;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_ngetN(JNIEnv *env, jobject thiz, jlong ptr) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->n;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_ngetC(JNIEnv *env, jobject thiz, jlong ptr) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->channels;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_ngetW(JNIEnv *env, jobject thiz, jlong ptr) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->width;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_ngetH(JNIEnv *env, jobject thiz, jlong ptr) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    return gp->height;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetN(JNIEnv *env, jobject thiz, jlong ptr,
                                                       jint val) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->n = val;
}

EC_JNIEXPORT void JNICALL SURFACE_VIEW_JAVA_API1_o(nsetC, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->channels = val;
}

EC_JNIEXPORT void JNICALL SURFACE_VIEW_JAVA_API1_o(nsetW, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->width = val;
}
EC_JNIEXPORT void JNICALL SURFACE_VIEW_JAVA_API1_o(nsetH, jint val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->height = val;
}
EC_JNIEXPORT void JNICALL SURFACE_VIEW_JAVA_API1_o(nsetOutputDir, jstring val){
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    OP_JSTRING(env, val, gp->setOutDir(str));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetStype(JNIEnv *env, jobject thiz, jlong ptr,
                                                           jint type) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->stype = type;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetUnitSize(JNIEnv *env, jobject thiz, jlong ptr,
                                                              jint unit_size) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->unitSize = unit_size;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetThreadCount(JNIEnv *env, jobject thiz,
                                                                 jlong ptr, jint n) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->num_thread = n;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetCluster(JNIEnv *env, jobject thiz, jlong ptr,
                                                             jint n) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->cluster = n;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_tenginestudy_GraphParam_nsetPrecision(JNIEnv *env, jobject thiz, jlong ptr,
                                                               jint n) {
    GraphParam * gp = reinterpret_cast<GraphParam *>(ptr);
    gp->precision = n;
}