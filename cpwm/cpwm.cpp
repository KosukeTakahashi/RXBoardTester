/*
 * cpwm.cpp
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#include <iodefine.h>
#include "cpwm.h"

int _carrierCycle = 0;
int _deadtime = 0;

void CPWM::setup(int carrierCycle, int deadtime)
{
    _carrierCycle = carrierCycle;
    _deadtime = deadtime;

    MTU.TSTRA.BYTE &= 0x07;

    MTU3.TCR.BIT.TPSC = 0;
    MTU3.TCR.BIT.CKEG = 0;
    MTU3.TCR.BIT.CCLR = 0;
    MTU4.TCR.BIT.TPSC = 0;
    MTU4.TCR.BIT.CKEG = 0;
    MTU4.TCR.BIT.CCLR = 0;

    MTU.TGCRA.BYTE = 0x80;

    MTU3.TCNT = deadtime;
    MTU4.TCNT = 0;

    MTU3.TGRB = carrierCycle / 4;
    MTU4.TGRA = carrierCycle / 4;
    MTU4.TGRB = carrierCycle / 4;
    MTU3.TGRD = carrierCycle / 4;
    MTU4.TGRC = carrierCycle / 4;
    MTU4.TGRD = carrierCycle / 4;

    MTU.TDDRA = _deadtime;
    MTU.TCDRA = carrierCycle / 2;
    MTU.TCBRA = carrierCycle / 2;
    MTU3.TGRA = carrierCycle / 2 + _deadtime;
    MTU3.TGRC = carrierCycle / 2 + _deadtime;

    MTU.TOCR1A.BIT.PSYE = 1;
    MTU.TOCR1A.BIT.TOCS = 0;
    MTU.TOCR1A.BIT.OLSN = 0;
    MTU.TOCR1A.BIT.OLSP = 0;

    MTU3.TMDR1.BIT.BFA = 1;
    MTU3.TMDR1.BIT.BFB = 1;
    MTU3.TMDR1.BIT.MD = 0xF;

    MTU.TOERA.BYTE = 0x3F;
}

void CPWM::setDutyU(float ratio)
{
    if (ratio < 0.0 || 1.0 < ratio)
        return;

    int duty = _carrierCycle * ratio / 2;
    MTU3.TGRD = duty;
}

void CPWM::setDutyV(float ratio)
{
    if (ratio < 0.0 || 1.0 < ratio)
        return;

    int duty = _carrierCycle * ratio / 2;
    MTU4.TGRC = duty;
}

void CPWM::setDutyW(float ratio)
{
    if (ratio < 0.0 || 1.0 < ratio)
        return;

    int duty = _carrierCycle * ratio / 2;
    MTU4.TGRD = duty;
}

void CPWM::start()
{
    if (POE.SPOER.BIT.MTUCH34HIZ)
        POE.SPOER.BIT.MTUCH34HIZ = 0;
    MTU.TSTRA.BYTE |= 0xC0;
}

void CPWM::cutoff()
{
    POE.SPOER.BIT.MTUCH34HIZ = 1;
}
