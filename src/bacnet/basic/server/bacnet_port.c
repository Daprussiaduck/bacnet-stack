/**
 * @file
 * @brief The BACnet/IPv4 datalink tasks for handling the device specific
 *  data link layer
 * @author Steve Karg <skarg@users.sourceforge.net>
 * @date April 2024
 * @copyright SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/* BACnet definitions */
#include "bacnet/bacdef.h"
/* BACnet library API */
#include "bacnet/basic/sys/mstimer.h"
#include "bacnet/datalink/datalink.h"
#include "bacnet/basic/object/netport.h"
#if defined(BACDL_BIP)
#include "bacnet/basic/server/bacnet_port_ipv4.h"
#elif defined(BACDL_BIP6)
#include "bacnet/basic/server/bacnet_port_ipv6.h"
#elif defined(BACDL_MSTP)
#include "bacnet/basic/server/bacnet_port_mstp.h"
#endif
/* me! */
#include "bacnet/basic/server/bacnet_port.h"

/* timer used to renew Foreign Device Registration */
static struct mstimer BACnet_Task_Timer;

static BACNET_PORT_TYPE BACnet_Port_Select = PORT_TYPE_BIP;

/**
 * @brief Periodic tasks for the BACnet datalink layer
 */
void bacnet_port_task(void)
{
    uint32_t elapsed_milliseconds = 0;
    uint32_t elapsed_seconds = 0;

    if (mstimer_expired(&BACnet_Task_Timer)) {
        /* 1 second tasks */
        mstimer_reset(&BACnet_Task_Timer);
        /* presume that the elapsed time is the interval time */
        elapsed_milliseconds = mstimer_interval(&BACnet_Task_Timer);
        elapsed_seconds = elapsed_milliseconds / 1000;
        switch (BACnet_Port_Select){
            #if defined(BACDL_BIP)
                case PORT_TYPE_BIP:
                    bacnet_port_ipv4_task(elapsed_seconds);
                    break;
            #elif defined(BACDL_BIP6)
                case PORT_TYPE_BIP6:
                    bacnet_port_ipv6_task(elapsed_seconds);
                    break;
            #elif defined(BACDL_MSTP)
                case PORT_TYPE_MSTP:
                    bacnet_port_mstp_task(elapsed_seconds);
                    break;
            #else
                /* nothing to do */
                (void)elapsed_seconds;
            #endif
            default:
                // Invalid port setting
        }
    }
}

/**
 * @brief Initialize the datalink network port
 */
bool bacnet_port_init(void){
    bool status = false;
    /* start the 1 second timer for non-critical cyclic tasks */
    mstimer_set(&BACnet_Task_Timer, 1000L);
    switch (BACnet_Port_Select){
        #if defined(BACDL_BIP)
            case PORT_TYPE_BIP:
                status = bacnet_port_ipv4_init();
                break;
        #elif defined(BACDL_BIP6)
            case PORT_TYPE_BIP6:
                status = bacnet_port_ipv6_init();
                break;
        #elif defined(BACDL_MSTP)
            case PORT_TYPE_MSTP:
                status = bacnet_port_mstp_init();
                break;
        #endif
        default:
            // Invalid port type
    }
    return status;
}

bool bacnet_port_deinit(void){
    bool status = false;
    switch (BACnet_Port_Select){
        #if defined(BACDL_BIP)
            case PORT_TYPE_BIP:
                status = bacnet_port_ipv4_deinit();
                break;
        #elif defined(BACDL_BIP6)
            case PORT_TYPE_BIP6:
                status = bacnet_port_ipv6_deinit();
                break;
        #elif defined(BACDL_MSTP)
            case PORT_TYPE_MSTP:
                status = bacnet_port_mstp_deinit();
                break;
        #endif
        default:
            // Invalid port type
    }
    return status;
}

bool bacnet_port_set_port(BACNET_PORT_TYPE portType){
    bool status = false;
    bacnet_port_deinit();
    switch (portType){
        #if defined(BACDL_BIP)
            case PORT_TYPE_BIP:
                BACnet_Port_Select = PORT_TYPE_BIP;
                status = true;
                break;
        #elif defined(BACDL_BIP6)
            case PORT_TYPE_BIP6:
                BACnet_Port_Select = PORT_TYPE_BIP6;
                status = true;
                break;
        #elif defined(BACDL_MSTP)
            case PORT_TYPE_MSTP:
                BACnet_Port_Select = PORT_TYPE_MSTP;
                status = true;
                break;
        #endif
        default:
            // Invalid port type
    }
    if (status){
        bacnet_port_init();
    }
    return status;
}