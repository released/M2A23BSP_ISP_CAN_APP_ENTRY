#ifndef __APP_CAN_ISP_ENTRY_H__
#define __APP_CAN_ISP_ENTRY_H__

/*_____ I N C L U D E S ____________________________________________________*/
#include "NuMicro.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/
#ifndef APP_CAN_ISP_ENTRY_RESET_DELAY_MS
#define APP_CAN_ISP_ENTRY_RESET_DELAY_MS                (4960UL)
#endif

#define APP_CAN_ISP_ENTRY_REQUEST_ID                    (0x487UL)

/*_____ F U N C T I O N S __________________________________________________*/
uint8_t APP_CAN_ISP_ENTRY_Init(CANFD_T *psCanfd, uint32_t u32ReservedFilterIndex);
void APP_CAN_ISP_ENTRY_OnRx(const CANFD_FD_MSG_T *psRxMsg, uint32_t u32TickMs);
void APP_CAN_ISP_ENTRY_Process(uint32_t u32TickMs);

#endif /* __APP_CAN_ISP_ENTRY_H__ */
