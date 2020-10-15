//
// Created by Administrator on 2020/10/13 0013.
//

#include "tgwrapper.h"
#include "log.h"
#include "common.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

void TgWrapper::destroy() {
    if(outDims != nullptr){
        free(outDims);
        outDims = nullptr;
    }
    // Release memory for input tensor
    if(input_tensor != nullptr){
        release_graph_tensor(input_tensor);
        input_tensor = nullptr;
    }
    if(mInput_data != nullptr){
        free(mInput_data);
        mInput_data = nullptr;
    }
    // destory tengine graph
    if(mGraph != nullptr){
        destroy_graph(mGraph);
        mGraph = nullptr;
    }
}

void TgWrapper::createGraph(const char *model_format, const char *file_name) {
    mGraph = create_graph(nullptr, model_format, file_name);
}

bool TgWrapper::getInputTensor(int input_node_idx, int tensor_idx) {
    input_tensor = get_graph_input_tensor(mGraph, input_node_idx, tensor_idx);
    if (input_tensor == nullptr) {
        LOGW("Get input tensor failed");
        return false;
    }
    return true;
}

bool TgWrapper::setTensorShape(GraphParam *& gp) {
    if(input_tensor == nullptr){
        return false;
    }
    if(mInput_data != nullptr){
        return false;
    }
    mInput_data = malloc(gp->unitSize * gp->getImageSize());
    int dims[4];
    int dimNum = gp->getDims(dims);
    if(set_tensor_shape(input_tensor, dims, dimNum) < 0){
        LOGW("Set input tensor shape failed");
        return false;
    }
    outDims = new int[4];
    this->gp = gp;
    return true;
}

void TgWrapper::setInputBuffer(cv::Mat& frame,const char* uniqueId) {
    LOGW("setInputBuffer not impled");
}
void TgWrapper::setInputBuffer(const char *frameFile, const char *uniqueId) {
    LOGW("setInputBuffer not impled");
}

void TgWrapper::preRunGraph() {
    if(gp == nullptr){
        return;
    }
    if(gp->num_thread > 1){
        struct options ops;
        ops.num_thread = gp->num_thread;
        ops.cluster = gp->cluster;
        ops.precision = gp->precision;
        if(prerun_graph_multithread(mGraph, ops) < 0){
            LOGW("prerun_graph_multithread failed.");
        }
    } else{
        if(prerun_graph(mGraph) < 0){
            LOGW("prerun_graph failed.");
        }
    }
}

bool TgWrapper::runGraph(int repeatCount, bool block) {
    /* run graph */
    double min_time = __DBL_MAX__;
    double max_time = -__DBL_MAX__;
    double total_time = 0.;
    const int b = block ? 1 : 0;
    for (int i = 0; i < repeatCount; i++) {
        double start = get_current_time();
        if (run_graph(mGraph, b) < 0){
            LOGW("Run graph failed");
            return -1;
        }
        double end = get_current_time();
        double cur = end - start;
        total_time += cur;
        min_time = std::min(min_time, cur);
        max_time = std::max(max_time, cur);
    }
    LOGD("Repeat %d times, thread %d, avg time %.2f ms, max_time %.2f ms, min_time %.2f ms",
            1,
            1,
            total_time, max_time, min_time);
    LOGD("--------------------------------------\n");
    return true;
}
bool TgWrapper::getOutputTensor(int input_node_idx, int tensor_idx) {
    out_tensor = get_graph_output_tensor(mGraph, input_node_idx, tensor_idx);

    if (get_tensor_shape(out_tensor, outDims, 4) <= 0) {
        LOGW("get tensor shape failed, errno: %s", get_tengine_errno());
        return false;
    }
    return true;
}
void TgWrapper::postProcess() {
    LOGW("TgWrapper::postProcess unimplements.");
}
void TgWrapper::postRunGraph() {
    // free output tensor memory
    if(out_tensor != nullptr){
        release_graph_tensor(out_tensor);
        out_tensor = nullptr;
    }
    // Release memory for each node memory of tengine graph
    if (postrun_graph(mGraph) != 0) {
        LOGW("Postrun graph failed, errno: %s", get_tengine_errno());
    }
}