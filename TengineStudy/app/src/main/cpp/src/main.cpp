//
// Created by Administrator on 2020/9/22 0022.
//
#include <jni.h>

#define EC_JNIEXPORT extern "C" JNIEXPORT
#define JAVA_PREFIX                                     com_heaven7_android_tenginestudy
#define CONCAT(prefix, class, func)                     Java_ ## prefix ## _ ## class ## _ ## func
#define CONCAT_SURFACE(prefix, func)                    CONCAT(prefix, MainActivity, func)

#define SURFACE_VIEW_JAVA_API1(func, p1)            CONCAT_SURFACE(JAVA_PREFIX, func)(JNIEnv* env, jclass, p1)

int main(int argc, char* argv[]);

EC_JNIEXPORT jint JNICALL SURFACE_VIEW_JAVA_API1(runMain, jobjectArray arr){
    jsize len = env->GetArrayLength(arr);
    const char* args[len];
    for (int i = 0; i < len; ++i) {
        jstring jstr = static_cast<jstring>(env->GetObjectArrayElement(arr, i));
        args[i] = env->GetStringUTFChars(jstr, nullptr);
        env->DeleteLocalRef(jstr);
    }
    int result = main(len, const_cast<char **>(args));

    for (int i = 0; i < len; ++i) {
        jstring jstr = static_cast<jstring>(env->GetObjectArrayElement(arr, i));
        env->ReleaseStringUTFChars(jstr, args[i]);
        env->DeleteLocalRef(jstr);
    }
    return result;
}