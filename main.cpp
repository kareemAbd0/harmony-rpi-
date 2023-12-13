#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>
#include "mycallback.h"
#include "Client.h"




int main() {



    Client &my_client = Client::get_instance(SERVER_ADDRESS, CLIENT_ID);


    my_client.add_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_subscribed_topic("rpi/03/position");
    my_client.remove_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_published_topic("rpi/01/position");

    if (my_client.start_client() == CONNECTION_STATUS::SUCCESS){
        std::cout << "starting all done successfully" << std::endl;
    }

    else{
        std::cout << "starting failed" << std::endl;
    }


    while (true) {

        //control logic here






        if (my_client.proxy_publish("some actions") == CONNECTION_STATUS::SUCCESS) {
            std::cout << "published successfully" << std::endl;
        } else {
            std::cout << "publishing failed" << std::endl;
        }

        if (my_client.v2v_publish("rpi/01/position", "some value") == CONNECTION_STATUS::SUCCESS) {
            std::cout << "published successfully" << std::endl;
        }
        else {
            std::cout << "publishing failed" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}
