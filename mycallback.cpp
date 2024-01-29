//
// Created by kareem on 12/4/23.
//
#include <map>
#include "mycallback.h"




void mycallback::connected(const std::string &cause) {
    std::cout << "Connected!: " << cause << std::endl;
}
void mycallback::connection_lost(const std::string &cause) {
        std::cout << "Connection lost: " << cause << std::endl;
}
void mycallback::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "Message arrived" << std::endl;
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
   std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
    messages[msg->get_topic()] = msg->to_string();
}
void mycallback::delivery_complete(mqtt::delivery_token_ptr token) {
    std::cout << "Delivery complete for token: "
              << (token ? token->get_message_id() : -1) << std::endl;
}




