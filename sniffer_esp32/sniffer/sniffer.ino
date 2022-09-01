#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "sniffer.h"

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc="CN", .schan = 1, .nchan = 13};

wifi_promiscuous_pkt_t *ppkt;
wifi_ieee80211_packet_t *ipkt;
wifi_ieee80211_mac_hdr_t *hdr;
wifi_header_frame_control_t *frame_ctrl;

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

/** This is the call back function called by the esp_wifi_set_promiscuous_rx_cb. 
    This function is called even of the user is not interacting with the Serial.
    
    Once the user give an input, the while will trap the code until the command
    is finished being processed. 
*/
void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
  if (type != WIFI_PKT_MGMT)
    return;

    ppkt = (wifi_promiscuous_pkt_t *)buff;
    ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
    hdr = (wifi_ieee80211_mac_hdr_t *)&ipkt->hdr;

    frame_ctrl = (wifi_header_frame_control_t *)&hdr->frame_ctrl;

    String serialCommand;
    while (Serial.available() > 0) {
        char serialChar = Serial.read();
        serialCommand += serialChar;

        if (serialChar == '\n') {
            processCommand(serialCommand);
            serialCommand = "";
        }
    }

    delay(100);
}

void setup() {
  Serial.begin(115200);
  wifi_sniffer_init();
}

void loop() {
  wifi_sniffer_set_channel(channel);
  channel = (channel % WIFI_CHANNEL_MAX) + 1;

  delay(100);
}

/** This function process the comand given by the user through the Serial. */
void processCommand(String command)
{
  command.trim();
  command.toUpperCase();

  // TODO: change command to 'get_network_info'
  if (command == "GET_RSSI")
        get_network_info();
}

/** This function returns the RSSI and SSID from the captured packet */
void get_network_info()
{
    if (frame_ctrl->type == WIFI_PKT_MGMT && frame_ctrl->subtype == BEACON)
    {
        const wifi_mgmt_beacon_t *beacon = (wifi_mgmt_beacon_t *) ipkt->payload;
        if (beacon->tag_length > 0)
           Serial.printf("%.*s %d\n", beacon->tag_length, beacon->ssid, ppkt->rx_ctrl.rssi);
    }
}
