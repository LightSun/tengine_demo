//
// Created by Administrator on 2020/9/22 0022.
//
#include <jni.h>
#include "ext.h"

#define EC_JNIEXPORT extern "C" JNIEXPORT
#define JAVA_PREFIX                                     com_heaven7_android_tenginestudy
#define CONCAT(prefix, class, func)                     Java_ ## prefix ## _ ## class ## _ ## func
#define CONCAT_SURFACE(prefix, func)                    CONCAT(prefix, MainActivity, func)

#define SURFACE_VIEW_JAVA_API1(func, p1)            CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass, p1)

extern "C" int test_main(TengineArgs* args);

extern "C" JNIEXPORT jint Java_com_heaven7_android_tenginestudy_MainActivity_runMain(JNIEnv
*env,jclass, jint argc,
jobjectArray arr
){
//EC_JNIEXPORT jint JNICALL SURFACE_VIEW_JAVA_API1(runMain, jobjectArray arr){
    jsize len = env->GetArrayLength(arr);
    TengineArgs targs;
    char* args[len];

    targs.outFile = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 0)), nullptr));
    targs.model_file = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 1)), nullptr));
    targs.image_file = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 2)), nullptr));
   /* for (int i = 0; i < len; ++i) {
        jstring jstr = static_cast<jstring>(env->GetObjectArrayElement(arr, i));
        args[i] = const_cast<char *>(env->GetStringUTFChars(jstr, nullptr));
        env->DeleteLocalRef(jstr);
    }*/
    int result = test_main(&targs);
/*
    for (int i = 0; i < len; ++i) {
        jstring jstr = static_cast<jstring>(env->GetObjectArrayElement(arr, i));
        env->ReleaseStringUTFChars(jstr, args[i]);
        env->DeleteLocalRef(jstr);
    }*/
    jstring jstr0 = static_cast<jstring>(env->GetObjectArrayElement(arr, 0));
    env->ReleaseStringUTFChars(jstr0, targs.outFile);
    env->DeleteLocalRef(jstr0);

    jstring jstr1 = static_cast<jstring>(env->GetObjectArrayElement(arr, 1));
    env->ReleaseStringUTFChars(jstr1, targs.model_file);
    env->DeleteLocalRef(jstr1);

    jstring jstr2 = static_cast<jstring>(env->GetObjectArrayElement(arr, 2));
    env->ReleaseStringUTFChars(jstr2, targs.image_file);
    env->DeleteLocalRef(jstr2);
    return result;
}