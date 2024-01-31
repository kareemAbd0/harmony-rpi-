//
// Created by kareem on 12/4/23.
//
#include <map>
#include <mutex>
#include "mycallback.h"


static std::mutex mtx;



void mycallback::connected(const std::string &cause) {
    std::cout << "Connected!: " << cause << std::endl;
}
void mycallback::connection_lost(const std::string &cause) {
        std::cout << "Connection lost: " << cause << std::endl;
        exit(1);
}
void mycallback::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "Message arrived" << std::endl;
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
    std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
    // This is a safety measure to avoid empty messages due to failure in other subsystems.
    // In normal operation, this should never happen.
    if (msg->to_string().empty()) {
        std::cout << "Empty message received!" << std::endl;
        messages[msg->get_topic()] = "'{\"xl\": \"-23.22166392048075\", \"yl\": \"40.485157426243994\", \"xf\": \"-19.073077371416503\", \"yf\": \"65.43151651773508\", \"vl\": \"0.0009547182841672307\", \"vf\": \"0.001628209484989103\"}'";
        std::cout << "new payload: " << messages[msg->get_topic()] << std::endl;
    }
    // lock
    mtx.lock();
    messages[msg->get_topic()] = msg->to_string();
    received_flag = 1;
    // unlock
    mtx.unlock();
    received_messages++;
}
void mycallback::delivery_complete(mqtt::delivery_token_ptr token) {
    if(!token){
        std::cout << "Delivery failed!" << std::endl;
        exit(1);
    }
    std::cout << "Delivery complete.\nToken: " << token->get_message_id() << std::endl;
}

std::string mycallback::get_message(const std::string &topic){
    // check if the topic is in the map
    // searching in all topics, which are V2V and Proxy topics
    if(messages.find(topic) == messages.end()){
        std::cout << "topic not found" << std::endl;
        exit(1);
    }
    // lock
    mtx.lock();
    received_flag = 0;
    std::string payload = messages[topic];
    // unlock
    mtx.unlock();
    return payload;
}



