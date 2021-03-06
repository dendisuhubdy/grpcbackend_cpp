#include "hub.h"

namespace thalhammer {
	namespace grpcbackend {
		namespace websocket {

			hub::hub(ttl::logger& logger)
				: _logger(logger)
			{
				auto g = std::make_shared<group>();
				g->name = "";
				g->handler = nullptr;
				_groups[""] = g;
			}
			hub::~hub() {
			}

			hub& hub::close_all()
			{
				for(auto& e: _groups) {
					std::unique_lock<std::mutex> lck(e.second->cons_lck);
					for(auto& c : e.second->cons)
						c->close();
					e.second->cons.clear();
				}
				return *this;
			}

			void hub::on_connect(connection_ptr con)
			{
				_logger(ttl::loglevel::TRACE, "wshub") << "on_connect: 0x" << std::hex << con.get();
				auto g = _groups.at("");
				auto hdl = std::atomic_load(&g->handler);
				auto attr = std::make_shared<group_attribute>();
				con->set_attribute(attr);
				attr->group = g;
				{
					std::unique_lock<std::mutex> lck(g->cons_lck);
					g->cons.insert(con);
				}
				if(hdl)
					hdl->on_connect(con);
			}

			void hub::on_message(connection_ptr con, bool bin, const std::string& msg)
			{
				_logger(ttl::loglevel::TRACE, "wshub") << "on_message: 0x" << std::hex << con.get();
				auto attr = con->get_attribute<group_attribute>();
				auto hdl = std::atomic_load(&_groups.at("")->handler);
				if(attr) {
					auto group = attr->group.lock();
					if(group) {
						hdl = group->handler;
					}
				}
				if(hdl)
					hdl->on_message(con, bin, msg);
			}

			void hub::on_disconnect(connection_ptr con)
			{
				_logger(ttl::loglevel::TRACE, "wshub") << "on_disconnect: 0x" << std::hex << con.get();
				auto attr = con->get_attribute<group_attribute>();
				auto hdl = std::atomic_load(&_groups.at("")->handler);
				if(attr) {
					auto group = attr->group.lock();
					if(group) {
						if(group->handler) group->handler->on_disconnect(con);
						else if(hdl) hdl->on_disconnect(con);
						{
							std::unique_lock<std::mutex> lck(group->cons_lck);
							group->cons.erase(con);
						}
						attr->group.reset();
						return;
					}
				}
				if(hdl)
					hdl->on_disconnect(con);
			}

			hub& hub::set_default_handler(std::shared_ptr<con_handler> handler)
			{
				std::atomic_store(&_groups.at("")->handler, handler);
				return *this;
			}

			hub& hub::set_connection_group(const connection_ptr& con, const std::string& gname)
			{
				if(_groups.count(gname)==0) {
					_logger(ttl::loglevel::WARN, "wshub") << "Tried to set not existing group " << gname << " on connection " << std::hex << con.get();
					return *this;
				}
				auto attr = con->get_attribute<group_attribute>();
				if(!attr) {
					attr = std::make_shared<group_attribute>();
					con->set_attribute(attr);
				}
				auto g = attr->group.lock();
				if(g) {
					std::unique_lock<std::mutex> lck(g->cons_lck);
					g->cons.erase(con);
				}
				g = _groups.at(gname);
				attr->group = g;
				{
					std::unique_lock<std::mutex> lck(g->cons_lck);
					g->cons.insert(con);
				}
				return *this;
			}

			std::string hub::get_connection_group(const connection_ptr& con)
			{
				auto attr = con->get_attribute<group_attribute>();
				if(attr) {
					auto g = attr->group.lock();
					if(g)
						return g->name;
				}
				return "";
			}

			hub& hub::add_group(const std::string& name, std::shared_ptr<con_handler> handler)
			{
				if(!handler) {
					handler = std::atomic_load(&_groups.at("")->handler);
				}
				if(_groups.count(name)==0) {
					auto g = std::make_shared<group>();
					g->name = name;
					g->handler = handler;
					_groups[name] = g;
				}
				return *this;
			}

			hub& hub::broadcast(const std::string& gname, bool bin, const std::string& msg)
			{
				if(_groups.count(gname)!=0) {
					auto g = _groups[gname];
					if(g) {
						std::unique_lock<std::mutex> lck(g->cons_lck);
						for(auto& e : g->cons) {
							e->send_message(bin, msg);
						}
					}
				}
				return *this;
			}

			hub& hub::broadcast(const connection_ptr& con, bool bin, const std::string& msg)
			{
				auto attr = con->get_attribute<group_attribute>();
				if(attr) {
					auto g = attr->group.lock();
					if(g){
						std::unique_lock<std::mutex> lck(g->cons_lck);
						for(auto& e : g->cons) {
							e->send_message(bin, msg);
						}
					}
				}
				return *this;
			}

			hub& hub::close_group(const std::string& gname)
			{
				for(auto& e : get_group_connections(gname))
					e->close();
				return *this;
			}

			std::set<connection_ptr> hub::get_group_connections(const std::string& gname)
			{
				std::set<connection_ptr> cons;
				if(_groups.count(gname)!=0) {
					auto g = _groups.at(gname);
					if(g) {
						std::unique_lock<std::mutex> lck(g->cons_lck);
						cons = g->cons;
					}
				}
				return cons;
			}

			std::set<connection_ptr> hub::get_group_connections(connection_ptr con)
			{
				std::set<connection_ptr> cons;
				auto attr = con->get_attribute<group_attribute>();
				if(attr) {
					auto g = attr->group.lock();
					if(g) {
						std::unique_lock<std::mutex> lck(g->cons_lck);
						cons = g->cons;
					}
				}
				return cons;
			}
		}
	}
}