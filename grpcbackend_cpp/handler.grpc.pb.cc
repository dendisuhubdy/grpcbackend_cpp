// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: handler.proto

#include "handler.pb.h"
#include "handler.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace thalhammer {
namespace http {

static const char* Handler_method_names[] = {
  "/thalhammer.http.Handler/Handle",
  "/thalhammer.http.Handler/HandleWebSocket",
};

std::unique_ptr< Handler::Stub> Handler::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< Handler::Stub> stub(new Handler::Stub(channel));
  return stub;
}

Handler::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Handle_(Handler_method_names[0], ::grpc::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_HandleWebSocket_(Handler_method_names[1], ::grpc::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::thalhammer::http::HandleRequest, ::thalhammer::http::HandleResponse>* Handler::Stub::HandleRaw(::grpc::ClientContext* context) {
  return new ::grpc::ClientReaderWriter< ::thalhammer::http::HandleRequest, ::thalhammer::http::HandleResponse>(channel_.get(), rpcmethod_Handle_, context);
}

::grpc::ClientAsyncReaderWriter< ::thalhammer::http::HandleRequest, ::thalhammer::http::HandleResponse>* Handler::Stub::AsyncHandleRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::ClientAsyncReaderWriter< ::thalhammer::http::HandleRequest, ::thalhammer::http::HandleResponse>::Create(channel_.get(), cq, rpcmethod_Handle_, context, tag);
}

::grpc::ClientReaderWriter< ::thalhammer::http::HandleWebSocketRequest, ::thalhammer::http::HandleWebSocketResponse>* Handler::Stub::HandleWebSocketRaw(::grpc::ClientContext* context) {
  return new ::grpc::ClientReaderWriter< ::thalhammer::http::HandleWebSocketRequest, ::thalhammer::http::HandleWebSocketResponse>(channel_.get(), rpcmethod_HandleWebSocket_, context);
}

::grpc::ClientAsyncReaderWriter< ::thalhammer::http::HandleWebSocketRequest, ::thalhammer::http::HandleWebSocketResponse>* Handler::Stub::AsyncHandleWebSocketRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::ClientAsyncReaderWriter< ::thalhammer::http::HandleWebSocketRequest, ::thalhammer::http::HandleWebSocketResponse>::Create(channel_.get(), cq, rpcmethod_HandleWebSocket_, context, tag);
}

Handler::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      Handler_method_names[0],
      ::grpc::RpcMethod::BIDI_STREAMING,
      new ::grpc::BidiStreamingHandler< Handler::Service, ::thalhammer::http::HandleRequest, ::thalhammer::http::HandleResponse>(
          std::mem_fn(&Handler::Service::Handle), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      Handler_method_names[1],
      ::grpc::RpcMethod::BIDI_STREAMING,
      new ::grpc::BidiStreamingHandler< Handler::Service, ::thalhammer::http::HandleWebSocketRequest, ::thalhammer::http::HandleWebSocketResponse>(
          std::mem_fn(&Handler::Service::HandleWebSocket), this)));
}

Handler::Service::~Service() {
}

::grpc::Status Handler::Service::Handle(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::thalhammer::http::HandleResponse, ::thalhammer::http::HandleRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Handler::Service::HandleWebSocket(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::thalhammer::http::HandleWebSocketResponse, ::thalhammer::http::HandleWebSocketRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace thalhammer
}  // namespace http

