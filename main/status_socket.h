#ifndef STATUS_SOCKET_H
#define STATUS_SOCKET_H

#include "types.h"
#include "esp_log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "utils.h"
#include "system_simulation.h"

// Function to set up the TCP socket
void setup_status_socket(void);

// Function to send the sensor readings a data packet
void sendSystemStatusDataPacket(sensor_readings_t sensor_readings, system_params_t system_params);

#endif // STATUS_SOCKET_H
