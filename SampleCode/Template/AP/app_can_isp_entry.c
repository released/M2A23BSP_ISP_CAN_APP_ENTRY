/*_____ I N C L U D E S ____________________________________________________*/
#include <string.h>
#include "NuMicro.h"

#include "app_can_isp_entry.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
static uint8_t s_u8ResetPending = 0U;
static uint32_t s_u32ResetRequestTick = 0U;

static const uint8_t s_au8IspConnectRequest[8] =
{
    0x00U, 0x00U, 0x00U, 0xB1U, 0x00U, 0x00U, 0x00U, 0x00U
};

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/
uint8_t APP_CAN_ISP_ENTRY_Init(CANFD_T *psCanfd, uint32_t u32ReservedFilterIndex)
{
    uint32_t u32FilterCount;

    s_u8ResetPending = 0U;
    s_u32ResetRequestTick = 0U;

    if (psCanfd == NULL)
    {
        return 0U;
    }

    u32FilterCount = (psCanfd->SIDFC & CANFD_SIDFC_LSS_Msk) >> CANFD_SIDFC_LSS_Pos;
    if (u32ReservedFilterIndex >= u32FilterCount)
    {
        return 0U;
    }

    CANFD_SetSIDFltr(psCanfd,
                     u32ReservedFilterIndex,
                     CANFD_RX_FIFO0_STD_MASK(APP_CAN_ISP_ENTRY_REQUEST_ID, 0x7FFU));

    return 1U;
}

void APP_CAN_ISP_ENTRY_OnRx(const CANFD_FD_MSG_T *psRxMsg, uint32_t u32TickMs)
{
    if ((psRxMsg == NULL) || (s_u8ResetPending != 0U))
    {
        return;
    }

    if ((psRxMsg->eIdType != eCANFD_SID) ||
        (psRxMsg->eFrmType != eCANFD_DATA_FRM) ||
        (psRxMsg->bFDFormat != 0U) ||
        (psRxMsg->u32Id != APP_CAN_ISP_ENTRY_REQUEST_ID) ||
        (psRxMsg->u32DLC != sizeof(s_au8IspConnectRequest)))
    {
        return;
    }

    if (memcmp(psRxMsg->au8Data,
               s_au8IspConnectRequest,
               sizeof(s_au8IspConnectRequest)) != 0)
    {
        return;
    }

    s_u32ResetRequestTick = u32TickMs;
    s_u8ResetPending = 1U;
}

void APP_CAN_ISP_ENTRY_Process(uint32_t u32TickMs)
{
    if (s_u8ResetPending == 0U)
    {
        return;
    }

    if ((uint32_t)(u32TickMs - s_u32ResetRequestTick) < APP_CAN_ISP_ENTRY_RESET_DELAY_MS)
    {
        return;
    }

    __disable_irq();
    SYS_UnlockReg();
    SYS_ResetChip();

    while (1)
    {
    }
}
