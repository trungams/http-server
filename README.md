# Simple HTTP/1.1 server

Implementation of a simple HTTP server in C++

## Features

- Can handle multiple concurrent connections, tested up to 10k.
- Support basic HTTP request and response. Provide an extensible framework to implement other HTTP features.
- HTTP/1.1: Persistent connection is enabled by default.

## Quick start

```bash
mkdir build && cd build
cmake ..
make
./test_SimpleHttpServer # Run unit tests
./SimpleHttpServer      # Start the HTTP server on port 8080
```

- There are two endpoints available at `/` and `/hello.html` which are created for demo purpose.
- In order to have multiple concurrent connections, make sure to raise the resource limit (with `ulimit`) before running the server. A non-root user by default can have about 1000 file descriptors opened, which corresponds to 1000 active clients.

## Design

The server program consists of:

- 1 main thread for user interaction.
- 1 listener thread to accept incoming clients.
- 5 worker threads to process HTTP requests and sends response back to client.
- Utility functions to parse and manipulate HTTP requests and repsonses conveniently.

## Benchmark

I used a tool called [wrk](https://github.com/wg/wrk) to benchmark this HTTP server. The tests were performed on my laptop with the following specs:

```bash
Model: Thinkpad T480
OS: Ubuntu 18.04 TLS x84_64
Kernel: 4.18.0-24-generic
CPU: Intel i7-8550 (8) @ 4.000 GHz
GPU: Intel UHD Graphics 620
Memory: 6010 MiB / 15803 MiB
```

Here are the results for two test runs. Each test ran for 1 minute, with 10 client threads. The first test had only 500 concurrent connections, while the second test had 10000.

```bash
$ ./wrk -t10 -c500 -d60s http://0.0.0.0:8080/
Running 1m test @ http://0.0.0.0:8080/
  10 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     5.01ms    1.31ms  57.86ms   86.35%
    Req/Sec     9.94k     0.99k   36.28k    76.69%
  5933266 requests in 1.00m, 441.36MB read
Requests/sec:  98760.82
Transfer/sec:      7.35MB
```

```bash
$ ./wrk -t10 -c10000 -d60s http://0.0.0.0:8080/
Running 1m test @ http://0.0.0.0:8080/
  10 threads and 10000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   111.78ms   21.38ms 403.80ms   76.79%
    Req/Sec     8.73k     1.42k   18.77k    75.62%
  5174508 requests in 1.00m, 384.91MB read
Requests/sec:  86123.84
Transfer/sec:      6.41MB

```

