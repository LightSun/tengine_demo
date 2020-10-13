//
// Created by Administrator on 2020/9/22 0022.
//
#include <jni.h>
#include "ext.h"
#include "demos.h"

#define JAVA_PREFIX                                     com_heaven7_android_tenginestudy
#define CONCAT(prefix, class, func)                     Java_ ## prefix ## _ ## class ## _ ## func
#define CONCAT_SURFACE(prefix, func)                    CONCAT(prefix, MainActivity, func)

#define SURFACE_VIEW_JAVA_API1(func, p1)            CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass, p1)

#define call(name, args) name::name##_main(args)


extern "C" JNIEXPORT jint Java_com_heaven7_android_tenginestudy_MainActivity_runMain(JNIEnv
*env,jclass, jint argc, jstring prefix,
jobjectArray arr
){
//EC_JNIEXPORT jint JNICALL SURFACE_VIEW_JAVA_API1(runMain, jobjectArray arr){
    jsize len = env->GetArrayLength(arr);
    TengineArgs targs;
    targs.prefix = const_cast<char *>(env->GetStringUTFChars(prefix, nullptr));
    targs.outDir = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 0)), nullptr));
    targs.model_file = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 1)), nullptr));
    targs.image_file = const_cast<char *>(env->GetStringUTFChars(static_cast<jstring>(env->GetObjectArrayElement(arr, 2)), nullptr));

    int result = call(openpose, &targs);

    jstring jstr0 = static_cast<jstring>(env->GetObjectArrayElement(arr, 0));
    env->ReleaseStringUTFChars(jstr0, targs.outDir);
    env->DeleteLocalRef(jstr0);

    jstring jstr1 = static_cast<jstring>(env->GetObjectArrayElement(arr, 1));
    env->ReleaseStringUTFChars(jstr1, targs.model_file);
    env->DeleteLocalRef(jstr1);

    jstring jstr2 = static_cast<jstring>(env->GetObjectArrayElement(arr, 2));
    env->ReleaseStringUTFChars(jstr2, targs.image_file);
    env->DeleteLocalRef(jstr2);

    env->ReleaseStringUTFChars(prefix, targs.prefix);
    return result;
}