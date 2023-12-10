//
// Created by kareem on 12/4/23.
//

#ifndef HARMONY_RPI_CLIENT_H
#define HARMONY_RPI_CLIENT_H






const std::string SERVER_ADDRESS{ "tcp://localhost:1883"};
const std::string CLIENT_ID{ "rpi_01"};
enum class CONNECTION_STATUS {SUCCESS, FAILURE};



/**
 * @brief Represents an MQTT client with simplified configuration, functionality and default values for the rpi project.
 */
class client {
public:
    /**
     * @brief Constructor for the MQTT client, sets some default values.
     * @param server_address The address of the MQTT server.
     * @param client_id The unique identifier for this client.
     */
    client(std::string server_address, std::string client_id);

    /**
     * @brief Destructor for the MQTT client. Disconnects the client.
     */
    ~client();

    /**
     * @brief Establishes a connection to the MQTT broker.
     * @return Status of the connection attempt.
     */
    CONNECTION_STATUS connect();

    /**
     * @brief Subscribes to the default MQTT topics.
     * @return Status of the subscription attempt.
     */
    CONNECTION_STATUS subscribe();

    /**
     * @brief Publishes a message to the default MQTT topic.
     * @param payload The message to be published.
     * @return Status of the publishing attempt.
     */
    CONNECTION_STATUS publish(const std::string& payload);

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
     * @brief Retrieves the last received message.
     * @return The last received message.
     */
    std::string get_message() const;

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
    void set_publish_topic(std::string topic);

    /**
     * @brief Sets the MQTT topic for subscribing.
     * @param topic The MQTT topic for subscribing.
     */
    void set_subscriber_topic(std::string topic);

    /**
     * @brief Sets the Quality of Service (QoS) level.
     * @param QOS The Quality of Service level.
     */
    void set_qos(int QOS);

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

private:
    mycallback cb; ///< Callback handler for MQTT events.
    mqtt::async_client cli; ///< The underlying Paho MQTT client.
    int QOS; ///< Quality of Service level.
    std::unique_ptr<mqtt::topic> publish_topic; ///< MQTT topic for publishing.
    std::unique_ptr<mqtt::topic> subscribe_topic; ///< MQTT topic for subscribing.
    mqtt::connect_options connOpts; ///< MQTT connection options.
};





#endif //HARMONY_RPI_CLIENT_H
