//
// Created by kareem on 12/4/23.
//

#ifndef HARMONY_RPI_MYCALLBACK_H
#define HARMONY_RPI_MYCALLBACK_H
#include "config.h"
#include <mqtt/async_client.h>

const std::string empty_string = "{}";

// TODO: a list with all topics

class mycallback : public virtual mqtt::callback
{
public:

    int received_messages = 0;
    int delivered_messages = 0;
    int received_flag = 0;
    std::map <std::string,std::string> messages;

    mycallback(){
        // TODO: initialize all topics with empty messages
        messages[sensor_topic] = empty_string;
    }

     std::string get_message(const std::string &topic);
private:

    void connected(const std::string& cause) override ;

    void connection_lost(const std::string& cause) override;

    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;


};


#endif //HARMONY_RPI_MYCALLBACK_H
