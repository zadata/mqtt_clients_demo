Author: Zvi Avraham (zvi-AT-zadata-DOT-com)

Copyright (C) 2012 ZADATA Ltd


ZADATA HTTP API
===============

This API allows CRUD operations on topic values.


##Endpoint

The endpoint is: 

http://api.zadata.com/v1/value

Currently only http supported, but soon the default will be https (SSL), i.e.:

https://api.zadata.com/v1/value (NOT IMPLEMENTED YET)


##Authentication

The authentication used is HTTP Basic Auth (i.e. username and password, same as in MQTT).
The username and password is transmitted in clear text, so https must be used when on non-trusted networks.

To find your MQTT Username and Password.
Login into your ZADATA account and click  navbar -> "Settings" -> "Credentials".


##Common Parameters:

Common HTTP agruments (all optional):
- `verbose`: 0|1 - boolean (default is 0). If verbose=1, HTTP response body will contain text with error message (in case of error).
- `no_status`: 0|1 - boolean (default is 0) - usefull for Flash clients (will always return HTTP status 200)
- `_method` - used to override HTTP method for environments where only GET and POST supported (i.e. browsers). Only used with POST method, otherwise ignored.
- `cid` - same as MQTT ClientID (default is "default"). Should be string with maximum 23 characters.
- `topic` - URL-encoded topic name (optiopnal, in case topic is not specified as part of URL path).


##Common HTTP Status codes:

```
200 OK - message is in response body
202 Accepted - message is accepted by server
404 Not Found - in case if there are no retain value for specified topic
429 Too Many Requests (RFC 6585) - message is throttled by server, need to publish again later
412 Precondition failed - invalid request. Use verbose=1 in order to get detailed error
413 Request Entity Too Large  -  message is too big (according to parameter)
401 Unauthorized - invalid username/password
403 Forbidden - username/password are valid, but doesn't provide permission (i.e. using subscribe password for publish or publishing/subscribing to somebody else's topic)
500 Internal Server Error
```


##Examples:


Get retained value on topic "a/b/c":

```
curl --user USER:PWD http://api.zadata.com/v1/value/a/b/c
# or
curl --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc
```

Publish / set new retain value "Hello, World" to topic "a/b/c":

```
curl -X PUT --user USER:PWD http://api.zadata.com/v1/value/a/b/c --data-binary "Hello, World"
# or
curl -X PUT --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc --data-binary "Hello, World"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c?_method=PUT --data-binary "Hello, World"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc&_method=PUT --data-binary "Hello, World"
```

Update retain value on topic "a/b/c":

```
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c --data-binary "Hello, World 2"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc --data-binary "Hello, World 2"
```

Delete retain value on topic "a/b/c" (same as PUT-ing empty message):

```
curl -X DELETE --user USER:PWD http://api.zadata.com/v1/value/a/b/c
# or
curl -X DELETE --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c?_method=DELETE
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value?_method=DELETE&topic=a%2Fb%2Fc
```


##Example of publish / set retained value

PUT /v1/value/SOME/LONG/TOPIC

PUT /v1/value?topic=URL-ENCODED-TOPIC

Published message is in HTTP request body and can be text or binary.

HTTP params:
- `retain`: 0|1 - boolean(default is 1)
- `qos`:  0|1|2 - only QoS=0 is implemented (default is 0)

```
# set/publish new retain value to topic a/b/c
curl -X PUT --user USER:PWD http://api.zadata.com/v1/value/a/b/c?retain=0|1&qos=0|1|2 --data-binary "Hello World"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c?_method=PUT --data-binary "Hello World"

#or

# set/publish new retain value to topic a/b/c
curl -X PUT --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc&retain=0|1&qos=0|1|2 --data-binary "Hello World"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc&_method=PUT --data-binary "Hello World"
```



HTTP Response status codes:

```
202 Accepted - message is accepted by server
429 Too Many Requests (RFC 6585) - message is throttled by server, need to publish again later
412 Precondition failed - invalid request. Use verbose=1 in order to get detailed error
413 Request Entity Too Large  -  message is too big (according to parameter)
401 Unauthorized - invalid username/password
403 Forbidden - username/password are valid, but doesn't provide permission (i.e. using subscribe password for publish or publishing/subscribing to somebody else's topic)
500 Internal Server Error
```


##Example of get retained value

GET /v1/value/SOME/LONG/TOPIC

GET /v1/value?topic=URL-ENCODED-TOPIC


```
curl --user USER:PWD http://api.zadata.com/v1/value/a/b/c
#or
curl --user USER:PWD http://api.zadata.com/v1/value?topic=a%2Fb%2Fc&
```

Return retained message as HTTP body

HTTP Return status:

```
200 OK - return retained message in HTTP response body
404 Not Found - in case if there are no retain value for this topic 
429 Too Many Requests (RFC 6585) - request is throttled by server
412 Precondition failed - invalid request. Use verbose=1 in order to get detailed error
401 Unauthorized - invalid username/password
403 Forbidden - username/password are valid, but doesn't provide permission (i.e. using subscribe password for publish or publishing/subscribing to somebody else's topic)
500 Internal Server Error
```
