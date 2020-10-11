//
// Created by axionl on 10/11/20.
//

#ifndef GRPCTEST_GREETER_CLIENT_H
#define GRPCTEST_GREETER_CLIENT_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "mat_converter.h"

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using IPCProto::MatRequest;
using IPCProto::MatReply;
using IPCProto::Greeter;

class GreeterClient {
public:
    GreeterClient(shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    string SendImg(const cv::Mat &img) {
        MatRequest request = MatConverter::EncodeImg(const_cast<cv::Mat &>(img));

        MatReply reply;

        ClientContext context;

        Status status = stub_->SendImg(&context, request, &reply);

        if (status.ok()) {
            cout << reply.count() << " " << reply.code() << endl;
            return "OK";
        } else {
            cout << status.error_message() << endl;
            return "Failed";
        }
    }

private:
    unique_ptr<Greeter::Stub> stub_;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    const string ip_address = "localhost:50051";
    cv::Mat img = cv::imread("../images/test.jpg", cv::IMREAD_COLOR);

    auto arguments = grpc::ChannelArguments();
//    int maxSize = img.cols * img.rows * img.elemSize1();

    arguments.SetMaxSendMessageSize(10000000);

    GreeterClient greeter(grpc::CreateCustomChannel(
            ip_address, grpc::InsecureChannelCredentials(),
            arguments));


    greeter.SendImg(img);
}

#endif