#include <iostream>


#include <boost/property_tree/json_parser.hpp>

#include "mycallback.h"
#include "Client.h"
#include "control.h"


int main(int argc, char* argv[]) {



    std::string SERVER_ADDRESS,CLIENT_ID ;

    std::tie(SERVER_ADDRESS, CLIENT_ID) = Client::get_server_and_clientID(argc, argv);

    Client &my_client = Client::get_instance(SERVER_ADDRESS, CLIENT_ID);


    //example of how to add and remove v2v topics
    my_client.add_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_subscribed_topic("rpi/03/position");
    my_client.remove_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_published_topic("rpi/01/position");


    if (my_client.start_client() != CONNECTION_STATUS::SUCCESS){
        std::cout << "starting client failed\nExiting";
        exit(1);
    }

    std::cout << "Entering While loop\n";

    double vl = 0.0 ;
    double vf = 0.0 ;
    double xl = 0.0 ;
    double yl = 0.0 ;
    double xf = 0.0 ;
    double yf = 0.0 ;


    while (true) {

        if (my_client.new_message_received()) {

            //below is example of how to get the message from the topic
            // control topic is on mycallback.h
            std::string sensor_topic1 = sensor_topic;
            std::string message = my_client.get_message(sensor_topic1);

            //below is example of how to publish a message to a topic
            std::cout << "Publishing message: " << message << std::endl;
            my_client.proxy_publish(message);




        }
    }
}
