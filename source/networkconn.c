#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "cy_secure_sockets.h"
#include "cy_wcm.h"
#include "cy_wcm_error.h"
#include "cy_nw_helper.h"
#include "networksvc.h"

#define MAKE_IP_PARAMETERS(a, b, c, d) ((((uint32_t)d) << 24) | \
                                        (((uint32_t)c) << 16) | \
                                        (((uint32_t)b) << 8) |  \
                                        ((uint32_t)a))

#define SOFTAP_IP_ADDRESS MAKE_IP_PARAMETERS(192, 168, 10, 1)
#define SOFTAP_NETMASK MAKE_IP_PARAMETERS(255, 255, 255, 0)
#define SOFTAP_GATEWAY MAKE_IP_PARAMETERS(192, 168, 10, 1)
#define SOFTAP_RADIO_CHANNEL (1u)

#define SOFTAP_SSID "BASIC06"
#define SOFTAP_PASSWORD "psocrocks"
#define SOFTAP_SECURITY_TYPE CY_WCM_SECURITY_WPA2_AES_PSK

static bool isDone = false ;

static cy_rslt_t softap_start(void)
{
    cy_rslt_t result;
    char ip_addr_str[20];

    /* IP variable for network utility functions */
    cy_nw_ip_address_t nw_ip_addr =
    {
        .version = NW_IP_IPV4
    };

    /* Initialize the Wi-Fi device as a Soft AP. */
    cy_wcm_ap_credentials_t softap_credentials = {SOFTAP_SSID, SOFTAP_PASSWORD,
                                                  SOFTAP_SECURITY_TYPE};
    cy_wcm_ip_setting_t softap_ip_info = {
        .ip_address = {.version = CY_WCM_IP_VER_V4, .ip.v4 = SOFTAP_IP_ADDRESS},
        .gateway = {.version = CY_WCM_IP_VER_V4, .ip.v4 = SOFTAP_GATEWAY},
        .netmask = {.version = CY_WCM_IP_VER_V4, .ip.v4 = SOFTAP_NETMASK}};

    cy_wcm_ap_config_t softap_config = {softap_credentials, SOFTAP_RADIO_CHANNEL,
                                        softap_ip_info,
                                        NULL};

    /* Start the the Wi-Fi device as a Soft AP. */
    result = cy_wcm_start_ap(&softap_config);

    if(result == CY_RSLT_SUCCESS)
    {
        nw_ip_addr.ip.v4 = softap_ip_info.ip_address.ip.v4;
        cy_nw_ntoa(&nw_ip_addr, ip_addr_str);
        printf("\n\nConnect TCP client device to the network: addr: %s\n", ip_addr_str);
        printf("SSID: %s Password:%s\n",SOFTAP_SSID, SOFTAP_PASSWORD);
        nw_ip_addr.ip.v4 = softap_ip_info.ip_address.ip.v4;
    }

    return result;
}

static void connected_callback(cy_wcm_event_t event, cy_wcm_event_data_t *data)
{
    if (event == CY_WCM_EVENT_STA_JOINED_SOFTAP)
    {
        printf("  remote client connected to the WIFI access point\n") ;
    }
}

void network_connect_task(void *param)
{
    cy_rslt_t result;
    cy_wcm_config_t wifi_config = {.interface = CY_WCM_INTERFACE_TYPE_AP};

    isDone = false ;

    /* Initialize Wi-Fi connection manager. */
    result = cy_wcm_init(&wifi_config);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("Wi-Fi Connection Manager initialization failed! Error code: 0x%08x\n", (unsigned int)result);
        CY_ASSERT(0);
    }

    cy_wcm_register_event_callback(connected_callback);

    /* Start the Wi-Fi device as a Soft AP interface. */
    result = softap_start();
    if (result != CY_RSLT_SUCCESS)
    {
        printf("Failed to Start Soft AP! Error code: 0x%08x\n", (unsigned int)result);
        CY_ASSERT(0);
    }

    isDone = true ;
    vTaskDelete(NULL) ;
}

bool network_connect_done()
{
    return isDone ;
}