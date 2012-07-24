/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#ifndef MQTTCPP_CONSTANTS_H
#define MQTTCPP_CONSTANTS_H

class Constants {
public:
    /* To find your MQTT Username and Password
       login into your ZADATA account and click
       navbar -> "Settings" -> "Credentials" */
    const static char *username;       // MQTT username
    const static char *password;       // MQTT password
    const static char *host;           // host for MQTT server
    const static int   port;           // MQTT port
    const static int   keepalive_secs; // keepalive ping interval in seconds
    const static char *topics[];       // topics to subscribe to
    const static int   topic_count;    // number of topics in topics array
    const static char *publish_topic;  // topic to publish hello message to
};
#endif//MQTTCPP_CONSTANTS_H
