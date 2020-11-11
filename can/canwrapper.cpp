/*
 * canwrapper.cpp
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#include <iodefine.h>
#include "canwrapper.h"

#define DEFAULT_TIMEOUT 0x20000

CANWrapper::Result::Type CANWrapper::wake(unsigned int timeout)
{
    int timeoutCounter = timeout;

    CAN0.CTLR.BIT.SLPM = 0;

    while ((CAN0.STR.BIT.SLPST) && (--timeoutCounter != 0))
        ;

    if (timeoutCounter == 0)
        return CANWrapper::Result::TIMEOUT;

    return CANWrapper::Result::OK;
}

CANWrapper::Result::Type CANWrapper::sleep(unsigned int timeout)
{
    int timeoutCounter = timeout;

    CANWrapper::Result::Type haltResult = switchMode(CANWrapper::Mode::HALT, timeout);
    if (haltResult != CANWrapper::Result::OK)
        return haltResult;

    CAN0.CTLR.BIT.SLPM = 1;

    while ((!CAN0.STR.BIT.SLPST) && (--timeoutCounter != 0))
        ;

    if (timeoutCounter == 0)
        return CANWrapper::Result::TIMEOUT;

    return CANWrapper::Result::OK;
}

CANWrapper::Result::Type CANWrapper::switchMode(CANWrapper::Mode::Type mode, unsigned int timeout)
{
    int timeoutCounter = timeout;

    switch (mode)
    {
    case CANWrapper::Mode::OPERATION:
        CAN0.CTLR.BIT.CANM = static_cast<int>(mode);

        while ((CAN0.STR.BIT.RSTST || CAN0.STR.BIT.HLTST) && (--timeoutCounter != 0))
            ;

        if (timeoutCounter == 0)
            return CANWrapper::Result::TIMEOUT;

        break;

    case CANWrapper::Mode::HALT:
        CAN0.CTLR.BIT.CANM = static_cast<int>(mode);

        while ((!CAN0.STR.BIT.HLTST) && (--timeoutCounter != 0))
            ;

        if (timeoutCounter == 0)
            return CANWrapper::Result::TIMEOUT;

        break;

    case CANWrapper::Mode::RESET:
        CAN0.CTLR.BIT.CANM = static_cast<int>(mode);

        while ((!CAN0.STR.BIT.RSTST) && (--timeoutCounter != 0))
            ;

        if (timeoutCounter == 0)
            return CANWrapper::Result::TIMEOUT;

        break;

    default:
        return CANWrapper::Result::INVALID;
    }

    return CANWrapper::Result::OK;
}

void CANWrapper::setup()
{
    IOPORT.PFJCAN.BIT.CANE = 1;
    IOPORT.PFJCAN.BIT.CANS = 0;
    PORTB.DR.BIT.B5 = 1;
    PORTB.ICR.BIT.B6 = 1;

    CANWrapper::switchMode(CANWrapper::Mode::RESET, DEFAULT_TIMEOUT);

    CAN0.CTLR.BIT.MBM = 0;
    CAN0.CTLR.BIT.IDFM = 2; // ミックスIDモード
    CAN0.CTLR.BIT.MLM = 0;
    CAN0.CTLR.BIT.TPM = 0;
    CAN0.CTLR.BIT.TSRC = 0;
    CAN0.CTLR.BIT.TSPS = 3;
    CAN0.CTLR.BIT.BOM = 0;
    CAN0.CTLR.BIT.RBOC = 0;

    CAN0.MIER = 0x00000000;
    CAN0.EIER.BYTE = 0x00;

    for (int i = 0; i < 32; i++)
    {
        CAN0.MKR[i].LONG = 0x00000000;
        CAN0.MKIVLR = 0x00000000;
    }

    // n = 4, n_Tq = 25 @ PCLK = 25 MHz => 250kbps
    CAN0.BCR.BIT.BRP = 3;    // n = 4
    CAN0.BCR.BIT.SJW = 0;    // 再同期ジャンプ幅 = 1Tq
    CAN0.BCR.BIT.TSEG1 = 15; // TSEG1 = (15 + 1) Tq
    CAN0.BCR.BIT.TSEG2 = 7;  // TSEG2 = (7 + 1) Tq

    CANWrapper::switchMode(CANWrapper::Mode::OPERATION, DEFAULT_TIMEOUT);
}

void CANWrapper::loadFrame(unsigned int mailbox, CANWrapper::CANFrame frame)
{
    if (31 < mailbox)
        return;

    do {
        CAN0.MCTL[mailbox].BYTE = 0x00;
    } while (CAN0.MCTL[mailbox].BYTE != 0x00);

    for (int i = 0; i < 8; i++)
        CAN0.MB[mailbox].DATA[i] = 0x00;

    CAN0.MIER |= 1 << mailbox;

    CAN0.MB[mailbox].ID.LONG = 0x7FF < frame.id ? frame.id : (frame.id << 18);
    CAN0.MB[mailbox].ID.BIT.RTR = frame.isRemoteFrame;
    CAN0.MB[mailbox].ID.BIT.IDE = 0x7FF < frame.id;
    CAN0.MB[mailbox].TS.WORD = 0x0000;
    CAN0.MB[mailbox].DLC.BIT.DLC = frame.dlc;

    for (int i = 0; i < 8; i++)
        CAN0.MB[mailbox].DATA[i] = frame.data[i];
}

void CANWrapper::transmit(unsigned int mailbox)
{
    if (31 < mailbox)
        return;

    CAN0.MCTL[mailbox].BIT.RX.RECREQ = 0;
    CAN0.MCTL[mailbox].BIT.TX.TRMREQ = 1;
}

void CANWrapper::markAsReceiver(unsigned int mailbox, unsigned int id, bool isRemoteFrame)
{
    if (31 < mailbox)
        return;

    if (0x7FF < id)
    {
        CAN0.MB[mailbox].ID.LONG = id;
        CAN0.MB[mailbox].ID.BIT.IDE = 1;
    }
    else
    {
        CAN0.MB[mailbox].ID.BIT.SID = id;
        CAN0.MB[mailbox].ID.BIT.IDE = 0;
    }

    CAN0.MB[mailbox].ID.BIT.RTR = isRemoteFrame;

    CAN0.MIER |= 1 << mailbox;
    CAN0.MCTL[mailbox].BIT.TX.TRMREQ = 0;
    CAN0.MCTL[mailbox].BIT.RX.RECREQ = 1;
}

CANWrapper::CANFrame CANWrapper::retrieveFrame(unsigned int mailbox)
{
    CANWrapper::CANFrame frame;

    if (31 < mailbox)
        return frame;

    if (CAN0.MB[mailbox].ID.BIT.IDE)
        frame.id = CAN0.MB[mailbox].ID.LONG & 0x1FFFFFFF;
    else
        frame.id = CAN0.MB[mailbox].ID.BIT.SID;

    frame.isRemoteFrame = CAN0.MB[mailbox].ID.BIT.RTR;
    frame.dlc = CAN0.MB[mailbox].DLC.BIT.DLC;

    for (int i = 0; i < 8; i++)
        frame.data[i] = CAN0.MB[mailbox].DATA[i];

    return frame;
}

void CANWrapper::clearMailbox(unsigned int mailbox)
{
    if (31 < mailbox)
        return;

    CAN0.MB[mailbox].ID.LONG = 0x00000000;
    CAN0.MB[mailbox].DLC.WORD = 0x0000;
    CAN0.MB[mailbox].TS.WORD = 0x0000;

    for (int i = 0; i < 8; i++)
        CAN0.MB[mailbox].DATA[i] = 0x00;

    CAN0.MCTL[mailbox].BYTE = 0x00;
}
