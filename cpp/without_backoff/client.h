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

private:
    void on_connect(int rc);
    void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos);
    void on_message(const struct mosquitto_message *msg);
    const char *generate_id();

private:
    char *id;
};

#endif//MQTTCPP_CLIENT_H
 
