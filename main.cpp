#include <iostream>

#include <boost/property_tree/json_parser.hpp>

#include "config.h"
#include "mycallback.h"
#include "client.h"
#include "control.h"


int main(int argc, char* argv[]) {
    std::string SERVER_ADDRESS,CLIENT_ID ;

    std::tie(SERVER_ADDRESS, CLIENT_ID) = Client::get_server_and_clientID(argc, argv);

    Client &my_client = Client::get_instance(SERVER_ADDRESS, CLIENT_ID);

    if (my_client.start_client() != CONNECTION_STATUS::SUCCESS){
        std::cout << "starting client failed\nExiting";
        exit(1);
    }

    std::cout << "Client started successfully."<< std::endl;

    //for testing purposes, to be removed later.
    double vl = 0.0 ;
    double vf = 0.0 ;
    double xl = 0.0 ;
    double yl = 0.0 ;
    double xf = 0.0 ;
    double yf = 0.0 ;

    std::string sensors_topic1 = sensors_topic;
    std::string local_control_message;

    while (true) {
        std::ostringstream oss;
        boost::property_tree::ptree sensors;
        boost::property_tree::ptree actions;
        std::cout << "Waiting for new message" << std::endl;
        my_client.wait_new_message();

        local_control_message = my_client.get_message(sensors_topic1);

        //control logic here (below is an example)
        std::stringstream buffer(local_control_message);

        boost::property_tree::json_parser::read_json(buffer, sensors);

        vl = sensors.get<double>("vl");
        vf = sensors.get<double>("vf");
        xl = sensors.get<double>("xl");
        yl = sensors.get<double>("yl");
        xf = sensors.get<double>("xf");
        yf = sensors.get<double>("yf");

        actions.put("af", Control::getControl( xl, yl, xf, yf,vl, vf));

        boost::property_tree::write_json(oss, actions);

        std::cout <<"Publishing: "<< oss.str() << std::endl;

        if (my_client.proxy_publish(oss.str()) != CONNECTION_STATUS::SUCCESS) {
            std::cout << "publishing failed" << std::endl;
            exit(1);
        }
    }
}
