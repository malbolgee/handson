#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "sniffer.h"

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc = "CN", .schan = 1, .nchan = 13};

wifi_promiscuous_pkt_t *promiscuous_packet;
packet_t *packet;
mac_hdr_t *hdr;
hdr_frame_control_t *frame_ctrl;

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void wifi_sniffer_init(void)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_country));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void wifi_sniffer_set_channel(uint8_t channel)
{
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

/**
 * This is the callback function called by the esp_wifi_set_promiscuous_rx_cb.
 * This function is called even of the user is not interacting with the Serial.
 *
 * Once the user give an input, the while will trap the code until the command
 * is finished being processed.
 */
void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type)
{

    String serialCommand;
    while (Serial.available() > 0)
    {
        char serialChar = Serial.read();
        serialCommand += serialChar;

        if (serialChar == '\n')
        {
            processCommand(serialCommand, buff);
            serialCommand = "";
        }
    }
}

void setup()
{
    Serial.begin(9600);
    wifi_sniffer_init();
}

void loop()
{
    wifi_sniffer_set_channel(channel);
    channel = (channel % WIFI_CHANNEL_MAX) + 1; // change channel every 500 miliseconds

    delay(500);
}

/** This function process the comand given by the user through the Serial. */
void processCommand(String command, void *buff)
{
    command.trim();
    command.toUpperCase();

    if (command == "GET_NETWORK_INFO")
        get_network_info(buff);
}

/**
 * This function parses a packet and writes a string to the Serial interface
 * containing the SSID and RSSI of the captured packet.
 *
 * Test if the packet captured is of subtype #BEACON and of type #WIFI_PKT_MGMT.
 * If it is, try to filter out the ones that have no SSID.
 *
 * Returns nothing.
 *
 * Writes to the Serial a string containing the SSID and RSSI of
 * the captured packet.
 *
 * If the packet does not have the type and subtype required, write to the serial
 * a default string containing a network named UNKNOWN and 0 as the RSSI value.
 */
void get_network_info(void *buff)
{

    promiscuous_packet = (wifi_promiscuous_pkt_t *)buff;
    packet = (packet_t *)promiscuous_packet->payload;
    hdr = (mac_hdr_t *)&packet->hdr;

    frame_ctrl = (hdr_frame_control_t *)&hdr->frame_ctrl;

    if (frame_ctrl->type == WIFI_PKT_MGMT && frame_ctrl->subtype == BEACON)
    {
        const mgmt_beacon_t *beacon = (mgmt_beacon_t *)packet->payload;
        if (beacon->tag_length > 0)
            Serial.printf("RES GET_NETWORK_INFO %.*s %d\n", beacon->tag_length, beacon->ssid, promiscuous_packet->rx_ctrl.rssi);
    }
    else
        Serial.printf("RES GET_NETWORK_INFO UNKNOWN 0\n");
}
