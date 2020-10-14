//
// Created by Administrator on 2020/9/24 0024.
//

#ifndef TENGINESTUDY_DEMOS_H
#define TENGINESTUDY_DEMOS_H

#include "ext.h"
#include "jni.h"
#define EC_JNIEXPORT extern "C" JNIEXPORT

#define OP_JSTRING(env, jstr, op) \
const char* str = env->GetStringUTFChars(jstr, nullptr); \
op; \
env->ReleaseStringUTFChars(jstr, str);

#define OP_JSTRING2(env, jstr1, jstr2, op) \
const char* str1 = env->GetStringUTFChars(jstr1, nullptr); \
const char* str2 = env->GetStringUTFChars(jstr2, nullptr); \
op; \
env->ReleaseStringUTFChars(jstr1, str1); \
env->ReleaseStringUTFChars(jstr2, str2);

#define API_MAIN(name)\
namespace name {\
    extern "C" int name##_main(TengineArgs *args);\
}

API_MAIN(openpose)
API_MAIN(landmark)


#endif //TENGINESTUDY_DEMOS_H
