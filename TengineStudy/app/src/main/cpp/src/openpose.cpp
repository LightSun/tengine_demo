//
// Created by Administrator on 2020/10/14 0014.
//

#include <opencv2/imgcodecs.hpp>
#include "openpose.h"
#include "opencv2/imgproc.hpp"
#include "GraphParam.h"
#include "log.h"
#include "ext.h"

#define getPosePairs(type, op)\
    switch (type){ \
        case OPENPOSE_TYPE_MPI: { \
            const static int POSE_PAIRS[14][2] = {{0,  1}, \
                                                  {1,  2}, \
                                                  {2,  3}, \
                                                  {3,  4}, \
                                                  {1,  5}, \
                                                  {5,  6}, \
                                                  {6,  7}, \
                                                  {1,  14},\
                                                  {14, 8},\
                                                  {8,  9}, \
                                                  {9,  10},\
                                                  {14, 11}, \
                                                  {11, 12}, \
                                                  {12, 13}}; \
            op;\
            break;\
        }\
        case OPENPOSE_TYPE_COCO:{\
            static const int POSE_PAIRS[17][2] = {{1,  2},\
                                                  {1,  5},\
                                                  {2,  3},\
                                                  {3,  4},\
                                                  {5,  6},\
                                                  {6,  7},\
                                                  {1,  8},\
                                                  {8,  9},\
                                                  {9,  10},\
                                                  {1,  11},\
                                                  {11, 12},\
                                                  {12, 13},\
                                                  {1,  0},\
                                                  {0,  14},\
                                                  {14, 16},\
                                                  {0,  15},\
                                                  {15, 17}};\
            op;\
            break;\
        }\
        case OPENPOSE_TYPE_BODY25:{ \
            static const int POSE_PAIRS[24][2] = {{1, 2},   {1, 5},   {2, 3},   {3, 4},   {5, 6},   {6, 7},   {1, 8},   {8, 9},\
                                                  {9, 10},  {10, 11}, {11, 24}, {11, 22}, {22, 23}, {8, 12},  {12, 13}, {13, 14},\
                                                  {14, 21}, {14, 19}, {19, 20}, {1, 0},   {0, 15},  {16, 18}, {0, 16},  {15, 17}};\
            op;\
        }break;\
    }

static void get_input_data_pose(cv::Mat img, float *input_data, int img_h, int img_w) {
    cv::resize(img, img, cv::Size(img_h, img_w));
    img.convertTo(img, CV_32FC3);

    float *img_data = (float *) img.data;
    int hw = img_h * img_w;
    double scalefactor = 1.0 / 255;
    float mean[3] = {0, 0, 0};
    //深度学习要求，数据排列RRR....GGG...BBB... so. make RGBRGB... TO  RRR...GGG...BBB    
    for (int h = 0; h < img_h; h++) {
        for (int w = 0; w < img_w; w++) {
            for (int c = 0; c < 3; c++) {
                input_data[c * hw + h * img_w + w] = scalefactor * (*img_data - mean[c]);
                img_data++;
            }
        }
    }
}
static void
post_process_pose(int openpose_type, cv::Mat img, cv::Mat frameCopy, float threshold, float *outdata, int num, int H,
                  int W) {
    const int nPoints = Openpose::getNPoints(openpose_type);
    getPosePairs(openpose_type, {
        std::vector<cv::Point> points(nPoints);

        int frameWidth = img.rows;
        int frameHeight = img.cols;
        LOGD("KeyPoints Coordinate:");
        for (int n = 0; n < num; n++) {
            cv::Point maxloc;
            int piexlNums = H * W;
            double prob = -1;
            for (int piexl = 0; piexl < piexlNums; ++piexl) {
                if (outdata[piexl] > prob) {
                    prob = outdata[piexl];
                    maxloc.y = (int) piexl / H;
                    maxloc.x = (int) piexl % W;
                }
            }
            cv::Point2f p(-1, -1);
            if (prob > threshold) {
                p = maxloc;
                p.y *= (float) frameWidth / W;
                p.x *= (float) frameHeight / H;

                cv::circle(frameCopy, cv::Point((int) p.x, (int) p.y), 8, cv::Scalar(0, 255, 255), -1);
                cv::putText(frameCopy, cv::format("%d", n), cv::Point((int) p.x, (int) p.y),
                            cv::FONT_HERSHEY_COMPLEX, 1,
                            cv::Scalar(0, 0, 255), 3);
            }
            points[n] = p;
            LOGD("n : p.x, p.y = %d, %d, %d", n, p.x, p.y);
            outdata += piexlNums;
        }

        int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);

        for (int n = 0; n < nPairs; n++) {
            cv::Point2f partA = points[POSE_PAIRS[n][0]];
            cv::Point2f partB = points[POSE_PAIRS[n][1]];

            if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
                continue;

            cv::line(img, partA, partB, cv::Scalar(0, 255, 255), 8);
            cv::circle(img, partA, 8, cv::Scalar(0, 0, 255), -1);
            cv::circle(img, partB, 8, cv::Scalar(0, 0, 255), -1);
        }
    });
}

int Openpose::getNPoints(int type) {
    switch (type) {
        case OPENPOSE_TYPE_MPI:
            return 15;
        case OPENPOSE_TYPE_COCO:
            return 18;
        case OPENPOSE_TYPE_BODY25:
            return 25;
        default:
            LOGW("wrong openpose type = %d", type);
    }
    return 0;
}

void Openpose::setInputBuffer(cv::Mat& frame,const char* uniqueId) {
    if(mInput_data != nullptr){
        this->uniqueId = copyStr(uniqueId);
        this->frame = frame;
        get_input_data_pose(frame, getInputBufferAsFloat(), gp->height, gp->width);
        // set_tensor_buffer(input_tensor, input_data, img_size*4);
        if (set_tensor_buffer(input_tensor, mInput_data, gp->getImageSize() * 4) < 0) {
            LOGW("Set input tensor buffer failed");
            return;
        }
    }
}
void Openpose::setInputBuffer(const char *frameFile, const char *uniqueId) {
    if(mInput_data != nullptr){
        this->uniqueId = copyStr(uniqueId);
        this->frame = cv::imread(frameFile);
        get_input_data_pose(frame, getInputBufferAsFloat(), gp->height, gp->width);
        // set_tensor_buffer(input_tensor, input_data, img_size*4);
        if (set_tensor_buffer(input_tensor, mInput_data, gp->getImageSize() * 4) < 0) {
            LOGW("Set input tensor buffer failed");
            return;
        }
    }
}

void Openpose::postProcess() {
    float *outdata = (float *) get_tensor_buffer(out_tensor);
    int num = getNPoints(gp->stype);
    //46 * 46
    int H = outDims[2];
    int W = outDims[3];
    float show_threshold = 0.1;
    cv::Mat frameCopy = frame.clone();

    post_process_pose(gp->stype, frame, frameCopy, show_threshold, outdata, num, H, W);

    char *outDir = gp->getOutDir();
    const char *result1 = concatStr2(outDir, uniqueId, "_Keypionts.jpg");
    const char *result2 = concatStr2(outDir, uniqueId, "_Skeleton.jpg");

    cv::imwrite(result1, frameCopy);
    cv::imwrite(result2, frame);
    free((void *) result1);
    free((void *) result2);
}
