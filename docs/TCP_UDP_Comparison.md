# TCP vs UDP Comparison

## 1. Introduction

TCP and UDP are transport-layer protocols used to transfer
data between applications. This project implements both
protocols for communication between a Vehicle ECU and
Dashboard ECU using Linux socket programming.

## 2. TCP Communication

TCP is connection-oriented. A connection is established
before application data is transmitted.

In this project:
- The Dashboard ECU acts as the TCP server.
- The Vehicle ECU acts as the TCP client.
- The server listens on port 5000.
- Vehicle data is transmitted using the send() function.
- The Dashboard receives data using recv().
- TCP provides reliable, ordered byte-stream delivery.

## 3. UDP Communication

UDP is connectionless and sends individual datagrams.

In this project:
- The Dashboard ECU receives UDP datagrams on port 5000.
- The Vehicle ECU transmits data using sendto().
- The Dashboard receives data using recvfrom().
- UDP has lower protocol overhead than TCP but does not
  guarantee delivery or ordering.

## 4. Comparison

| Feature | TCP | UDP |
|---|---|---|
| Connection | Connection-oriented | Connectionless |
| Reliability | Reliable delivery mechanisms | No delivery guarantee |
| Ordering | Ordered byte stream | No ordering guarantee |
| Data format | Byte stream | Datagrams |
| Connection setup | Required | Not required |
| Typical socket type | SOCK_STREAM | SOCK_DGRAM |
| Send function | send() | sendto() |
| Receive function | recv() | recvfrom() |
| Server setup | bind(), listen(), accept() | bind() |
| Project use | Reliable vehicle data transfer | Lightweight data transfer |

## 5. Wireshark Analysis

The UDP capture shows packets sent from 127.0.0.1 to
127.0.0.1 with destination port 5000.

The TCP capture shows an established TCP connection between
client port 37170 and server port 5000. TCP data packets
contain the vehicle speed, RPM, and steering-angle values.
Acknowledgment packets are also visible.

## 6. Conclusion

Both TCP and UDP communication were implemented using Linux
sockets. Wireshark was used to inspect the transmitted
vehicle-monitoring data. The experiment demonstrates the
differences between connection-oriented TCP communication
and connectionless UDP communication.

The tests were performed over the loopback interface.
Therefore, the results demonstrate local socket communication
rather than testing a physical Ethernet network.
