#include <M5Stack.h>


// 超音波センサに使うピン番号
// TRIGPINはセンサのTRIGに
// ECHOPINはセンサのECHOにつなげる

// 1つめのセンサ
#define TRIGPIN1 16
#define ECHOPIN1 17

// 2つめのセンサ
#define TRIGPIN2 19
#define ECHOPIN2 35

double distanceCm1 = 0, distanceCm2 = 0;

void setup() {

  //M5Stackの初期化
  M5.begin();

  // M5Stackの内蔵スピーカーをOFFにする（騒音対策）
  dacWrite(25, 0);

  // pinモードの指定
  // TRIGPINは出力、ECHOPINは入力
  pinMode(TRIGPIN1, OUTPUT);
  pinMode(ECHOPIN1, INPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);
}

// 超音波センサで距離を測定する関数
double measureDistance(int tpin, int epin) {

  double dtime = -1, dist = -1;

  digitalWrite(tpin, LOW);
  delayMicroseconds(2);

  // TRIGPINからHIGHを出力
  digitalWrite(tpin, HIGH);
  delayMicroseconds(10);

  digitalWrite(tpin, LOW);

  // 帰ってくる時間で距離を計算する
  dtime = pulseIn(epin, HIGH);
  if(dtime > 0) {
    dtime = dtime / 2;
    dist = dtime * 340 * 100 / 1000000;
  }

  return dist;
}

void loop() {

  //センサ1で測定
  distanceCm1 = measureDistance(TRIGPIN1, ECHOPIN1);
  //センサ2で測定
  //distanceCm2 = measureDistance(TRIGPIN2, ECHOPIN2);

  // 結果を表示
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("distance = %f, d2 = %f\n", distanceCm1, distanceCm2);

  delayMicroseconds(10);
}
