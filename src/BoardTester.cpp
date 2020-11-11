/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.cpp                                   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

#include "iodefine.h"
#include "../cpwm/cpwm.h"
#include "../sensors/sensors.h"
#include "../timer/timer.h"
#include "../can/canwrapper.h"

#define DEFAULT_TIMEOUT 0x20000

extern "C"
{
#include "uartprint.h"
}

#define CARRIER_CYCLE 5000
#define DEADTIME 100

void main(void)
{
    /////////////////////////////////////////////
    /// 保護リセットずっと出しとく、デバッグ用なので要解除 ///
    /////////////////////////////////////////////
    PORT2.DDR.BIT.B4 = 1;
    PORT2.DR.BIT.B4 = 1; // リセットボタン離した状態

    MSTP(MTU) = 0;
    MSTP(S12AD0) = 0;
    MSTP(S12AD1) = 0;
    MSTP(CMT0) = 0;
    MSTP(CAN0) = 0;
    MSTP(SCI1) = 0;

    IEN(CMT0, CMI0) = 1;
    IPR(CMT0, CMI0) = 2;

    IEN(CAN0, TXM0) = 1;
    IEN(CAN0, RXM0) = 1;
    IPR(CAN0, TXM0) = 4;
    IPR(CAN0, RXM0) = 15;

    IEN(SCI1, ERI1) = 1;
    IEN(SCI1, RXI1) = 1;
    IEN(SCI1, TXI1) = 1;
    IEN(SCI1, TEI1) = 1;
    IPR(SCI1, ERI1) = 2;
    IPR(SCI1, RXI1) = 2;
    IPR(SCI1, TXI1) = 2;
    IPR(SCI1, TEI1) = 2;

    UART1_TX_init();

    Sensors::setup();
    CPWM::setup(CARRIER_CYCLE, DEADTIME);
    Timer::setup();
    CANWrapper::wake(DEFAULT_TIMEOUT);
    CANWrapper::setup();

    CANWrapper::markAsReceiver(0, 0x00000001, false);

    CPWM::start();
//    Timer::start();
//    MTU.TCSYSTR.BYTE = 0x98;
    Timer::start();

    while (1)
    {
    }
}

#pragma interrupt(on_cmi0(vect=VECT(CMT0, CMI0)))
void on_cmi0(void)
{
}

#pragma interrupt on_txm0(vect=VECT(CAN0, TXM0))
void on_txm0(void)
{
}

#pragma interrupt on_rxm0(vect=VECT(CAN0, RXM0))
void on_rxm0(void)
{
    CANWrapper::CANFrame incoming = CANWrapper::retrieveFrame(0);

    int throttle = (incoming.data[2] << 8) | (incoming.data[3]);
    float throttleRatio = throttle / 4096.0F;

    CPWM::setDutyU(throttleRatio);
    CPWM::setDutyW(1.0F - throttleRatio);

    CANWrapper::clearMailbox(0);
    CANWrapper::markAsReceiver(0, 0x00000001, false);

    UART1_vtransmit("%d\n", throttle);
}

#ifdef __cplusplus
void abort(void)
{
}
#endif
