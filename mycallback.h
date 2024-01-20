//
// Created by kareem on 12/4/23.
//

#ifndef HARMONY_RPI_MYCALLBACK_H
#define HARMONY_RPI_MYCALLBACK_H
#include <mqtt/async_client.h>



class mycallback : public virtual mqtt::callback
{
public:

    std::string payload;

private:

    void connected(const std::string& cause) override ;

    void connection_lost(const std::string& cause) override;

    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override;


};


#endif //HARMONY_RPI_MYCALLBACK_H
