# Development of an Ethernet-Based Vehicle Monitoring System Using Linux Socket Programming

## 1. Project Overview

This project demonstrates communication between a Vehicle ECU and a Dashboard ECU using Linux socket programming in C. The Vehicle ECU periodically generates vehicle parameters, and the Dashboard ECU receives and displays the data.

The project implements both TCP and UDP communication to compare their behaviour, reliability, and packet transmission characteristics.

**Note:** The current implementation uses the loopback IP address `127.0.0.1` for local testing. Communication over separate computers or a physical Ethernet network requires additional configuration and testing.

## 2. Objectives

- Implement TCP and UDP communication using Linux sockets.
- Simulate Vehicle ECU and Dashboard ECU applications.
- Transmit vehicle speed, engine RPM, and steering angle.
- Compile the C programs using GCC.
- Capture and analyse network traffic using Wireshark.
- Compare TCP and UDP communication.

## 3. Project Structure

```text
ethernet-vehicle-monitoring/
├── README.md
├── docs/
│   ├── Design_Document.md
│   ├── Design_Document.pdf
│   ├── Validation_Report.md
│   ├── Validation_Report.pdf
│   ├── TCP_UDP_Comparison.md
│   └── TCP_UDP_Comparison.pdf
├── src/
│   ├── vehicle_ecu.c
│   ├── dashboard_ecu.c
│   ├── vehicle_ecu_tcp.c
│   ├── dashboard_ecu_tcp.c
│   └── Makefile
├── screenshots/
└── wireshark/
    ├── tcp_capture.pcapng
    └── udp_capture.pcapng
```

## 4. Technologies Used

- Programming language: C
- Operating system: Ubuntu Linux
- Compiler: GCC
- Networking: Linux sockets, TCP, UDP
- Packet analysis: Wireshark
- Documentation: Markdown, Pandoc, XeLaTeX

## 5. Features

- Vehicle ECU data generation.
- Dashboard ECU data reception and display.
- TCP client-server communication.
- UDP sender-receiver communication.
- Network packet capture using Wireshark.
- Separate source files for TCP and UDP implementations.

## 6. Compilation

Open a terminal and navigate to the source directory:

```bash
cd ~/ethernet-vehicle-monitoring/src
```

Compile all programs:

```bash
make
```

To remove the generated executables:

```bash
make clean
```

## 7. Running the UDP Implementation

Open two Ubuntu terminal windows.

**Terminal 1 — Dashboard ECU receiver:**

```bash
cd ~/ethernet-vehicle-monitoring/src
./dashboard_ecu
```

**Terminal 2 — Vehicle ECU sender:**

```bash
cd ~/ethernet-vehicle-monitoring/src
./vehicle_ecu
```

Observe the vehicle parameters received by the Dashboard ECU.

Stop the programs with `Ctrl+C` when required.

## 8. Running the TCP Implementation

Open two Ubuntu terminal windows.

**Terminal 1 — Dashboard ECU TCP server:**

```bash
cd ~/ethernet-vehicle-monitoring/src
./dashboard_ecu_tcp
```

**Terminal 2 — Vehicle ECU TCP client:**

```bash
cd ~/ethernet-vehicle-monitoring/src
./vehicle_ecu_tcp
```

The Vehicle ECU sends data through a TCP connection, and the Dashboard ECU receives and displays the messages.

Stop the programs with `Ctrl+C` when required.

**Important:** Do not run both Dashboard ECU implementations simultaneously if they use the same port, `5000`.

## 9. Wireshark Packet Analysis

The project includes packet captures in the `wireshark/` directory.

Open Wireshark and select the loopback interface (`lo`) for local testing.

### UDP display filter

```text
udp.port == 5000
```

### TCP display filter

```text
tcp.port == 5000
```

Inspect the source and destination addresses, ports, packet sequence, and application payload where available.

Saved captures:

- `wireshark/udp_capture.pcapng`
- `wireshark/tcp_capture.pcapng`

## 10. Validation

The validation process covers the following scenarios:

- Normal TCP communication.
- Normal UDP communication.
- Behaviour when the Dashboard ECU is not running.
- Continuous vehicle data transmission.
- Invalid destination IP address.
- Comparison of TCP and UDP traffic in Wireshark.

Refer to `docs/Validation_Report.pdf` for the test plan and recorded results. Only mark a scenario as passed after verifying its actual result.

## 11. Documentation

The `docs/` directory contains:

- `Design_Document.pdf` — system architecture and design.
- `Validation_Report.pdf` — validation scenarios and results.
- `TCP_UDP_Comparison.pdf` — comparison of the two transport protocols.

The corresponding Markdown files are also included for editing.

## 12. Limitations and Future Scope

The current implementation uses loopback communication for local testing. Future work can include testing between two computers over Ethernet, configuring network interfaces and IP addresses, improving message validation, adding error handling, and integrating a real vehicle data source.

## 13. Conclusion

This project demonstrates the fundamental implementation of TCP and UDP communication between simulated automotive ECUs using Linux socket programming in C. It also provides a basis for analysing network traffic and understanding the differences between connection-oriented and connectionless transport protocols.
