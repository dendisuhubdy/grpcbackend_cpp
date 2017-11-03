#pragma once
#include <ttl/logger.h>
#include <iostream>
#include "websocket/hub.h"
#include "handler.h"
#include <grpc++/server_builder.h>
#include <grpc++/server.h>
#include <thread>
#include <functional>
#include <atomic>

namespace thalhammer {
	namespace grpcbackend {
		class server
		{
			thalhammer::logger logger;
			http::router router;
			websocket::hub hub;
			handler http_service;

			struct cqcontext {
				std::thread th;
				std::unique_ptr<::grpc::ServerCompletionQueue> cq;
			};
			std::atomic<bool> exit;
			std::vector<cqcontext> cqs;
			grpc::ServerBuilder builder;

			std::unique_ptr<::grpc::Server> mserver;
		public:
			server(std::ostream& logstream = std::cout);
			~server();

			void start_server();
			void shutdown_server();

			::grpc::ServerBuilder& get_builder() { return builder; }

			thalhammer::logger& get_logger() { return logger; }
			http::router& get_router() { return router; }
			websocket::hub& get_wshub() { return hub; }
			handler& get_handler() { return http_service; }
		};
	}
}