//
// Created by Administrator on 2020/9/22 0022.
//

#ifndef TENGINESTUDY_EXT_H
#define TENGINESTUDY_EXT_H

#define TENGINE_MODE_UINT8 1
#define TENGINE_MODE_FP16 2
#define TENGINE_MODE_FP32 3

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TengineArgs {
    char * prefix;
    char *outDir;
    char *outFile;
    char *model_file;
    char *image_file;
    int repeat_count;
    int num_thread;
} TengineArgs;

const char *concatStr(const char *s1, const char *s2);

const char *concatStr2(const char *s1,const char *s2,  const char *s3);
#ifdef __cplusplus
}
#endif

#endif //TENGINESTUDY_EXT_H
