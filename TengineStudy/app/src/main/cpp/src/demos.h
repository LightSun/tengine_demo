//
// Created by Administrator on 2020/9/24 0024.
//

#ifndef TENGINESTUDY_DEMOS_H
#define TENGINESTUDY_DEMOS_H

#include "ext.h"

namespace openpose {
    extern "C" int openpose_main(TengineArgs *args);
}

namespace landmark{
    extern "C" int landmark_main(TengineArgs *args);
}

#endif //TENGINESTUDY_DEMOS_H
