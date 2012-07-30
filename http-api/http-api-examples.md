
Examples (without curl verbose mode):

Tried to publish message "Hello, World" to topic "A/B/C", but throttled (b/c there are many other publishes currently in the system).
The JSON error message is because of our optional flag: verbose=1.
When messages throttled HTTP status code is 503:

```
$ curl -X PUT --user USER:PUB-PWD http://api.zadata.com/v1/value/A/B/C?verbose=1 --data-binary "Hello, World"
{"reasons":["Publish throttled, try again later"]}
```

Throttled again:

```
$ curl -X PUT --user USER:PUB-PWD http://api.zadata.com/v1/value/A/B/C?verbose=1 --data-binary "Hello, World"
{"reasons":["Publish throttled, try again later"]}
```

This time message accepted (HTTP Status 202):

```
$ curl -X PUT --user USER:PUB-PWD http://api.zadata.com/v1/value/A/B/C?verbose=1 --data-binary "Hello, World"
```

Now, let's get the message:

```
$ curl --user USER:SUB-PWD http://api.zadata.com/v1/value/A/B/C
Hello, World
```


Now same as above, but with cURL verbose mode:

```
$ curl -X PUT --user USER:PUB-PWD http://api.zadata.com/v1/value/A/B/C?verbose=1 --data-binary "Hello, World" -v 
* About to connect() to api.zadata.com port 80 (#0)
*   Trying 23.23.106.80... connected
* Connected to api.zadata.com (23.23.106.80) port 80 (#0)
* Server auth using Basic with user 'admin'
> PUT /v1/value/A/B/C?verbose=1 HTTP/1.1
> Authorization: Basic ZAWtaW46cHVibGlzaGVycGFzcw==
> User-Agent: curl/7.19.7 (i486-pc-linux-gnu) libcurl/7.19.7 OpenSSL/0.9.8k zlib/1.2.3.3 libidn/1.15
> Host: api.zadata.com
> Accept: */*
> Content-Length: 12
> Content-Type: application/x-www-form-urlencoded
> 
< HTTP/1.1 503 Service Unavailable
< Access-Control-Allow-Origin: *
< Content-Type: application/json; charset=utf-8
< Date: Mon, 30 Jul 2012 09:16:50 GMT
< Server: PPSS/1.0
< Content-Length: 50
< Connection: keep-alive
< 
* Connection #0 to host api.zadata.com left intact
* Closing connection #0
{"reasons":["Publish throttled, try again later"]}
```


```
$ curl -X PUT --user USER:PUB-PWD http://api.zadata.com/v1/value/A/B/C?verbose=1 --data-binary "Hello, World" -v 
* About to connect() to api.zadata.com port 80 (#0)
*   Trying 23.23.106.80... connected
* Connected to api.zadata.com (23.23.106.80) port 80 (#0)
* Server auth using Basic with user 'admin'
> PUT /v1/value/A/B/C?verbose=1 HTTP/1.1
> Authorization: Basic ZAWtaW46cHVibGlzaGVycGFzcw==
> User-Agent: curl/7.19.7 (i486-pc-linux-gnu) libcurl/7.19.7 OpenSSL/0.9.8k zlib/1.2.3.3 libidn/1.15
> Host: api.zadata.com
> Accept: */*
> Content-Length: 12
> Content-Type: application/x-www-form-urlencoded
> 
< HTTP/1.1 202 Accepted
< Access-Control-Allow-Origin: *
< Content-Type: application/json; charset=utf-8
< Date: Mon, 30 Jul 2012 09:17:02 GMT
< Server: PPSS/1.0
< Content-Length: 0
< Connection: keep-alive
< 
* Connection #0 to host api.zadata.com left intact
* Closing connection #0
```


``` 
$ curl --user USER:SUB-PWD http://api.zadata.com/v1/value/A/B/C -v* About to connect() to api.zadata.com port 80 (#0)
*   Trying 23.23.106.80... connected
* Connected to api.zadata.com (23.23.106.80) port 80 (#0)
* Server auth using Basic with user 'admin'
> GET /v1/value/A/B/C HTTP/1.1
> Authorization: Basic ZAWtaW46c3Vic2NyaWJlcnBhc3M=
> User-Agent: curl/7.19.7 (i486-pc-linux-gnu) libcurl/7.19.7 OpenSSL/0.9.8k zlib/1.2.3.3 libidn/1.15
> Host: api.zadata.com
> Accept: */*
> 
< HTTP/1.1 200 OK
< Content-Type: text/plain; charset=utf-8
< Date: Mon, 30 Jul 2012 09:17:54 GMT
< Server: PPSS/1.0
< Content-Length: 12
< Connection: keep-alive
< 
* Connection #0 to host api.zadata.com left intact
* Closing connection #0
Hello, World
```

