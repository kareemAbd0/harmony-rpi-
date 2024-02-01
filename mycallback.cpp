#include <cstdlib>
#include <map>
#include <mutex>
#include <condition_variable>
#include "mycallback.h"


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
    std::cout << "Message size: " << msg->to_string().size() << std::endl;
    // This is a safety measure to avoid empty messages due to failure in other subsystems.
    // In normal operation, this should never happen.
    if (msg->to_string().empty()) {
        std::cout << "Empty message received!" << std::endl;
        exit(1);
    }
    // lock
    std::lock_guard<std::mutex> lock(mtx);
    messages[msg->get_topic()] = msg->to_string();
    received_flag = true;
    cv.notify_one();
    received_messages++;
}
void mycallback::delivery_complete(mqtt::delivery_token_ptr token) {
    if(!token){
        std::cout << "Delivery failed! Invalid token receieved" << std::endl;
        exit(1);
    }
    std::cout << "Delivery complete. Token: " << token->get_message_id() << std::endl;
}

std::string mycallback::get_message(const std::string &topic){
    // check if the topic is in the map
    // searching in all topics, which are V2V and Proxy topics
    if(messages.find(topic) == messages.end()){
        std::cout << "topic ( "<< topic << " ) was not found.\n Exiting..." << std::endl;
        exit(1);
    }
    // lock
    std::lock_guard<std::mutex> lock(mtx);
    received_flag = false;
    std::string payload = messages[topic];
    return payload;
}

void mycallback::wait_new_message(){
    // lock
    std::unique_lock<std::mutex> lk(mtx);
    // stops waiting when received_flag is true
    // checks for spurious wakeups
    // automatically unlocks the mutex when the thread is blocked
    // automatically reacquires the mutex before the call to wait returns
    // https://en.cppreference.com/w/cpp/thread/condition_variable/wait
    cv.wait(lk, [this]{return received_flag;});
    // unlock
    lk.unlock();
}
