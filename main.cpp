#include <iostream>

// used for sleeping in only 1 line
// #include <chrono>
// #include <thread>

#include <boost/property_tree/json_parser.hpp>

#include "mycallback.h"
#include "Client.h"

#include "control.h"


int main(int argc, char* argv[]) {



    // What is this? Takes the value for sleeping in the last.
    // Why though?
    // TODO: get the value from the command line arguments
    //for testing purposes, to be removed later.
    int time_ms_debug = 1;
    std::cout << "enter time in ms to sleep for in loop" << std::endl;
    std::cin >> time_ms_debug;

    // Normal strings, but all caps. Why? no reason.
    std::string SERVER_ADDRESS,CLIENT_ID ;

    //get server address and client id from command line arguments, tie is used
    //to return the two variables at once
    // What does this line do? to get 2 values from a function, 
    // you can use std::pair but C++17
    std::tie(SERVER_ADDRESS, CLIENT_ID) = Client::get_server_and_clientID(argc, argv);

    Client &my_client = Client::get_instance(SERVER_ADDRESS, CLIENT_ID);


    my_client.add_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_subscribed_topic("rpi/03/position");
    my_client.remove_v2v_subscribed_topic("rpi/00/position");
    my_client.add_v2v_published_topic("rpi/01/position");

    if (my_client.start_client() != CONNECTION_STATUS::SUCCESS){
        std::cout << "starting client failed\nExiting";
        exit(1);
    }

    std::cout << "Entering While loop\n";

    //for testing purposes, to be removed later.
    double vl = 0.0 ;
    double vf = 0.0 ;
    double xl = 0.0 ;
    double yl = 0.0 ;
    double xf = 0.0 ;
    double yf = 0.0 ;


    while (true) {

        if (my_client.new_message_received()) {

            std::string control_topic1("rpi/01/sensors");
            std::string local_control_message = my_client.get_message(control_topic1);

            //control logic here (below is an example)
            std::stringstream buffer(local_control_message);
            std::ostringstream oss;

            boost::property_tree::ptree sensors;
            boost::property_tree::ptree acions;
            boost::property_tree::json_parser::read_json(buffer, sensors);

            vl = sensors.get<double>("vl");
            vf = sensors.get<double>("vf");
            xl = sensors.get<double>("xl");
            yl = sensors.get<double>("yl");
            xf = sensors.get<double>("xf");
            yf = sensors.get<double>("yf");

            acions.put("af", Control::getControl(vl, vf, xl, yl, xf, yf));
            boost::property_tree::write_json(oss, acions);
            std::cout << oss.str() << std::endl;
            if (my_client.proxy_publish(oss.str()) == CONNECTION_STATUS::SUCCESS) {
                std::cout << "published successfully" << std::endl;
            } else {
                std::cout << "publishing failed" << std::endl;
            }
            // What does this do? This uses 2 libs.
            // std::this_thread::sleep_for(std::chrono::milliseconds(time_ms_debug));
        }
    }
}
