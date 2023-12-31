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
proxy_publish_topic(std::make_unique<mqtt::topic>( cli, "rpi/01/actions", QOS, true)),
connOpts(mqtt::connect_options_builder().clean_session().will(mqtt::message("last", "bye!", 5, 1, true)) .keep_alive_interval(std::chrono::seconds(60)).finalize()),
proxy_subscribe_topic (std::make_unique<mqtt::topic>(cli, "rpi/01/sensors", QOS, true)){



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
        proxy_subscribe_topic->subscribe()->wait();
        std::cout << "Subscribed proxy" << std::endl;
        return CONNECTION_STATUS::SUCCESS;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
}

CONNECTION_STATUS Client::proxy_publish(const std::string& payload) {
    try {
        proxy_publish_topic->publish(payload);
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
std::string Client::get_message() const {
   std::string payload = cb.payload;
    return payload;
}






CONNECTION_STATUS Client::start_client() {
    if (connect() == CONNECTION_STATUS::FAILURE) {
        std::cout << "connection failed" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
    if (proxy_subscribe() == CONNECTION_STATUS::FAILURE) {
        std::cout << "subscription failed of proxy" << std::endl;
        return CONNECTION_STATUS::FAILURE;
    }
    for (auto&& v2v_topic_pair: v2v_subscribed_topics) {
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
        std::cout << "Subscribed" << std::endl;
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


int Client::get_qos() const {
    return QOS;
}

const std::unique_ptr<mqtt::topic> &Client::getPublishTopic() const {
    return proxy_publish_topic;
}

const std::unique_ptr<mqtt::topic> &Client::getSubscribeTopic() const {
    return proxy_subscribe_topic;
}










