#pragma once
#include <string>
#include <map>
#include <memory>
#include <typeindex>
#include "../attribute.h"
#include "../uri.h"

namespace thalhammer {
	namespace grpcbackend {
		namespace http {
			class request {
			protected:
				std::map<std::type_index, std::shared_ptr<attribute>> _attributes;
				std::unique_ptr<uri> _parsed_uri;
			public:
				// Clientinfo
				virtual const std::string& get_client_ip() const = 0;
				virtual uint16_t get_client_port() const = 0;

				// Serverinfo
				virtual const std::string& get_server_ip() const = 0;
				virtual uint16_t get_server_port() const = 0;
				virtual bool is_encrypted() const = 0;

				// Request info
				virtual const std::string& get_method() const = 0;
				virtual const std::string& get_resource() const = 0;
				virtual const std::string& get_protocol() const = 0;
				virtual const std::multimap<std::string, std::string>& get_headers() const = 0;
				virtual std::istream& get_istream() = 0;

				const uri& get_parsed_uri() {
					if (!_parsed_uri) {
						_parsed_uri = std::make_unique<uri>(get_resource());
					}
					return *_parsed_uri;
				}

				bool is_get() const {
					return get_method() == "GET";
				}

				bool is_post() const {
					return get_method() == "POST";
				}

				bool has_header(const std::string& key) const {
					return get_headers().count(key) != 0;
				}

				const std::string& get_header(const std::string& key) const {
					static const std::string empty = "";
					if (has_header(key))
						return get_headers().find(key)->second;
					else return empty;
				}

				template<typename T>
				typename std::enable_if<std::is_base_of<attribute, T>::value, void>::type
					set_attribute(std::shared_ptr<T> attr) {
					_attributes[typeid(T)] = attr;
				}

				template<typename T>
				typename std::enable_if<std::is_base_of<attribute, T>::value, bool>::type
					has_attribute() const {
					return _attributes.count(typeid(T)) != 0;
				}

				template<typename T>
				typename std::enable_if<std::is_base_of<attribute, T>::value, std::shared_ptr<T>>::type
					get_attribute() const {
					if (!has_attribute<T>())
						return nullptr;
					return std::dynamic_pointer_cast<T>(_attributes.at(typeid(T)));
				}

				const std::map<std::type_index, std::shared_ptr<attribute>>& get_attributes() const {
					return _attributes;
				}
			};
		}
	}
}