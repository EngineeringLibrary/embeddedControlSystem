#ifndef WIFI_H
#define WIFI_H

#include "driver/gpio.h"
#include <iostream>
#include <string.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include <lwip/ip.h>
#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/api.h>

#include <sstream>

class Wifi
{
  public:
    Wifi(){}
    ~Wifi(){::vTaskDelete(m_pTask);}
    bool connect(void (*FunctionToCall)(std::string));
    bool write(std::string word){ err = netconn_write(this->newconn,  word.c_str(),strlen(word.c_str()),NETCONN_COPY); return 1;}
    std::string read(){return this->readString;}
    bool disconnect(){return 0;}

    struct netconn *conn, *newconn;   
    err_t err;
    std::string readString;

  private:

    void (*FunctionToCall)(std::string);
    tcpip_adapter_ip_info_t info;
    wifi_init_config_t initConfig;
    wifi_config_t config;
    TaskHandle_t  m_pTask;

    bool start_dhcp_server();
    bool initialise_wifi_in_ap(void);
    void setWifiConfig(void);
    void setDHCPConfig(void);

    void communicationInit();
    void callFunction(const std::string &data);
    static void taskRead(void*);
};
static esp_err_t event_handler(void *ctx, system_event_t *event);
static EventGroupHandle_t wifi_event_group;

#endif
