/*
 * timer.cpp
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#include <iodefine.h>
#include "timer.h"

//#define MTU0_COUNTER_LIMIT 5000

void Timer::setup()
{
    // MTU0
//    MTU.TSTRA.BIT.CST0 = 0; // カウンタ停止
//    MTU0.TCR.BIT.TPSC = 0;  // ICLK/1で
//    MTU0.TCR.BIT.CKEG = 0;  // 立ち上がりエッジでカウント
//    MTU0.TCR.BIT.CCLR = 1;  // TGRAコンペアマッチ・インプットキャプチャでカウンタクリア
//    MTU0.TIORH.BIT.IOA = 0; // 出力禁止
//    MTU0.TIORH.BYTE = 0;    // TGRA, TGRB のコンペアマッチによる出力を禁止
//    MTU0.TIORL.BYTE = 0;    // TGRC, TGRD のコンペアマッチによる出力を禁止
//    MTU0.TIER.BIT.TTGE = 1; // MTU0.TGRAコンペアマッチによるAD変換開始要求を許可
//    MTU0.TGRA = MTU0_COUNTER_LIMIT;

    // CMT0
    CMT.CMSTR0.BIT.STR0 = 0;
    CMT0.CMCR.BIT.CKS = 0;
    CMT0.CMCR.BIT.CMIE = 1;
    CMT0.CMCNT = 0;
    CMT0.CMCOR = 3125; // 1ms
}

void Timer::start()
{
//    MTU.TSTRA.BIT.CST0 = 1;
    CMT.CMSTR0.BIT.STR0 = 1;
}

void Timer::stop()
{
//    MTU.TSTRA.BIT.CST0 = 0;
//    MTU0.TCNT = 0;

    CMT.CMSTR0.BIT.STR0 = 0;
    CMT0.CMCNT = 0;
}
