CC = gcc
CFLAGS = -Wall -Wextra -O2

all: vehicle_ecu dashboard_ecu vehicle_ecu_tcp dashboard_ecu_tcp

# Existing UDP programs
vehicle_ecu: vehicle_ecu.c
	$(CC) $(CFLAGS) vehicle_ecu.c -o vehicle_ecu

dashboard_ecu: dashboard_ecu.c
	$(CC) $(CFLAGS) dashboard_ecu.c -o dashboard_ecu

# New TCP programs
vehicle_ecu_tcp: vehicle_ecu_tcp.c
	$(CC) $(CFLAGS) vehicle_ecu_tcp.c -o vehicle_ecu_tcp

dashboard_ecu_tcp: dashboard_ecu_tcp.c
	$(CC) $(CFLAGS) dashboard_ecu_tcp.c -o dashboard_ecu_tcp

clean:
	rm -f vehicle_ecu dashboard_ecu vehicle_ecu_tcp dashboard_ecu_tcp

.PHONY: all clean
