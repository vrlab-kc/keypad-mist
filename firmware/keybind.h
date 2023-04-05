#include <stdint.h>
#include <HID-Project.h>

class KeybindInfo
{
  public:
    int outputs[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};//この順で入力される。大文字のAが打ちたいなら先にKEY_LEFT_SHFTを入力する必要あり。
    int consumers[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    bool repeat = false;
} ;


class KeyBindings{
  public:
    static KeybindInfo key[12];
  
  //ここでキーバインドする
    static void init() {
      key[0].outputs[0] = KEY_LEFT_CTRL;
      key[0].outputs[1] = KEY_Z;

      key[1].consumers[0] = CONSUMER_BRIGHTNESS_DOWN;

      key[2].outputs[0] = KEY_A;
      key[2].repeat = true;

      key[3].consumers[0] = CONSUMER_BRIGHTNESS_UP;
    }

};
KeybindInfo KeyBindings::key[12];

struct RotbindInfo
{
  int outputs[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};//この順で入力される。大文字のAが打ちたいなら先にKEY_LEFT_SHFTを入力する必要あり。
  int consumers[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  int sensitivity = 1;
};

struct RotaryEncoderBaind{
  RotbindInfo right,left;
};

struct RotBindings{
  static RotbindInfo rot[2];
  //ここでバインドする
  static void init(){
    
  }
};
