/*
 * Demo MQTT C++ Client using libmosquittopp
 *
 * copyright 2012 Nivertech (Nywhere Tech Ltd)
 */

#include "client.h"
#include "constants.h"
#include <iostream>
#include <errno.h>

int main() {
    int rc;
    Client client;

    client.lib_init();
    if (client.username_pw_set(Constants::username, Constants::password)) {
        std::cerr << "Error setting username and password.\n";
        return 1;
    }
    rc = client.connect(Constants::host, Constants::port, Constants::keepalive_secs, true);
    if (rc) {
        std::cerr << "Unable to connect ";
        if (rc == MOSQ_ERR_ERRNO)
            std::cerr << "errno" << " " << errno << "\n";
        else
            std::cerr << "mosq_err" << " " << rc << "\n";
        return rc;
    }
    do {
        rc = client.loop(-1);
    } while(rc == MOSQ_ERR_SUCCESS);

    client.disconnect();
    client.lib_cleanup();
}
