#include <stdint.h>
#include <HID-Project.h>

struct KeybindInfo
{
  int keys[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};//この順で入力される。大文字のAが打ちたいなら先にKEY_LEFT_SHFTを入力する必要あり。
  int consumers[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  bool repeat = false;
} ;


struct Keybind{
  static KeybindInfo key[12];
  
  //ここでキーバインドする
  static void init() {
    key[0].keys[0] = KEY_LEFT_CTRL;
    key[0].keys[1] = KEY_Z;

    key[1].consumers[0] = CONSUMER_BRIGHTNESS_DOWN;

    key[2].keys[0] = KEY_A;
    key[2].repeat = true;

    key[3].consumers[0] = CONSUMER_BRIGHTNESS_UP;
  }

};
KeybindInfo Keybind::key[12];
