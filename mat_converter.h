//
// Created by axionl on 10/11/20.
//

#ifndef GRPCTEST_MAT_CONVERTER_H
#define GRPCTEST_MAT_CONVERTER_H

#include <iostream>
#include <opencv2/opencv.hpp>

#include "IPCProto.grpc.pb.h"

using std::string;
using std::vector;
using IPCProto::MatRequest;
using IPCProto::MatReply;

class MatConverter {
public:
    static MatRequest EncodeImg(cv::Mat &img);

    static cv::Mat DecodeImg(MatRequest &request);

    static int GetImReadFlag(cv::Mat &img);

};


#endif //GRPCTEST_MAT_CONVERTER_H
