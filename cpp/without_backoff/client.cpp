/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#include "client.h"
#include "constants.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <stdlib.h>

Client::Client() : mosquittopp(generate_id()) { }

Client::~Client() { free(id); }

void Client::on_connect(int rc) {
    if (!rc) {
        std::cerr << "Connected successfuly to " << Constants::host << ":" << Constants::port << "\n";
        for (int i = 0; i < Constants::topic_count; ++i) {
            std::cerr << "Subscribing to " << Constants::topics[i] << "\n";
            subscribe(NULL, Constants::topics[i], 0);
            if (rc) {
                std::cerr << "Error: failed to subscribe to " << Constants::topics[i] << ", subscribe returned " << rc << "\n";
                disconnect();
                exit(rc);
            }
        }
        time_t rawtime;
        time(&rawtime);
        std::stringstream strm;
        strm << "Hello world from C++ demo at " << ctime(&rawtime);
        std::string str = strm.str();
        rc = publish(NULL, Constants::publish_topic, str.size(), (const uint8_t*)(str.c_str()), 0, 1);
        if (rc) {
            std::cerr << "Error: publish returned " << rc << ", disconnecting.\n";
            disconnect();
            exit(rc);
        }
    } else {
        switch(rc) {
            case 1:
                std::cerr << "Connection Refused: unacceptable protocol version\n";
                break;
            case 2:
                std::cerr << "Connection Refused: identifier rejected\n";
                break;
            case 3:
                std::cerr << "Connection Refused: broker unavailable\n";
                break;
            case 4:
                std::cerr << "Connection Refused: bad user name or password\n";
                break;
            case 5:
                std::cerr << "Connection Refused: not authorised\n";
                break;
            default:
                std::cerr << "Connection Refused: unknown reason\n";
                break;
        }
    }
}
 
void Client::on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos) {
    std::cerr << "Subscribed successfuly...\n";
}

void Client::on_message(const struct mosquitto_message *msg) {
    std::cout << "Got message to topic " << msg->topic;
    if (msg->payloadlen)
        std::cout << ", payload text is " << msg->payload << "\n";
    else
        std::cout << ", payload is (null)\n";
}

const char *Client::generate_id() {
    std::stringstream strm;
    strm << "clientcpp" << time(NULL);
    id = strdup(strm.str().c_str());
    return id;
}
