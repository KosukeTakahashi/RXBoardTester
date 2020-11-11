/*
 * canwrapper.h
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#ifndef CANWRAPPER_H_
#define CANWRAPPER_H_

namespace  CANWrapper
{
struct CANFrame
{
    int id;
    int dlc;
    bool isRemoteFrame;
    unsigned char data[8];
};


namespace Result
{
enum Type
{
    OK = 0,
    FAIL = 1,
    TIMEOUT = 2,
    INVALID = 3
};
}

namespace Mode
{
enum Type
{
    OPERATION = 0,
    RESET = 1,
    HALT = 2
};
}

Result::Type wake(unsigned int timeout);
Result::Type sleep(unsigned int timeout);
Result::Type switchMode(Mode::Type mode, unsigned int timeout);
void setup();
void loadFrame(unsigned int mailbox, CANFrame frame);
void transmit(unsigned int mailbox);
void markAsReceiver(unsigned int mailbox, unsigned int id, bool isRemoteFrame);
CANFrame retrieveFrame(unsigned int mailbox);
void clearMailbox(unsigned int mailbox);
}

#endif /* CANWRAPPER_H_ */
