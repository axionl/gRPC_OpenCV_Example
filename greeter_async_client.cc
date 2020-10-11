//
// Created by axionl on 10/11/20.
//

#ifndef GRPCTEST_GREETER_ASYNC_CLIENT_H
#define GRPCTEST_GREETER_ASYNC_CLIENT_H

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
using grpc::ClientAsyncResponseReader;
using grpc::CompletionQueue;
using grpc::Status;
using IPCProto::MatRequest;
using IPCProto::MatReply;
using IPCProto::Greeter;

class GreeterAsyncClient {
public:
    explicit GreeterAsyncClient(shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    string SendImg(const cv::Mat &img) {
        MatRequest request = MatConverter::EncodeImg(const_cast<cv::Mat &>(img));
        MatReply reply;
        ClientContext context;
        CompletionQueue cq;
        Status status;

        unique_ptr<ClientAsyncResponseReader<MatReply>> rpc(
                stub_->PrepareAsyncSendImg(&context, request, &cq));

        rpc->StartCall();
        rpc->Finish(&reply, &status, (void *) 1);

        void *got_tag;
        bool ok = false;
        GPR_ASSERT(cq.Next(&got_tag, &ok));
        GPR_ASSERT(got_tag == (void *) 1);
        GPR_ASSERT(ok);
        if (status.ok()) {
            return reply.code();
        } else {
            return "RPC failed";
        }
    }

private:
    unique_ptr<Greeter::Stub> stub_;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    const string ip_address = "localhost:50051";
    cv::Mat img = cv::imread("../images/test.jpg", cv::IMREAD_COLOR);

    auto arguments = grpc::ChannelArguments();
    int maxSize = img.cols * img.rows * img.elemSize1();

    arguments.SetMaxSendMessageSize(maxSize);
    arguments.SetMaxReceiveMessageSize(maxSize);

    GreeterAsyncClient greeter(grpc::CreateCustomChannel(
            ip_address, grpc::InsecureChannelCredentials(),
            arguments));

    string reply = greeter.SendImg(img);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}

#endif
