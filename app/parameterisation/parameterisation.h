#ifndef PARAMETERISATION_H
#define PARAMETERISATION_H

#define parameterisation_ssid "oled"
#define parameterisation_password "oledoled"
#define parameterisation_server "172.16.90.8"

typedef struct parameterisation_data {
    uint16_t order_id;
    char order_date[32];
    char buyer_name[32];
    char marketplace[32];

    uint16_t serial_number;
    uint8_t mac[6];
    char manufacture_date[32];
    uint8_t hardware_version;
    uint8_t software_version;
} parameterisation_data_t;

#endif