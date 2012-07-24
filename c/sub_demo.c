/* 
 * MQTT client example in C using libmosquitto (
 * how to compile:
 * - install libmosquitto
 * - compile with: 
 *      gcc sub_demo.c -Wall -pedantic -lmosquitto -o sub_demo
 */
 
/* adapted from mosquitto_sub in libmosquitto: 

Copyright (c) 2009-2012 Roger Light <roger@atchoo.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of mosquitto nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <mosquitto.h>
#include <errno.h>
#include <time.h>

/* To find your MQTT Username and Password
   login into your ZADATA account and click  navbar -> "Settings" -> "Credentials" */
#define USERNAME    "YOUR-MQTT-USER"
#define PASSWORD    "YOUR-MQTT-SUBSCRIBER-PWD"
#define HOST        "mqtt.zadata.com"
#define PORT        1883
#define KEEPALIVE_IN_SECS 30

/* here we subscribe to 2 topics (you may change them to your own topics */
char *TOPICS[] = {"/quotes_sim/data/exchange/NYSE/ticker/SPY/price",
                  "/quotes_sim/data/exchange/NYSE/ticker/HPQ/price"};
#define TOPIC_COUNT (sizeof(TOPICS) / sizeof(char*))

void connect_callback(void *obj, int result) {
    struct mosquitto *mosq = obj;

    int i;
    if (!result) {
        fprintf(stderr, "Connected successfuly to %s:%d\n", HOST, PORT);
        for (i = 0; i < TOPIC_COUNT; ++i) {
            fprintf(stderr, "Subscribing to %s\n", TOPICS[i]);
            mosquitto_subscribe(mosq, NULL, TOPICS[i], 0);
        }
    } else {
        switch(result){
            case 1:
                fprintf(stderr, "Connection Refused: unacceptable protocol version\n");
                break;
            case 2:
                fprintf(stderr, "Connection Refused: identifier rejected\n");
                break;
            case 3:
                fprintf(stderr, "Connection Refused: broker unavailable\n");
                break;
            case 4:
                fprintf(stderr, "Connection Refused: bad user name or password\n");
                break;
            case 5:
                fprintf(stderr, "Connection Refused: not authorised\n");
                break;
            default:
                fprintf(stderr, "Connection Refused: unknown reason\n");
                break;
        }
    }
}

void subscribe_callback(void *obj, uint16_t mid, int qos_count, const uint8_t *granted_qos) {
    fprintf(stderr, "Subscribed successfuly...\n");
}

void message_callback(void *obj, const struct mosquitto_message *message) {
    printf("Got message to topic %s, payload %sis %s\n",
           message->topic,
           message->payloadlen ? "text " : "",
           message->payloadlen ? (char*)message->payload : "empty");
}


int main(int argc, char *argv[]) {
    struct mosquitto *mosq = NULL;
    char id[64];
    int rc;
    mosquitto_lib_init();
    sprintf(id, "subdemo%d", (int)time(NULL));
    mosq = mosquitto_new(id, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: out of memory.\n");
        return 1;
    }
    if (mosquitto_username_pw_set(mosq, USERNAME, PASSWORD)) {
        fprintf(stderr, "Error setting username and password.\n");
        return 1;
    }
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);
    
    rc = mosquitto_connect(mosq, HOST, PORT, KEEPALIVE_IN_SECS, true);
    if (rc) {
        fprintf(stderr, "Unable to connect (%s %d).\n",
                rc == MOSQ_ERR_ERRNO ? "errno" : "mosq_errno",
                rc == MOSQ_ERR_ERRNO ? errno   : rc);
        return rc;
    }

    do {
        rc = mosquitto_loop(mosq, -1);
    } while (rc == MOSQ_ERR_SUCCESS);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return rc;
}

