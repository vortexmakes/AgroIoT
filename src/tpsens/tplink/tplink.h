/*
 * tplfsm.h
 */

#ifndef __TPLINK_H__
#define __TPLINK_H__

#include "mytypes.h"
#include "tplcfg.h"
#include "tpldefs.h"

/*
 * TPLINK frame struct
 */
typedef struct
{
#if TPLINK_ADDRESSING_ON
    uchar addr;
#endif
    ushort qty;
    uchar pload[TPLINK_MAX_PLOAD_SIZE + 2];
#if ((TPLINK_DEV_TYPE == TPLINK_DEV_MASTER) && (TPLINK_VAR_FRMTOUT))
    ushort toutfrm;
#endif
}TPLFRM_T;

/*
 * TPLINK error codes
 */
enum
{
    TPLINK_OK,
    TPLINK_ERR_BUSY,
    TPLINK_ERR_PLOAD_SIZE_EXCEED
};

#if (TPLINK_VAR_FRMTOUT) && (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
/*
 * tplink_frmtout:
 *  Used to convert timer values in msec to TpLink base time.
 */
#define tplink_frmtout(x)       ((x) / (TPLINK_BASE_TIME))
#endif

/*
 * tplink_init:
 *      Initilizes TPLINK software and hardware
 */
void tplink_init(void);

/*
 * tplink_deinit:
 *      Deinit TPLINK software and hardware
 */
void tplink_deinit(void);

/*
 * tplink_send_frame:
 *      Sends a frame by the TX channel.
 *      After calling to this function, the memory pointed
 *      by p can be released. It is a not blocking function.
 *      Previous to start to transmit, it verifies if there are
 *      any RX or TX in progress.
 *
 *      -args:
 *          p: pointer to frame to be sended.
 *
 *      -returns:
 *          TPLINK_OK: the frame is copied to internal buffer
 *                     and start transmision.
 *          -TPLINK_ERR_BUSY: if there are any RX/TX in progress
 *                            at calling time.
 *
 */
MInt tplink_send_frame(TPLFRM_T *p);

/*
 * tplink_onrcv:
 *      Is defined here but must be implemented by the user.
 *      Is called when a valid frame is received
 *
 *      - args:
 *          p: pointer to received frame. The user must copy
 *          the pointer content before return from this call.
 */
void tplink_onrcv(TPLFRM_T *p);

/*
 * tplink_ontout:
 *      Is defined here but must be implemented by the user.
 *      Is called when an inter byte timout ocurr.
 *      The timout TPLINK_INTER_BYTE_TIME value is defined in tplcfg.h:
 */
void tplink_ontout(void);

/*
 * tplink_onchkerr:
 *      Is defined here but must be implemented by the user.
 *      Is called when an checksum error ocurr.
 */

#if __USE_TPLINK_ONCHERR_CBF__
void tplink_onchkerr(void);
#else
#define tplink_onchkerr();
#endif

/*
 * tplink_onxmit_cmp:
 *      Is defined here but must be implemented by the user.
 *      Is called when frame transmision is complete.
 */

#if __USE_TPLINK_ONXMIT_CMP_CBF__
void tplink_onxmit_cmp(void);
#else
#define tplink_onxmit_cmp();
#endif

#endif
/* ------------------------------ End of file ------------------------------ */
