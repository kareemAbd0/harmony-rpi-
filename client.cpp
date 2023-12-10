//
// Created by kareem on 12/4/23.
//

#include "mycallback.h"
#include <mqtt/async_client.h>
#include <memory>
#include "client.h"




client::client(std::string server_address, std::string client_id) : cli(server_address, client_id) {

    //set up the default data
     QOS = 1;
     publish_topic = std::make_unique<mqtt::topic>( cli, "rpi/01/actions", QOS, true);
     subscribe_topic = std::make_unique<mqtt::topic>(cli, "rpi/01/sensors", QOS, true);

     connOpts = mqtt::connect_options_builder()
            .clean_session()
            .will(mqtt::message("last", "bye!", 5, 1, true))
            .keep_alive_interval(std::chrono::seconds(60))  // Set the keep alive interval to 60 seconds
            .finalize();
    //set up the callback
    cli.set_callback(cb);
}

client::~client() {
    disconnect();
}

CONNECTION_STATUS client::connect() {

    try {

        cli.connect(connOpts)->wait();
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

CONNECTION_STATUS client::subscribe() {
    try {
        subscribe_topic->subscribe()->wait();
        std::cout << "Subscribed" << std::endl;
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

CONNECTION_STATUS client::publish(const std::string& payload) {
    try {
        publish_topic->publish(payload);
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

 CONNECTION_STATUS client::disconnect() {
    try {
        cli.disconnect()->wait();
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

std::string client::get_message() const {
   std::string payload = cb.payload;
    return payload;
}

CONNECTION_STATUS client::start_client() {
    if (connect() == CONNECTION_STATUS::FAILURE) {
        std::cout << "connection failed" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
    if (subscribe() == CONNECTION_STATUS::FAILURE) {
        std::cout << "subscription failed" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }

    return CONNECTION_STATUS::SUCCESS;
}


void client::set_connOpts(const mqtt::connect_options &connOpts) {

    client::connOpts = connOpts;
}

const mqtt::connect_options &client::get_connOpts() const {
    return connOpts;
}

void client::set_publish_topic(std::string topic) {
    publish_topic = std::make_unique<mqtt::topic>(mqtt::topic(cli, topic, QOS, true));

}

   void client::set_subscriber_topic(std::string topic) {
       subscribe_topic = std::make_unique<mqtt::topic>(cli, topic, QOS, true);
   }

void client::set_qos(int QOS) {
    this->QOS = QOS;
}


int client::get_qos() const {
    return QOS;
}

const std::unique_ptr<mqtt::topic> &client::getPublishTopic() const {
    return publish_topic;
}

const std::unique_ptr<mqtt::topic> &client::getSubscribeTopic() const {
    return subscribe_topic;
}










