/*
 * sensors.cpp
 *
 *  Created on: 2020/10/15
 *      Author: kosuke
 */

#include <iodefine.h>
#include <math.h>
#include "sensors.h"

void Sensors::setup()
{
    S12AD0.ADCSR.BIT.ADST = 0;      // AD変換停止
    S12AD0.ADCSR.BIT.EXTRG = 0;     // ADCトリガをADSTRGRで選択
    S12AD0.ADCSR.BIT.TRGE = 0;      // 外部・周辺モジュールトリガによるAD変換開始を禁止
    S12AD0.ADCSR.BIT.CKS = 0;       // ADCクロック = PCLK/8
    S12AD0.ADCSR.BIT.ADIE = 0;      // AD変換終了時の割り込みを禁止
    S12AD0.ADCSR.BIT.ADCS = 2;      // 連続スキャンモード
    S12AD0.ADANS.BIT.PG000EN = 0;   // Ch.0 PGA動作禁止
    S12AD0.ADANS.BIT.PG001EN = 0;   // Ch.1 PGA動作禁止
    S12AD0.ADANS.BIT.PG002EN = 0;   // Ch.2 PGA動作禁止
    S12AD0.ADANS.BIT.PG000SEL = 0;  // Ch.0 PGAを経由しない
    S12AD0.ADANS.BIT.PG001SEL = 0;  // Ch.1 PGAを経由しない
    S12AD0.ADANS.BIT.PG002SEL = 0;  // Ch.2 PGAを経由しない
    S12AD0.ADANS.BIT.CH = 3;        // AN000~AN003をスキャン
    S12AD0.ADCER.BIT.SHBYP = 0;     // サンプル&ホールド回路を使用する
    S12AD0.ADCER.BIT.ADPRC = 0;     // 12-bit精度で変換
    S12AD0.ADCER.BIT.ACE = 0;       // ADDRn, ADRDの読み出しによる自動クリアを禁止
    S12AD0.ADCER.BIT.DIAGVAL = 2;   // VREFH0×1/2の電圧でADC自己診断を実施
    S12AD0.ADCER.BIT.DIAGLD = 0;    // ADC自己診断に用いる電圧値を自動的にローテーション
    S12AD0.ADCER.BIT.DIAGM = 1;     // ADCの自己診断を実施する
    S12AD0.ADCER.BIT.ADIE2 = 0;     // グループ0・グループ1トリガそれぞれの変換終了時にS12ADIを発生（連続スキャンモードのため無視）
    S12AD0.ADCER.BIT.ADIEW = 0;     // ダブルトリガそれぞれの変換終了時にS12ADIを発生（連続スキャンモードのため無視）
    S12AD0.ADCER.BIT.ADRFMT = 0;    // AD変換データを右詰めで格納
//    S12AD0.ADSTRGR.BIT.ADSTRS0 = 1; // MTU0.TCNTとMTU0.TGRAとのコンペアマッチでAD変換開始
    S12AD.ADCMPMD0.WORD = 0x0000;   // コンパレータを使用しない

    S12AD1.ADCSR.BIT.ADST = 0;      // AD変換停止
    S12AD1.ADCSR.BIT.EXTRG = 0;     // ADCトリガをADSTRGRで選択
    S12AD1.ADCSR.BIT.TRGE = 0;      // 外部・周辺モジュールトリガによるAD変換開始を禁止
    S12AD1.ADCSR.BIT.CKS = 0;       // ADCクロック = PCLK/8
    S12AD1.ADCSR.BIT.ADIE = 0;      // AD変換終了時の割り込みを禁止
    S12AD1.ADCSR.BIT.ADCS = 2;      // 連続スキャンモード
    S12AD1.ADANS.BIT.PG100EN = 0;   // Ch.0 PGA動作禁止
    S12AD1.ADANS.BIT.PG101EN = 0;   // Ch.1 PGA動作禁止
    S12AD1.ADANS.BIT.PG102EN = 0;   // Ch.2 PGA動作禁止
    S12AD1.ADANS.BIT.PG100SEL = 0;  // Ch.0 PGAを経由しない
    S12AD1.ADANS.BIT.PG101SEL = 0;  // Ch.1 PGAを経由しない
    S12AD1.ADANS.BIT.PG102SEL = 0;  // Ch.2 PGAを経由しない
    S12AD1.ADANS.BIT.CH = 3;        // AN100~AN103をスキャン
    S12AD1.ADCER.BIT.SHBYP = 0;     // サンプル&ホールド回路を使用する
    S12AD1.ADCER.BIT.ADPRC = 0;     // 12-bit精度で変換
    S12AD1.ADCER.BIT.ACE = 0;       // ADDRn, ADRDの読み出しによる自動クリアを禁止
    S12AD1.ADCER.BIT.DIAGVAL = 2;   // VREFH0×1/2の電圧でADC自己診断を実施
    S12AD1.ADCER.BIT.DIAGLD = 0;    // ADC自己診断に用いる電圧値を自動的にローテーション
    S12AD1.ADCER.BIT.DIAGM = 1;     // ADCの自己診断を実施する
    S12AD1.ADCER.BIT.ADIE2 = 0;     // グループ0・グループ1トリガそれぞれの変換終了時にS12ADIを発生（連続スキャンモードのため無視）
    S12AD1.ADCER.BIT.ADIEW = 0;     // ダブルトリガそれぞれの変換終了時にS12ADIを発生（連続スキャンモードのため無視）
    S12AD1.ADCER.BIT.ADRFMT = 0;    // AD変換データを右詰めで格納
//    S12AD1.ADSTRGR.BIT.ADSTRS0 = 1; // MTU0.TCNTとMTU0.TGRAとのコンペアマッチでAD変換開始
    S12AD.ADCMPMD0.WORD = 0x0000;   // コンパレータを使用しない
}

void Sensors::start()
{
    S12AD0.ADCSR.BIT.ADST = 1;
    S12AD1.ADCSR.BIT.ADST = 1;
}

void Sensors::stop()
{
    S12AD0.ADCSR.BIT.ADST = 0;
    S12AD1.ADCSR.BIT.ADST = 0;
}

float Sensors::readUniversalAD0()
{
    return (S12AD0.ADDR0A) / 4095.0F;
}

float Sensors::readUniversalAD1()
{
    return (S12AD0.ADDR1) / 4095.0F;
}

float Sensors::readTemperatureC()
{
    const float log10000 = 9.210340371976183;
    const float k = 11.5268456376; // 3435 / 298
    const float j = 29.7313797314; // 24350 / 819

    float l = log(j * (S12AD0.ADDR3));
    return (3435.0F / (l - log10000 + k)) - 273.0F;
}

float Sensors::readDCA()
{
    return 0.0F;
}

float Sensors::readUA()
{
    return 0.0F;
}

float Sensors::readVA()
{
    return 0.0F;
}

float Sensors::readWA()
{
    return 0.0F;
}
