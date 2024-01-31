//
// Created by kareem on 12/4/23.
//

#include "mycallback.h"
#include <mqtt/async_client.h>
#include <memory>
#include "Client.h"





Client::Client(const std::string& server_address, const std::string& client_id) :
    cli(server_address, client_id),
    QOS(1),
    proxy_publish_topic(std::make_unique<mqtt::topic>( cli, "rpi/01/actions", QOS, false)),
    connOpts(mqtt::connect_options_builder().clean_session().will(mqtt::message("last", "bye!", 5, 1, true)) .keep_alive_interval(std::chrono::seconds(60)).finalize()),
    proxy_subscribe_topic (std::make_unique<mqtt::topic>(cli, "rpi/01/sensors", QOS, false)){

    cli.set_callback(cb);
}

Client::~Client() {
    disconnect();
}

Client &Client::get_instance(const std::string& IP, const std::string& client_id) {
    static Client instance(IP, client_id);
    return instance;
}


CONNECTION_STATUS Client::connect() {

    try {
        // why wait? wait for the mqtt server to fully connect
        cli.connect(connOpts)->wait();
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

CONNECTION_STATUS Client::proxy_subscribe() {
    try {
        // why wait? wait for the mqtt server to fully subscribe to the topic
        // then return to this function after the full subscription is done
        proxy_subscribe_topic->subscribe()->wait();
        // TODO: print the actual topic name.
        std::cout << "proxy subscribed" << std::endl;
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

CONNECTION_STATUS Client::proxy_publish(const std::string& payload) {
    try {
        proxy_publish_topic->publish(payload)->wait();
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

 CONNECTION_STATUS Client::disconnect() {
    try {
        cli.disconnect()->wait();
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}


//needs removal,should be async with call back only
std::string Client::get_message( std::string &topic)  {
    return cb.get_message(topic);
}



CONNECTION_STATUS Client::start_client() {
    // Start the client, connects to the mosquitto broker
    if (connect() == CONNECTION_STATUS::FAILURE) {
        std::cout << "connection failed" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
    // 
    if (proxy_subscribe() == CONNECTION_STATUS::FAILURE) {
        std::cout << "subscription failed of proxy" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
    // the actual type?
    for (auto&& v2v_topic_pair : v2v_subscribed_topics) {
        // Connects to all other RPIs
        if (v2v_subscribe(v2v_topic_pair.first) == CONNECTION_STATUS::FAILURE) {
            std::cout << "subscription failed of " << v2v_topic_pair.first <<  std::endl;
            return CONNECTION_STATUS::FAILURE;
        }

    }

    return CONNECTION_STATUS::SUCCESS;
}

CONNECTION_STATUS Client::v2v_publish(const std::string& topic_name, const std::string &payload) {

    try {
        v2v_published_topics[topic_name]->publish(payload);
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }

}

CONNECTION_STATUS Client::v2v_subscribe(const std::string& topic_name) {

    try {
        v2v_subscribed_topics[topic_name]->subscribe()->wait();
        std::cout << "v2v "  << topic_name <<  " Subscribed" << std::endl;
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }

}



void Client::add_v2v_subscribed_topic(const std::string& topic_name) {

       std::unique_ptr<mqtt::topic> new_topic =  std::make_unique<mqtt::topic>(cli, topic_name, QOS, true);

       //move is used here because the unique_ptr can't be copied, we only transfer the ownership
       v2v_subscribed_topics.insert(std::make_pair(topic_name, std::move(new_topic)));

}

void Client::add_v2v_published_topic(const std::string& topic_name) {

    std::unique_ptr<mqtt::topic> new_topic =  std::make_unique<mqtt::topic>(cli, topic_name, QOS, true);

    //move is used here because the unique_ptr can't be copied, we only transfer the ownership
    v2v_published_topics.insert(std::make_pair(topic_name, std::move(new_topic)));

}


void Client::remove_v2v_subscribed_topic(const std::string& topic) {

    v2v_subscribed_topics.erase(topic);

}


void Client::remove_v2v_published_topic(const std::string& topic) {

    v2v_published_topics.erase(topic);

}


void Client::set_connOpts(const mqtt::connect_options &connOpts) {

    Client::connOpts = connOpts;
}

const mqtt::connect_options &Client::get_connOpts() const {
    return connOpts;
}

void Client::set_publish_topic(const std::string& topic) {
    proxy_publish_topic = std::make_unique<mqtt::topic>(mqtt::topic(cli, topic, QOS, true));

}

   void Client::set_subscriber_topic(const std::string& topic) {
       proxy_subscribe_topic = std::make_unique<mqtt::topic>(cli, topic, QOS, true);
   }

void Client::set_qos(int QOS) {
    this->QOS = QOS;
}


std::pair<std::string, std::string> Client::get_server_and_clientID(int argc, char* argv[]){
    std::string SERVER_ADDRESS;
    std::string CLIENT_ID;

    switch (argc) {
        case 1:
            SERVER_ADDRESS = "tcp://localhost:1883";
            CLIENT_ID = "rpi_01";
            break;
        case 2:
            SERVER_ADDRESS = argv[1];
            CLIENT_ID = "rpi_01";
            break;
        case 3:
            SERVER_ADDRESS = argv[1];
            CLIENT_ID = argv[2];
            break;
        default:
            std::cout << "too many arguments provided" << std::endl;
            std::cout << "usage: harmony_rpi <server address> <client id>" << std::endl;
            exit(1);
    }
    std::cout << "server address is " << SERVER_ADDRESS << std::endl;
    std::cout << "client id is " << CLIENT_ID << std::endl;
    return {SERVER_ADDRESS, CLIENT_ID};
}

int Client::get_qos() const {
    return QOS;
}

const std::unique_ptr<mqtt::topic> &Client::getPublishTopic() const {
    return proxy_publish_topic;
}

const std::unique_ptr<mqtt::topic> &Client::getSubscribeTopic() const {
    return proxy_subscribe_topic;
}

int Client::new_message_received() const {
    return cb.received_flag;
}
