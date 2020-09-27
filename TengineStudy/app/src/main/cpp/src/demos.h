//
// Created by Administrator on 2020/9/24 0024.
//

#ifndef TENGINESTUDY_DEMOS_H
#define TENGINESTUDY_DEMOS_H

#include "ext.h"

#define API_MAIN(name)\
namespace name {\
    extern "C" int name##_main(TengineArgs *args);\
}

API_MAIN(openpose)
API_MAIN(landmark)

#endif //TENGINESTUDY_DEMOS_H
