#include <iostream>
#include <csignal>
#include "mycallback.h"
#include "client.h"




int main() {



    client myclient (SERVER_ADDRESS,CLIENT_ID);


    if (myclient.start_client() == CONNECTION_STATUS::SUCCESS){
        std::cout << "starting all done successfully" << std::endl;
    }

    else{
        std::cout << "starting failed" << std::endl;
    }

    while (1){

        //control logic here



        if ( myclient.publish("kareem has done classes") == CONNECTION_STATUS::SUCCESS) {
            std::cout << "published successfully" << std::endl;
        }
        else {
            std::cout << "publishing failed" << std::endl;
        }

        std::cout << "arrived message is: " << myclient.get_message() << std::endl;

        //sleep(1);
    }

    return 0;
}
