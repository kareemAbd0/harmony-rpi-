#ifndef HARMONY_RPI_CLIENT_H
#define HARMONY_RPI_CLIENT_H

#include <unordered_map>

enum class CONNECTION_STATUS {SUCCESS, FAILURE};

/**
 * @brief Represents an MQTT client with simplified configuration, functionality and default values for the rpi project.
 */
class Client {
public:

    /**
     * @brief Gets the singleton instance of the MQTT client.
     * @return The singleton instance of the MQTT client.
     */
    static Client& get_instance(const std::string& server_address, const std::string& client_id);

    /**
     * @brief Destructor for the MQTT client. Disconnects the client.
     */
    ~Client();

    /**
     * @brief Establishes a connection to the MQTT broker.
     * @return Status of the connection attempt.
     */
    CONNECTION_STATUS connect();

    /**
     * @brief Subscribes to the default MQTT topics.
     * @return Status of the subscription attempt.
     */
    CONNECTION_STATUS proxy_subscribe();

    /**
     * @brief Publishes a message to the default MQTT topic.
     * @param payload The message to be published.
     * @return Status of the publishing attempt.
     */
    CONNECTION_STATUS proxy_publish(const std::string& payload);

    /**
     * @brief Disconnects the client from the MQTT broker.
     * @return Status of the disconnection attempt.
     */
    CONNECTION_STATUS disconnect();

    /**
     * @brief Starts the client by connecting and subscribing.
     * @return Status of the client start attempt.
     */
    CONNECTION_STATUS start_client();

    /**
     * @brief  publish a message to a specific topic
     * @param topic_name The topic to be published to.
     * @param payload The message to be published.
     */

    CONNECTION_STATUS v2v_publish(const std::string& topic_name ,const std::string& payload);


    /**
     * @brief  subscribe to a specific topic
     * @param topic_name The topic to be subscribed to.
     */

     CONNECTION_STATUS v2v_subscribe(const std::string& topic_name);



    /**
     * @brief Adds a topic to the list of subscribed V2V topics.
     * @param topic_name The topic to be added.
     */

    void add_v2v_subscribed_topic(const std::string& topic_name);

    /**
     * @brief Adds a topic to the list of published V2V topics.
     * @param topic_name The topic to be added.
     */

    void add_v2v_published_topic(const std::string& topic_name);

    /**
     * @brief Removes a topic from the list of subscribed V2V topics.
     * @param topic The topic to be removed.
     */
    void remove_v2v_subscribed_topic(const std::string& topic);


    /**
     * @brief Removes a topic from the list of published V2V topics.
     * @param topic The topic to be removed.
     */
    void remove_v2v_published_topic(const std::string& topic);


    /**
     * @brief Gets the latest message from a specific topic.
     * @param topic The topic to get the message from.
     * @return The message from the topic.
     */
    std::string get_message(std::string &topic) ;

    /**
     * @brief Sets the MQTT connection options.
     * @param connOpts The MQTT connection options.
     */
    void set_connOpts(const mqtt::connect_options &connOpts);

    /**
     * @brief Gets the current MQTT connection options.
     * @return The MQTT connection options.
     */
    const mqtt::connect_options &get_connOpts() const;

    /**
     * @brief Sets the MQTT topic for publishing.
     * @param topic The MQTT topic for publishing.
     */
    void set_publish_topic(const std::string& topic);

    /**
     * @brief Sets the MQTT topic for subscribing.
     * @param topic The MQTT topic for subscribing.
     */
    void set_subscriber_topic(const std::string& topic);

    /**
     * @brief Sets the Quality of Service (QoS) level.
     * @param QOS The Quality of Service level.
     */
    void set_qos(int QOS);

    /**
     * @brief Gets the server address and client id from the command line arguments, is is static because it is used before the client is created.
     * @param argc The number of command line arguments.
     * @param argv The command line arguments.
     * @return The server address and client id.
     */
    static std::pair<std::string, std::string> get_server_and_clientID(int argc, char* argv[]);

    /**
     * @brief Gets the current Quality of Service (QoS) level.
     * @return The Quality of Service level.
     */
    int get_qos() const;

    /**
     * @brief Gets the MQTT topic for publishing.
     * @return The MQTT topic for publishing.
     */
    const std::unique_ptr<mqtt::topic> &getPublishTopic() const;

    /**
     * @brief Gets the MQTT topic for subscribing.
     * @return The MQTT topic for subscribing.
     */
    const std::unique_ptr<mqtt::topic> &getSubscribeTopic() const;

    /**
     */
    void wait_new_message();

private:

    /**
     * @brief Constructor for the MQTT client, sets some default values.
     * @param server_address The address of the MQTT server.
     * @param client_id The unique identifier for this client.
     */
    Client(const std::string& server_address, const std::string& client_id);

    mycallback cb; ///< Callback handler for MQTT events.
    mqtt::async_client cli; ///< The underlying Paho MQTT client.
    int QOS; ///< Quality of Service level.

    // V2V topics equals to the number of vehicles(including platoon leader) in
    // the simulation
    std::unordered_map<std::string,std::unique_ptr<mqtt::topic>> 
    v2v_subscribed_topics; ///MQTT subscribed V2V topics.

    // V2v topics equals to the number of vehicles(including platoon leader) in
    // the simulation
    std::unordered_map<std::string,std::unique_ptr<mqtt::topic>> 
    v2v_published_topics; ///MQTT published V2V topics.

    // Proxy topic is only one for each vehicle as this is the actions sent to
    // the simulator through the proxy
    std::unique_ptr<mqtt::topic> 
    proxy_publish_topic; ///< MQTT topic for publishing.

    // Proxy topic is only one for each vehicle as this is the sensor data
    // coming from the simulator through the proxy
    std::unique_ptr<mqtt::topic> 
    proxy_subscribe_topic; ///< MQTT topic for subscribing.

    mqtt::connect_options connOpts; ///< MQTT connection options.
};





#endif //HARMONY_RPI_CLIENT_H
