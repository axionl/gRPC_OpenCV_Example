//
// Created by axionl on 10/11/20.
//

#ifndef GRPCTEST_GREETER_ASYNC_SERVER_H
#define GRPCTEST_GREETER_ASYNC_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "mat_converter.h"

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerCompletionQueue;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using IPCProto::MatRequest;
using IPCProto::MatReply;
using IPCProto::Greeter;

class GreeterAsyncServiceImpl final {
public:
    ~GreeterAsyncServiceImpl() {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void RunServer() {
        const string ip_address = "localhost:50051";

        ServerBuilder builder;
        builder.AddListeningPort(ip_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();

        cout << "Async Server listening on: " << ip_address << endl;

        HandleRpcs();
    }

private:
    std::unique_ptr<ServerCompletionQueue> cq_;
    Greeter::AsyncService service_;
    std::unique_ptr<Server> server_;

    void HandleRpcs() {
        new CallData(&service_, cq_.get());
        void *tag;
        bool ok;
        while (true) {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallData *>(tag)->Proceed();
        }
    }

    class CallData {
    public:
        CallData(Greeter::AsyncService *service, ServerCompletionQueue *cq) :
                service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                service_->RequestSendImg(&ctx_, &request_, &responder_, cq_, cq_, this);
                status_ = PROCESS;
            } else if (status_ == PROCESS) {
                cout << "Processing" << endl;

                new CallData(service_, cq_);

                try {
                    cv::Mat img = MatConverter::DecodeImg(request_);

                    cv::imwrite("../images/server.jpg", img);
                }
                catch (std::exception &err) {
                    throw err;
                }

                reply_.set_code("OK");
                responder_.Finish(reply_, Status::OK, this);
                status_ = FINISH;
            } else {
                GPR_ASSERT(status_ == FINISH);
                delete this;
            }
        }

    private:
        Greeter::AsyncService *service_;
        ServerCompletionQueue *cq_;
        ServerContext ctx_;
        MatRequest request_;
        MatReply reply_;
        ServerAsyncResponseWriter<MatReply> responder_;

        enum CallStatus {
            CREATE,
            PROCESS,
            FINISH
        };

        CallStatus status_{CREATE};
    };
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    GreeterAsyncServiceImpl server;
    server.RunServer();

    return 0;
}

#endif