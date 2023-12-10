#include <iostream>
#include <csignal>
#include "mycallback.h"
#include "Client.h"




int main() {



    Client myclient (SERVER_ADDRESS, CLIENT_ID);

    myclient.add_v2v_subscribed_topic("rpi/00/position");
    myclient.add_v2v_subscribed_topic("rpi/03/position");
    myclient.add_v2v_published_topic("rpi/01/position");

    if (myclient.start_client() == CONNECTION_STATUS::SUCCESS){
        std::cout << "starting all done successfully" << std::endl;
    }

    else{
        std::cout << "starting failed" << std::endl;
    }

    while (true){

        //control logic here



        if (myclient.proxy_publish("some actions") == CONNECTION_STATUS::SUCCESS) {
            std::cout << "published successfully" << std::endl;
        }
        if (myclient.v2v_publish("rpi/01/position", "some value") == CONNECTION_STATUS::SUCCESS) {
            std::cout << "published successfully" << std::endl;
        }
        else {
            std::cout << "publishing failed" << std::endl;
            break;
        }


        sleep(1);
    }

    return 0;
}
