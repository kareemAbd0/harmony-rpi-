#ifndef HARMONY_RPI_MYCALLBACK_H
#define HARMONY_RPI_MYCALLBACK_H
#include <mqtt/async_client.h>
#include <cstdlib>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>

#define EMPTY_JSON_MESSAGE   "{}"
#define MAIN_TOPIC           "rpi/01/sensors"
// TODO: a list with all topics

class mycallback : public virtual mqtt::callback
{
public:

    int received_messages = 0;
    int delivered_messages = 0;
    bool received_flag = false;
    std::mutex mtx;
    std::condition_variable cv;
    std::map <std::string,std::string> messages;

    mycallback(){
        // TODO: initialize all topics with empty messages
        messages[MAIN_TOPIC]  = EMPTY_JSON_MESSAGE;
    }

    std::string get_message(const std::string &topic);
    void wait_new_message(void);
private:

    void connected(const std::string& cause) override ;

    void connection_lost(const std::string& cause) override;

    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;
};


#endif //HARMONY_RPI_MYCALLBACK_H
