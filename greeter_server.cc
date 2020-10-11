//
// Created by axionl on 10/11/20.
//
#ifndef GRPCTEST_GREETER_SERVER_H
#define GRPCTEST_GREETER_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "mat_converter.h"

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using IPCProto::MatRequest;
using IPCProto::MatReply;
using IPCProto::Greeter;

static int counter = 0;

class GreeterServiceImpl final : public Greeter::Service {
    Status SendImg(ServerContext *context, const MatRequest *request, MatReply *reply) override {
        counter++;

        cout << "Received Image: " << counter << endl;

        try {
            cv::Mat img = MatConverter::DecodeImg((MatRequest &) *request);

            cv::imwrite("../images/server.jpg", img);
        }
        catch (std::exception &err) {
            throw err;
        }
        reply->set_code("OK");
        reply->set_count(counter);

        return Status::OK;
    }
};

void RunServer() {
    const string ip_address = "localhost:50051";
    GreeterServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;
    builder.AddListeningPort(ip_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    builder.SetMaxReceiveMessageSize(10000000);

    unique_ptr<Server> server{builder.BuildAndStart()};

    cout << "Server listening on: " << ip_address << endl;

    server->Wait();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    RunServer();
    return 0;
}
#endif
