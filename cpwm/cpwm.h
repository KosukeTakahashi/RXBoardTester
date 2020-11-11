/*
 * cpwm.h
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#ifndef CPWM_H_
#define CPWM_H_

namespace CPWM
{
    void setup(int carrierCycle, int deadtime);
    void setDutyU(float ratio);
    void setDutyV(float ratio);
    void setDutyW(float ratio);
    void start();
    void cutoff();
}

#endif /* CPWM_H_ */
