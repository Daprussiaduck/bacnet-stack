/**
 * @file
 * @brief The BACnet MS/TP datalink tasks for handling the device specific
 *  data link network port layer
 * @author Steve Karg <skarg@users.sourceforge.net>
 * @date January 2025
 * @copyright SPDX-License-Identifier: Apache-2.0
 */
#ifndef BACNET_PORT_MSTP_H
#define BACNET_PORT_MSTP_H

#include <stdbool.h>
#include <stdint.h>
/* BACnet Stack defines - first */
#include "bacnet/bacdef.h"
#include "bacnet/datalink/dlmstp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BACNET_STACK_EXPORT
void bacnet_port_mstp_task(uint16_t elapsed_seconds);
BACNET_STACK_EXPORT
bool bacnet_port_mstp_init(void);
BACNET_STACK_EXPORT
bool bacnet_port_mstp_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
