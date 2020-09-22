//
// Created by Administrator on 2020/9/22 0022.
//

#ifndef TENGINESTUDY_EXT_H
#define TENGINESTUDY_EXT_H

#define TENGINE_MODE_UINT8 1
#define TENGINE_MODE_FP16 2
#define TENGINE_MODE_FP32 3

typedef struct options{
    int num_thread;
    int cluster;
    int precision;
}options;

typedef struct TengineArgs{
    char* outDir;
    char* outFile;
    char* model_file;
    char* image_file;
    int repeat_count;
    int num_thread;
}TengineArgs;

#endif //TENGINESTUDY_EXT_H
