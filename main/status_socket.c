#include "status_socket.h"

// Define the static variables
static const char *TAG = "TCP SATUS SOCKET";
struct sockaddr_in dest_addr;
int status_socket;

void send_packet(data_packet_t data_packet) {
    // Convert struct to JSON
    char *payload = data_packet_to_json(data_packet);
    char rx_buffer[128];

    // Send JSON payload
    int err = send(status_socket, payload, strlen(payload), 0);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    }

    int len = recv(status_socket, rx_buffer, sizeof(rx_buffer) - 1, 0);
    // Error occurred during receiving
    if (len < 0) {
        ESP_LOGE(TAG, "recv failed: errno %d", errno);
    }

    // Free the JSON string
    free(payload);
}

void sendSystemStatusDataPacket(sensor_readings_t sensor_readings, system_params_t system_params){
    data_packet_t data_packet;
    data_packet.device_type = ESP;
    data_packet.message_type = SYSTEM_STATUS;
    data_packet.system_settings = system_params;
    data_packet.sensor_readings = sensor_readings;
    send_packet(data_packet);
}

void setup_status_socket(void){
    inet_pton(AF_INET, CONFIG_IPV4_ADDR, &dest_addr.sin_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(CONFIG_STATUS_PORT);

    status_socket =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (status_socket < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return; // Add return statement to prevent further execution
    }
    ESP_LOGI(TAG, "created, connecting to %s:%d", CONFIG_IPV4_ADDR, CONFIG_STATUS_PORT);

    int err = connect(status_socket, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "unable to connect: errno %d", errno);
        close(status_socket); // Add cleanup on failure
        return; // Add return statement to prevent further execution
    }
    ESP_LOGI(TAG, "Successfully connected to server");
}