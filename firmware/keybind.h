#include <stdint.h>
#include <HID-Project.h>

//キーボード
class KeybindInfo {
public:
  int outputs[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };  //この順で入力される。大文字のAが打ちたいなら先にKEY_LEFT_SHFTを入力する必要あり。
  int consumers[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  bool repeat = false;
};

class KeyBindings {
public:
  static KeybindInfo key[12];
  static void init();
};
KeybindInfo KeyBindings::key[12];

//ロータリーエンコーダー
class RotbindInfo {
public:
  int outputs[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };  //この順で入力される。大文字のAが打ちたいなら先にKEY_LEFT_SHFTを入力する必要あり。
  int consumers[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  int sensitivity = 1;
};

class RotLeftRight {
public:
  RotbindInfo right, left;
};

class RotBindings {
public:
  static RotLeftRight rot[2];
  static void init();
};
RotLeftRight RotBindings::rot[2];

//ここでバインドする
static void KeyBindings::init() {
  key[0].outputs[0] = KEY_LEFT_CTRL;
  key[0].outputs[1] = KEY_Z;

  key[1].consumers[0] = CONSUMER_BRIGHTNESS_DOWN;

  key[2].outputs[0] = KEY_A;
  key[2].repeat = true;

  key[3].outputs[0] = KEY_B;
  key[3].consumers[0] = CONSUMER_BRIGHTNESS_UP;
  key[3].repeat = true;
}

static void RotBindings::init(){
  //rot[0].left.consumers[0] = MEDIA_VOLUME_DOWN;
  //rot[0].right.consumers[0] =MEDIA_VOLUME_UP;
}