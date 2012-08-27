/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * for how to run instructions see howto.md
 *
 * Copyright (C) 2012 ZADATA Ltd. All Rights Reserved.
 */

#include "backoff.h"
#include "client.h"
#include "constants.h"
#include <iostream>
#include <errno.h>

Client *reconnect(Client *client) {
    delete client;
    client = new Client();
    if (client->username_pw_set(Constants::username, Constants::password)) {
        std::cerr << "Error setting username and password.\n";
        exit(1);
    }

    int rc = client->connect(Constants::host, Constants::port, Constants::keepalive_secs, true);
    if (rc) {
        std::cerr << "Unable to connect ";
        if (rc == MOSQ_ERR_ERRNO)
            std::cerr << "errno" << " " << errno << "\n";
        else
            std::cerr << "mosq_err" << " " << rc << "\n";
        return client;
    }

    rc = MOSQ_ERR_SUCCESS;
    while (rc == MOSQ_ERR_SUCCESS && !client->need_force_retry()) {
        rc = client->loop(-1);
    }

    return client;
}

int main() {
    Client *client = NULL;
    Backoff backoff;

    srand(time(NULL));
    Client::lib_init();
    while (1) {
        client = reconnect(client);
        backoff.sleep();
    }

    Client::lib_cleanup();

    return 0;
}
