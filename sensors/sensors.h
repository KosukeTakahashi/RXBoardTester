/*
 * sensors.h
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#ifndef SENSORS_H_
#define SENSORS_H_

namespace Sensors
{
    void setup();
    void start();
    void stop();
    float readUniversalAD0();
    float readUniversalAD1();
    float readTemperatureC();
    float readDCA();
    float readUA();
    float readVA();
    float readWA();
};

#endif /* SENSORS_H_ */
