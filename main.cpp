#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>
#include "mycallback.h"
#include "Client.h"


int main(int argc, char* argv[]) {



    //for testing purposes, to be removed later.
    int time_ms_debug = 1;
    std::cout << "enter time in ms to sleep for in loop" << std::endl;
    std::cin >> time_ms_debug;

    std::string SERVER_ADDRESS,CLIENT_ID ;

    //get server address and client id from command line arguments, tie is used to return the two variables at once
    std::tie(SERVER_ADDRESS, CLIENT_ID) = Client::get_server_and_clientID(argc, argv);

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

        //control logic here (below is an example)

        std::string control_topic1("rpi/01/sensors");
        std::string local_control_message = my_client.get_message(control_topic1);
        //convert the string to a float
        float local_control_value = std::stof(local_control_message);
        local_control_value ++;
        local_control_value*=22;

        //control example ends here

        local_control_message = std::to_string(local_control_value);

        if (my_client.proxy_publish(local_control_message) == CONNECTION_STATUS::SUCCESS) {
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



        std::this_thread::sleep_for(std::chrono::milliseconds(time_ms_debug));
    }

}
