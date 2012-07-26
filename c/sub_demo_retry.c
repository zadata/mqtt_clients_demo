/* 
 * MQTT client example in C using libmosquitto (
 *
 * for how to run instructions see howto.md
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
#include <string.h>
#include <stdlib.h>

/* To find your MQTT Username and Password
   login into your ZADATA account and click  navbar -> "Settings" -> "Credentials" */
#define USERNAME    "YOUR-MQTT-USER"
#define PASSWORD    "YOUR-MQTT-PUBLISHER-PWD"
#define HOST        "mqtt.zadata.com"
#define PORT        1883
#define KEEPALIVE_IN_SECS 25

// see http://googleappsdeveloper.blogspot.co.il/2011/12/documents-list-api-best-practices.html
#define BACKOFF_RESET_IN_SECS 60      // after this amount of time passes successfuly between backoffs, backoff is reset
#define BACKOFF_LIMIT_IN_SECS 60      // maximal backoff delay
#define BACKOFF_INITIAL_IN_SECS 1     // initial backoff delay 
#define BACKOFF_MAX_RANDOM_IN_MS 2500 // added randomized delay is between 0 and this value

/* here we subscribe to 2 topics (you may change them to your own topics */
#define PUBLISH_TOPIC "hello_topic"
char *TOPICS[] = {"/quotes_sim/data/exchange/NYSE/ticker/SPY/price",
                  "/quotes_sim/data/exchange/NYSE/ticker/HPQ/price",
                  PUBLISH_TOPIC};
#define TOPIC_COUNT (sizeof(TOPICS) / sizeof(char*))

bool force_retry = false;

struct backoff {
    time_t last_fail_time;          // last time when backoff function was finished
    unsigned int backoff_secs;      // current backoff delay
};  

struct backoff init_backoff() {
    struct backoff ret;
    ret.last_fail_time = 0;
    ret.backoff_secs = BACKOFF_INITIAL_IN_SECS;
    return ret;
};

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

void connect_callback(void *obj, int result) {
    struct mosquitto *mosq = obj;

    int i;
    int rc;
    char buf[512];
    time_t rawtime;
    if (!result) {
        fprintf(stderr, "Connected successfuly to %s:%d\n", HOST, PORT);
        for (i = 0; i < TOPIC_COUNT; ++i) {
            fprintf(stderr, "Subscribing to %s\n", TOPICS[i]);
            rc = mosquitto_subscribe(mosq, NULL, TOPICS[i], 0);
            if (rc) {
                fprintf(stderr, "Error: failed to subscribe to %s, subscribe returned %d\n",
                        TOPICS[i], rc);
                mosquitto_disconnect(mosq);
                force_retry = true;
                return;
            }
        }
        time(&rawtime);
        sprintf(buf, "Hello world from C demo at %s", ctime(&rawtime));
        fprintf(stderr, "Publishing to %s - %s\n", PUBLISH_TOPIC, buf);
        rc = mosquitto_publish(mosq, NULL, PUBLISH_TOPIC, strlen(buf), (uint8_t*)buf, 0, 1);
        if (rc) {
            fprintf(stderr, "Error: publish returned %d, disconnecting.\n", rc);
            mosquitto_disconnect(mosq);
            force_retry = true;
            return;
        }
    } else {
        force_retry = true;
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


struct mosquitto *reconnect(char *id, struct mosquitto *mosq) {
    int rc;
    force_retry = false;
    if (mosq)
        mosquitto_destroy(mosq);

    mosq = mosquitto_new(id, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: out of memory.\n");
        exit(1);
    }
    if (mosquitto_username_pw_set(mosq, USERNAME, PASSWORD)) {
        fprintf(stderr, "Error setting username and password.\n");
        exit(1);
    }
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);

    rc = mosquitto_connect(mosq, HOST, PORT, KEEPALIVE_IN_SECS, true);
    if (rc) {
        fprintf(stderr, "Unable to connect (%s %d).\n",
                rc == MOSQ_ERR_ERRNO ? "errno" : "mosq_errno",
                rc == MOSQ_ERR_ERRNO ? errno   : rc);
        return mosq;
    }

    rc = MOSQ_ERR_SUCCESS;
    while (rc == MOSQ_ERR_SUCCESS && !force_retry) {
        rc = mosquitto_loop(mosq, -1);
    };

    return mosq;
}

void backoff(struct backoff *backoff) {
    unsigned int sleep_ms = rand() % BACKOFF_MAX_RANDOM_IN_MS;
    time_t now = time(NULL);
    if(now < backoff->last_fail_time ||
       now - backoff->last_fail_time > BACKOFF_RESET_IN_SECS) {
        backoff->backoff_secs = BACKOFF_INITIAL_IN_SECS;
    }
    sleep_ms += backoff->backoff_secs * 1000;
    fprintf(stderr, "Disconnected, will wait %dms (backoff %dms)\n", sleep_ms, backoff->backoff_secs * 1000);
    Sleep(sleep_ms); 
    backoff->backoff_secs *= 2;
    if (backoff->backoff_secs > BACKOFF_LIMIT_IN_SECS)
        backoff->backoff_secs = BACKOFF_LIMIT_IN_SECS;
    backoff->last_fail_time = time(NULL);
}

int main(int argc, char *argv[]) {
    struct mosquitto *mosq = NULL;
    struct backoff bo = init_backoff();
    char id[64];
    int rc;

    srand(time(NULL));
    
    mosquitto_lib_init();
    sprintf(id, "subdemo%d", (int)time(NULL));
    while (1) {
        mosq = reconnect(id, mosq);
        backoff(&bo);
    }
    mosquitto_lib_cleanup();

    return 0;
}
