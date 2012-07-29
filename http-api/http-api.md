
ZADATA HTTP API
===============

This API allows CRUD operations on topic values.
The endpoint is: 

http://api.zadata.com/v1/value

Currently only http supported, but soon the default will be https (SSL), i.e.:

https://api.zadata.com/v1/value (NOT IMPLEMENTED YET)

The authentication used is HTTP Basic Auth (i.e. username and password, same as in MQTT).
The username and password is transmitted in clear text, so https must be used when on non-trusted networks.


Common Parameters (optional):
=============================

Common HTTP agruments (all optional):
- `verbose`: 0|1 - boolean (default is 0). If verbose=1, HTTP response body will contain text with error message (in case of error).
- `no_status`: 0|1 - boolean (default is 0) - usefull for Flash clients (will always return HTTP status 200)
- `_method` - used to override HTTP method for environments where only GET and POST supported (i.e. browsers). Only used with POST method, otherwise ignored.
- `cid` - same as MQTT ClientID (default is "default"). Should be string with maximum 23 characters.
- `topic` - URL-encoded topic name (optiopnal, in case topic is not specified as part of URL path).


Common HTTP Status codes:
=========================

```
200 OK - message is in response body
202 Accepted - message is accepted by server
429 Too Many Requests (RFC 6585) - message is throttled by server, need to publish again later
412 Precondition failed - invalid request
413 Request Entity Too Large  -  message is too big (according to parameter)
401 Unauthorized - check your credentials
500 Internal Server Error
```

Examples:
=========

```
# get retained value on topic "a/b/c"
curl --user USER:PWD http://api.zadata.com/v1/value/a/b/c

# set/publish new retain value "Hello, World" to topic "a/b/c"
curl -X PUT --user USER:PWD http://api.zadata.com/v1/value/a/b/c --data-binary "Hello, World"
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c?_method=PUT --data-binary "Hello, World"

# update retain value on topic "a/b/c"
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c --data-binary "Hello, World 2"

# delete retain value on topic "a/b/c" (same as PUT-ing empty message)
curl -X DELETE --user USER:PWD http://api.zadata.com/v1/value/a/b/c
# or
curl -X POST --user USER:PWD http://api.zadata.com/v1/value/a/b/c?_method=DELETE
```

Example of publish / set retained value
=======================================



Example of get retained value
=============================



