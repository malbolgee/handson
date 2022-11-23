#include <WiFi.h>
#include <stdio.h>
#include <string.h>

#define SSID_MAX_LENGTH 32U
#define BSSID_MAX_LENGTH 22U

const __int8_t EOS = 127;

typedef struct _network_info
{
    __int8_t rssi;
    char ssid[SSID_MAX_LENGTH + 2U];
    char bssid[BSSID_MAX_LENGTH + 2U];
} __attribute__((packed)) network_info_t;

void setup()
{
    Serial.begin(9600);
}

void loop()
{

    delay(50);
    String serial_command;
    while (Serial.available() > 0)
    {

        char serial_char = Serial.read();
        serial_command += serial_char;

        if (serial_char == '\n')
        {
            process_command(serial_command);
            serial_command = "";
        }
    }
}

void process_command(String command)
{
    command.trim();
    command.toUpperCase();

    if (command == "GET_NETWORK_INFO")
        wifi_scan();
}

void wifi_scan()
{

    __uint8_t networks_found = WiFi.scanNetworks();
    if (networks_found > 0)
    {

        network_info_t arr[networks_found];

        for (int32_t i = 0; i < networks_found; ++i)
        {

            network_info_t network_info = {0};

            network_info.rssi = (__int8_t)WiFi.RSSI(i);
            strcpy(network_info.ssid, WiFi.SSID(i).c_str());
            strcpy(network_info.bssid, toString(WiFi.BSSID(i)).c_str());

            arr[i] = network_info;

            // Serial.write((__uint8_t *)&network_info, sizeof(network_info_t));
            // Serial.flush();
        }

        // Serial.write((__uint8_t *)&EOS, sizeof(__uint8_t));
        // Serial.flush();

        Serial.write((__uint8_t *)&networks_found, sizeof(__uint8_t));
        Serial.flush();
        Serial.write((__uint8_t *)arr, sizeof(network_info_t) * networks_found);
        Serial.flush();
        Serial.write((__uint8_t *)&EOS, sizeof(__uint8_t));
        Serial.flush();

        // Serial.printf(serialize(arr, networks_found).c_str());
    }
}

static inline String toString(uint8_t *mac)
{

    if (mac == NULL)
        return "";

    String s = "";
    for (int i = 0; i < 5; ++i)
        s += String(mac[i], HEX) + "::";

    return s + mac[5];
}

static inline String stringfy(network_info_t network_info)
{

    String ret = "";

    ret += String(network_info.rssi);
    ret += ",";
    ret += String(network_info.ssid);
    ret += ",";
    ret += String(network_info.bssid);

    return ret;
}

static inline String serialize(network_info_t *network_info, int size)
{

    String ret = "";
    for (int i = 0; i < size - 1; ++i)
        ret += (stringfy(network_info[i]) + ";");

    return ret += stringfy(network_info[size - 1]);
}
