#include <M5Stack.h>
#include "control_map.h"

void setup(){
    m5.begin();
    Serial2.begin(115200);  
}

/*!
 * \brief LidarCar::ControlWheel
 * \param 
 *      X   範囲 -7 ~ 7
 *      Y   範囲 -7 ~ 7
 *      A   範囲0/1水平移動標識、1の場合、左右に移動できます
 * - 車両の方向と速度の制御
 */
void ControlWheel(int8_t X, int8_t Y, byte A)// X, Y, A: -7 ~ 7 
    //controlMapテーブルを介してパラメーターを4つのモーターの制御値に変換する
    //controlMapX 水平移動用のデータテーブル
    //controlMap  水平移動用のデータテーブル 
{  
    int wheelPowerA, wheelPowerB, wheelPowerC, wheelPowerD;

  if (A == 0x01){
    wheelPowerA = controlMapX[7 + Y][X + 7];
    wheelPowerB = controlMapY[7 + Y][X + 7];
    wheelPowerC = controlMapY[7 + Y][X + 7];
    wheelPowerD = controlMapX[7 + Y][X + 7];
  }else{
    wheelPowerA = controlMap[-Y + 7][X + 7];
    wheelPowerB = controlMap[-Y + 7][14 - X - 7];
    wheelPowerC = controlMap[-Y + 7][X + 7];
    wheelPowerD = controlMap[-Y + 7][14 - X - 7];
  }
   //シリアルポートを介して指示を発行する
  Serial2.write(0xAA);
  Serial2.write(wheelPowerA);
  Serial2.write(wheelPowerB);
  Serial2.write(wheelPowerC);
  Serial2.write(wheelPowerD);
  Serial2.write(0x55);
}

void loop(){
    ControlWheel(0, 7, 0);
    delay(1000);
    ControlWheel(7, 0, 0);
    delay(1000);
    ControlWheel(7, 0, 1);
    delay(1000);
    ControlWheel(0, 0, 0);
    delay(1000);
}
