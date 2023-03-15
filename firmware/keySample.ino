#include <Arduino.h>
//#include <Keyboard.h>

#include <HID-Project.h>
#include "keybind.h"

//----------------------------------------//
// AE_KEYPAD4X3   ------   Arduino UNO    //
//                                        //
//   |>        <------       5.0V         //
//   X         ------>       8            //
//   Y         ------>       7            //
//   Z         ------>       6            //
//   A         <------       5            //
//   B         <------       4            //
//   C         <------       3            //
//   D         <------       2            //
//----------------------------------------//

void AE_KEYPAD4X3_Init(void);
int AE_KEYPAD4X3_getKey(void);
char AE_KEYPAD4X3_getKeyChar(void);


// KEYPAD用のピン記述
#define _KEY_X 8
#define _KEY_Y 7
#define _KEY_Z 6
#define _KEY_A 5
#define _KEY_B 4
#define _KEY_C 3
#define _KEY_D 2

// ロータリーエンコーダー用のピン記述
#define _ROT_A 15
#define _ROT_B 16

void setup() {
  Serial.begin(9600);
  Serial.println("AE_KEYPAD4X3 Test");

  AE_KEYPAD4X3_Init();  //KEYPAD用の入出力ピン設定

  //ロータリーエンコーダーようの設定
  pinMode(_ROT_A, INPUT_PULLUP);
  pinMode(_ROT_B, INPUT_PULLUP);

  //PCにキーボードとして認識させる
  Keyboard.begin();

  //キーバインドのセット
  Keybind::init();
}

unsigned long previousMillis = 0;
const long interval = 100;  // 1秒間隔で処理を実行することを意味します

bool encoderPreviousState = false;
int previous_ROT_A = 1;
int previous_ROT_B = 1;
int rot = 0;
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Key_Output();
  }

  //ロータリーエンコーダーの入力確認
  int current_ROT_A = digitalRead(_ROT_A);
  int current_ROT_B = digitalRead(_ROT_B);
  if (encoderPreviousState == false) {
    //ピンAが下がった瞬間
    if (current_ROT_A == 0 && previous_ROT_A == 1) {
      if (current_ROT_B == 0) {
        encoderPreviousState = true;
        rot++;
      }
    }
    //ピンBが下がった瞬間
    if (current_ROT_B == 0 && previous_ROT_B == 1) {
      if (current_ROT_A == 0) {
        encoderPreviousState = true;
        rot--;
      }
    }
  }
  previous_ROT_A = current_ROT_A;
  previous_ROT_B = current_ROT_B;
  //1入力の終了を検知
  if (current_ROT_A == 1 && current_ROT_B == 1) encoderPreviousState = false;

  Serial.println(rot);
}

int beforeNumber = -1;
//キーボード機能の出力
void Key_Output() {
  int cdata;
  cdata = AE_KEYPAD4X3_getKeyInt();  //押されたキーを文字で出力する 例 '1'や'#'など
  //Serial.println(cdata);

  if (cdata != -1) {

    if (beforeNumber != cdata || Keybind::key[cdata].repeat == true) {
      beforeNumber = cdata;

      //キー入力
      for (int i = 0; i < 10; ++i) {
        if (Keybind::key[cdata].keys[i] == -1) break;                               //入力が-1ならこれ以降入力なしと判定
        Keyboard.press(static_cast<KeyboardKeycode>(Keybind::key[cdata].keys[i]));  //引数はKeyboardKeycode型じゃないと駄目っぽい
      }
      Keyboard.releaseAll();

      //特殊な入力
      for (int i = 0; i < 10; ++i) {
        if (Keybind::key[cdata].consumers[i] == -1) break;
        Consumer.press(static_cast<ConsumerKeycode>(Keybind::key[cdata].consumers[i]));
      }
      Consumer.releaseAll();
    }
  } else {
    beforeNumber = -1;
  }
}

//----------------------------------------------------//
// KEYPAD用の入出力ピン設定                             //
//----------------------------------------------------//
void AE_KEYPAD4X3_Init(void) {
  pinMode(_KEY_X, OUTPUT);
  pinMode(_KEY_Y, OUTPUT);
  pinMode(_KEY_Z, OUTPUT);
  pinMode(_KEY_A, INPUT);
  pinMode(_KEY_B, INPUT);
  pinMode(_KEY_C, INPUT);
  pinMode(_KEY_D, INPUT);
  digitalWrite(_KEY_X, 1);
  digitalWrite(_KEY_Y, 1);
  digitalWrite(_KEY_Z, 1);
}

//----------------------------------------------------//
// 押されたキーを検出し、数字を返します。
//----------------------------------------------------//
int AE_KEYPAD4X3_getKey(void) {
  digitalWrite(_KEY_X, 0);
  digitalWrite(_KEY_Y, 1);
  digitalWrite(_KEY_Z, 1);
  if (digitalRead(_KEY_A) == 0)
    return 10;
  if (digitalRead(_KEY_B) == 0)
    return 7;
  if (digitalRead(_KEY_C) == 0)
    return 4;
  if (digitalRead(_KEY_D) == 0)
    return 1;

  digitalWrite(_KEY_X, 1);
  digitalWrite(_KEY_Y, 0);
  digitalWrite(_KEY_Z, 1);
  if (digitalRead(_KEY_A) == 0)
    return 11;
  if (digitalRead(_KEY_B) == 0)
    return 8;
  if (digitalRead(_KEY_C) == 0)
    return 5;
  if (digitalRead(_KEY_D) == 0)
    return 2;

  digitalWrite(_KEY_X, 1);
  digitalWrite(_KEY_Y, 1);
  digitalWrite(_KEY_Z, 0);
  if (digitalRead(_KEY_A) == 0)
    return 12;
  if (digitalRead(_KEY_B) == 0)
    return 9;
  if (digitalRead(_KEY_C) == 0)
    return 6;
  if (digitalRead(_KEY_D) == 0)
    return 3;

  return 0;
}

//----------------------------------------------------//
// 押されたキーを検出し、一文字を返します。
//----------------------------------------------------//
char AE_KEYPAD4X3_getKeyChar(void) {
  if (AE_KEYPAD4X3_getKey() == 1) return '1';
  else if (AE_KEYPAD4X3_getKey() == 2) return '2';
  else if (AE_KEYPAD4X3_getKey() == 3) return '3';
  else if (AE_KEYPAD4X3_getKey() == 4) return '4';
  else if (AE_KEYPAD4X3_getKey() == 5) return '5';
  else if (AE_KEYPAD4X3_getKey() == 6) return '6';
  else if (AE_KEYPAD4X3_getKey() == 7) return '7';
  else if (AE_KEYPAD4X3_getKey() == 8) return '8';
  else if (AE_KEYPAD4X3_getKey() == 9) return '9';
  else if (AE_KEYPAD4X3_getKey() == 10) return '#';
  else if (AE_KEYPAD4X3_getKey() == 11) return '0';
  else if (AE_KEYPAD4X3_getKey() == 12) return '*';
  else return '\0';
}

int AE_KEYPAD4X3_getKeyInt(void) {
  if (AE_KEYPAD4X3_getKey() == 1) return 1;
  else if (AE_KEYPAD4X3_getKey() == 2) return 2;
  else if (AE_KEYPAD4X3_getKey() == 3) return 3;
  else if (AE_KEYPAD4X3_getKey() == 4) return 4;
  else if (AE_KEYPAD4X3_getKey() == 5) return 5;
  else if (AE_KEYPAD4X3_getKey() == 6) return 6;
  else if (AE_KEYPAD4X3_getKey() == 7) return 7;
  else if (AE_KEYPAD4X3_getKey() == 8) return 8;
  else if (AE_KEYPAD4X3_getKey() == 9) return 9;
  else if (AE_KEYPAD4X3_getKey() == 10) return 10;
  else if (AE_KEYPAD4X3_getKey() == 11) return 0;
  else if (AE_KEYPAD4X3_getKey() == 12) return 11;
  else return -1;
}
