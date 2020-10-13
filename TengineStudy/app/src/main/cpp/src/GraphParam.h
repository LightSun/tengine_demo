//
// Created by Administrator on 2020/10/13 0013.
//

#ifndef TENGINESTUDY_GRAPHPARAM_H
#define TENGINESTUDY_GRAPHPARAM_H

class GraphParam{
public:
    int n;
    int channels;
    int width;
    int height;

    /*
     *  int img_size = img_h * img_w * channel;
        int dims[] = {1, channel, img_h, img_w};
     */
    int getImageSize();
    void getDims(int *dims);
};

#endif //TENGINESTUDY_GRAPHPARAM_H
