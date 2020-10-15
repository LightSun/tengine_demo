//
// Created by Administrator on 2020/10/13 0013.
//

#ifndef TENGINESTUDY_GRAPHPARAM_H
#define TENGINESTUDY_GRAPHPARAM_H

#define OPENPOSE_TYPE_MPI    1
#define OPENPOSE_TYPE_COCO   2
#define OPENPOSE_TYPE_BODY25 3

class GraphParam{
private:
    /** the output dir */
    char* outDir; //need release

public:
    int n;
    int channels;
    int width;
    int height;

    //see 'struct options'.
    int num_thread;
    int cluster;
    int precision;
    /**
     * the unit size of every image position. often is 'sizeof(float)'
     */
    int unitSize;
    /**
     * the sub type. such as openpose-type
     */
    int stype;
    /*
     *  int img_size = img_h * img_w * channel;
        int dims[] = {1, channel, img_h, img_w};
     */
    inline int getImageSize(){ return channels * width * height;};
    int getDims(int *dims);
    void setOutDir(const char* outDir);
    char* getOutDir();

    ~GraphParam();
};

#endif //TENGINESTUDY_GRAPHPARAM_H
