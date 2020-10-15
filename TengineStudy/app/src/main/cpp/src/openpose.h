//
// Created by Administrator on 2020/10/14 0014.
//

#ifndef TENGINESTUDY_OPENPOSE_H
#define TENGINESTUDY_OPENPOSE_H

#include "tgwrapper.h"

class Openpose : public TgWrapper{
private:

public:
    void setInputBuffer(cv::Mat& frame, const char* uniqueId);
    void setInputBuffer(const char* frameFile, const char* uniqueId);
    void postProcess();

    static int getNPoints(int type);
    //static int** getPosePairs(int type); //c++can't cast int arr[14][2] to int **
};

#endif //TENGINESTUDY_OPENPOSE_H
