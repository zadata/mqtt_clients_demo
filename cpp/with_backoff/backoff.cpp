/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */
#include <stdlib.h>
#include <iostream>

#include "backoff.h"
#ifdef __WIN32 // also captures win64
#include <windows.h>
#else
void Sleep(long long ms) {
    long long ns = ms * 1000000LL;
    struct timespec ts = {ns / 1000000000, ns % 1000000000};
    struct timespec rem;
    while (nanosleep(&ts, NULL) == -1)
        rem = ts;
}
#endif

const int Backoff::RESET_IN_SECS = 60;
const int Backoff::LIMIT_IN_SECS = 60;
const int Backoff::INITIAL_IN_SECS = 1;
const int Backoff::MAX_RANDOM_IN_MS = 2500;

Backoff::Backoff() {
    last_fail_time = 0;
    backoff_secs = INITIAL_IN_SECS;
}

Backoff::~Backoff() {}

void Backoff::sleep() {
    unsigned int sleep_ms = rand() % MAX_RANDOM_IN_MS;
    time_t now = time(NULL);
    if(now < last_fail_time ||
       now - last_fail_time > RESET_IN_SECS) {
        backoff_secs = INITIAL_IN_SECS;
    }
    sleep_ms += backoff_secs * 1000;
    std::cerr << "Backoff:sleep, will wait " << sleep_ms << "ms (backoff " << backoff_secs * 1000 << "ms)\n";
    Sleep(sleep_ms);
    backoff_secs *= 2;
    if(backoff_secs > LIMIT_IN_SECS)
        backoff_secs = LIMIT_IN_SECS;
    last_fail_time = time(NULL);
}
