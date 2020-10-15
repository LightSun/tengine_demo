//
// Created by Administrator on 2020/10/13 0013.
//

#ifndef TENGINESTUDY_TGWRAPPER_H
#define TENGINESTUDY_TGWRAPPER_H

#include "tengine_c_api.h"
#include "GraphParam.h"
#include "opencv2/core/mat.hpp"

class TgWrapper{
protected:
    graph_t mGraph;
    tensor_t input_tensor;
    GraphParam* gp;
    cv::Mat frame;
    tensor_t out_tensor;
    int* outDims;

    void* mInput_data;
    const char* uniqueId;

public:
    inline static void initEngine(){ initEngine();}
    inline static void destroyEngine(){release_tengine();}

    void destroy();

    void createGraph(const char* model_format, const char* file_name);

    bool getInputTensor(int input_node_idx, int tensor_idx);
    /**
     * set tensor shape. default is float
     * @param gp the graph parameter
     */
    bool setTensorShape(GraphParam*& gp);

    /**
     * set input buffer.
     * @param frame  the frame as input buffer
     * @param uniqueId the id of this frame. this is often used for generate result.
     */
    void setInputBuffer(cv::Mat& frame, const char* uniqueId);
    void setInputBuffer(const char* frameFile, const char* uniqueId);

    void preRunGraph();

    bool runGraph(int repeatCount, bool block);//c++ default value must at the ends

    bool getOutputTensor(int input_node_idx, int tensor_idx);

    //========== result should order in here ========
    void postProcess();

    void postRunGraph();

    inline graph_t getGraph(){
        return mGraph;
    }
    inline float* getInputBufferAsFloat(){
        return static_cast<float *>(mInput_data);
    }
    inline int getImageSize(){
        return gp->getImageSize();
    }
};

#endif //TENGINESTUDY_TGWRAPPER_H
