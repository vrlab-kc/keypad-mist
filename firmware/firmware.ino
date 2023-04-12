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
  KeyBindings::init();
  RotBindings::init();
}

unsigned long previousMillis = 0;
const long interval = 165;  // 1秒間隔で処理を実行することを意味します 明るさ最速連打は165ほど、それ以上間隔が短いと反応しない

bool encoderPreviousState = false;
int previous_ROT_A = 1;
int previous_ROT_B = 1;
int rot = 0;

//----------------------------------------------------//
// メインループ
//----------------------------------------------------//
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Key_Output();
    Rot_Output();
  }

  //ロータリーエンコーダーの入力確認
  Rot_Input();
}

int rightCount = 0;
int leftCount = 0;

//ロータリーエンコーダーの入出力処理
void Rot_Input() {
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

  //出力処理
  if(rot >= RotBindings::rot[0].right.sensitivity){
    rot -= RotBindings::rot[0].right.sensitivity;
    rightCount++;
    Serial.write("rotation!,");
  }
  if(rot <= -RotBindings::rot[0].left.sensitivity){
    rot += RotBindings::rot[0].left.sensitivity;
    leftCount++;  
  }
  
}

void Rot_Output(){
  if(rightCount > 0){
    rightCount--;
    
    for (int i = 0; i < 10; ++i) {
      if (RotBindings::rot[0].right.outputs[i] == -1) break;                               //入力が-1ならこれ以降入力なしと判定
      Keyboard.press(static_cast<KeyboardKeycode>(RotBindings::rot[0].right.outputs[i]));  //引数はKeyboardKeycode型じゃないと駄目っぽい
    }
    Keyboard.releaseAll();

    //特殊な入力
    for (int i = 0; i < 10; ++i) {
      if (RotBindings::rot[0].left.consumers[i] == -1) break;
      Consumer.press(static_cast<ConsumerKeycode>(RotBindings::rot[0].right.consumers[i]));
    }
    Consumer.releaseAll();   
  }

  if(leftCount > 0){
    leftCount--;

    for (int i = 0; i < 10; ++i) {
      if (RotBindings::rot[0].left.outputs[i] == -1) break;                               //入力が-1ならこれ以降入力なしと判定
      Keyboard.press(static_cast<KeyboardKeycode>(RotBindings::rot[0].left.outputs[i]));  //引数はKeyboardKeycode型じゃないと駄目っぽい
    }
    Keyboard.releaseAll();

    //特殊な入力
    for (int i = 0; i < 10; ++i) {
      if (RotBindings::rot[0].left.consumers[i] == -1) break;
      Consumer.press(static_cast<ConsumerKeycode>(RotBindings::rot[0].left.consumers[i]));
    }
    Consumer.releaseAll();  
  }
}

int beforeNumber = -1;
//キーボード機能の出力
void Key_Output() {
  int keynum;
  keynum = AE_KEYPAD4X3_getKeyInt();  //押されたキーを数字で出力する 0~11 (入力なし-1)

  //入力なしなら 早期リターン
  if (keynum == -1) {
    beforeNumber = -1;
    return;
  }

  //前回と違うキーが押されている　||　押されたキーのリピートがオン
  //なら、キーバインド情報を元に、キー出力する
  if (beforeNumber != keynum || KeyBindings::key[keynum].repeat == true) {
    beforeNumber = keynum;

    //キー入力
    for (int i = 0; i < 10; ++i) {
      if (KeyBindings::key[keynum].outputs[i] == -1) break;                               //入力が-1ならこれ以降入力なしと判定
      Keyboard.press(static_cast<KeyboardKeycode>(KeyBindings::key[keynum].outputs[i]));  //引数はKeyboardKeycode型じゃないと駄目っぽい
    }
    Keyboard.releaseAll();

    //特殊な入力
    for (int i = 0; i < 10; ++i) {
      if (KeyBindings::key[keynum].consumers[i] == -1) break;
      Consumer.press(static_cast<ConsumerKeycode>(KeyBindings::key[keynum].consumers[i]));
    }
    Consumer.releaseAll();
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
