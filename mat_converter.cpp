//
// Created by axionl on 10/11/20.
//
#include "mat_converter.h"

MatRequest MatConverter::EncodeImg(cv::Mat &img) {
    vector<uchar> encodeData;
    cv::imencode(".jpg", img, encodeData);
    string encodeStr{encodeData.begin(), encodeData.end()};

    MatRequest request;
    request.set_rows(img.rows);
    request.set_cols(img.cols);
    request.set_flag(GetImReadFlag(img));
    request.set_data(encodeStr);

    return request;
}

cv::Mat MatConverter::DecodeImg(MatRequest &request) {
    string decodeStr = request.data();
    vector<uchar> decodeData{decodeStr.begin(), decodeStr.end()};

    cv::Mat img = cv::imdecode(decodeData, request.flag());

    return img;
}

int MatConverter::GetImReadFlag(cv::Mat &img) {
    int flag = cv::IMREAD_COLOR;

    switch (img.channels()) {
        case 1:
            flag = cv::IMREAD_GRAYSCALE;
            break;
        case 3:
            flag = cv::IMREAD_COLOR;
            break;
    }

    return flag;
}