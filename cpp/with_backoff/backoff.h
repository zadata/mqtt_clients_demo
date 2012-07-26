/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#ifndef MQTTCPP_BACKOFF_H
#define MQTTCPP_BACKOFF_H

#include <time.h>

class Backoff {
public:
    Backoff();
    virtual ~Backoff();
    void sleep(); /* sleep according to backoff values */
private:
    time_t last_fail_time; // last time when backoff function was finished
    unsigned int backoff_secs; // current backoff delay
private:
    static const int RESET_IN_SECS; // after this amount of time passes successfuly between backoffs, backoff is reset
    static const int LIMIT_IN_SECS; // maximal backoff delay
    static const int INITIAL_IN_SECS; // initial backoff delay
    static const int MAX_RANDOM_IN_MS; // added randomized delay is between 0 and this value
};

#endif
