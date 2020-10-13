//
// Created by Administrator on 2020/10/13 0013.
//

#ifndef TENGINESTUDY_TGWRAPPER_H
#define TENGINESTUDY_TGWRAPPER_H

#include "tengine_c_api.h"
#include "GraphParam.h"
#include "opencv2/core/mat.hpp"

class TgWrapper{
public:
    static void init();
    static void destroy();
    static graph_t initModule(const char* file);

    void setTensorShape(const GraphParam* gp);
};

class Openpose{
public:
    void setInputBuffer(const cv::Mat* mat);
};
#endif //TENGINESTUDY_TGWRAPPER_H
