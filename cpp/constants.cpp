/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#include "constants.h"

const char *Constants::username       = "YOUR-MQTT-USER";
const char *Constants::password       = "YOUR-MQTT-SUBSCRIBER-PWD";
// see constants.h for info
const char *Constants::host           = "mqtt.zadata.com";
const int   Constants::port           = 1883;
const int   Constants::keepalive_secs = 30;
const char *Constants::topics[] = {"/quotes_sim/data/exchange/NYSE/ticker/SPY/price",
                                   "/quotes_sim/data/exchange/NYSE/ticker/HPQ/price"};
const int   Constants::topic_count = (sizeof(Constants::topics) / sizeof(char*));
