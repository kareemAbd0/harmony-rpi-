#include <iostream>

#include <boost/property_tree/json_parser.hpp>

#include "mycallback.h"
#include "Client.h"

#include "control.h"


int main(int argc, char* argv[]) {
    // Normal strings, but all caps. Why? no reason.
    std::string SERVER_ADDRESS,CLIENT_ID ;

    std::tie(SERVER_ADDRESS, CLIENT_ID) = Client::get_server_and_clientID(argc, argv);

    Client &my_client = Client::get_instance(SERVER_ADDRESS, CLIENT_ID);

    // my_client.add_v2v_subscribed_topic("rpi/00/position");
    // my_client.add_v2v_subscribed_topic("rpi/03/position");
    // my_client.remove_v2v_subscribed_topic("rpi/00/position");
    // my_client.add_v2v_published_topic("rpi/01/position");

    if (my_client.start_client() != CONNECTION_STATUS::SUCCESS){
        std::cout << "starting client failed\nExiting";
        exit(1);
    }

    std::cout << "Client started successfully.\nEntering While loop\n";

    //for testing purposes, to be removed later.
    double vl = 0.0 ;
    double vf = 0.0 ;
    double xl = 0.0 ;
    double yl = 0.0 ;
    double xf = 0.0 ;
    double yf = 0.0 ;

    std::string control_topic1("rpi/01/sensors");
    std::ostringstream oss;
    std::string local_control_message;
    boost::property_tree::ptree sensors;
    boost::property_tree::ptree acions;

    while (true) {
        my_client.wait_new_message();

        local_control_message = my_client.get_message(control_topic1);

        //control logic here (below is an example)
        std::stringstream buffer(local_control_message);

        boost::property_tree::json_parser::read_json(buffer, sensors);

        vl = sensors.get<double>("vl");
        vf = sensors.get<double>("vf");
        xl = sensors.get<double>("xl");
        yl = sensors.get<double>("yl");
        xf = sensors.get<double>("xf");
        yf = sensors.get<double>("yf");

        acions.put("af", Control::getControl(vl, vf, xl, yl, xf, yf));

        boost::property_tree::write_json(oss, acions);

        std::cout <<"Publishing: "<< oss.str() << std::endl;

        if (my_client.proxy_publish(oss.str()) != CONNECTION_STATUS::SUCCESS) {
            std::cout << "publishing failed" << std::endl;
            exit(1);
        }
    }
}
