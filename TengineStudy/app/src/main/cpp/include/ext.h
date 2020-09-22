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

#endif //TENGINESTUDY_EXT_H
