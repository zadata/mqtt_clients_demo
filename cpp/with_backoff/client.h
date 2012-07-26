/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#ifndef MQTTCPP_CLIENT_H
#define MQTTCPP_CLIENT_H
#include <mosquittopp.h>
#include <string>

class Client : public mosquittopp::mosquittopp {
public:
    Client();
    virtual ~Client();

    bool need_force_retry(); // sets force_retry to false and returns old value of force_retry,
                             // used as a flag for stopping the loop
    
private:
    void on_connect(int rc); // connect handler
    void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos); // subscribe handler
    void on_message(const struct mosquitto_message *msg); // message handler
    const char *generate_id(); // generates random id and stores it in id field to be releases during destruction

private:
    char *id;
    bool force_retry;
};

#endif//MQTTCPP_CLIENT_H
 
