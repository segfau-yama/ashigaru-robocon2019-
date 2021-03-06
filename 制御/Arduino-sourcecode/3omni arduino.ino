// ここでピンの指定をする
#define PWM_PIN1    28
#define PWM_PIN2    36
#define PWM_PIN3    42

#define DIGITAL_PIN1    32
#define DIGITAL_PIN2    40
#define DIGITAL_PIN3    44


#include <Wii.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb); 
WII Wii(&Btd, PAIR);
bool printAngle;

/* ピンの位置
       (1)
        |

    |       |
   (2)      (3)
*/




/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
<<         分 割 失 敗 し た ! !　   　  メインまで読み飛ばしてもいい!!                 >>
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//*/

///////////////////////////////////////////////////////////////////////////
// 実はｈファイルに分割したい部分
/////////////////////////

// 以下くらす宣言
class MotorCtrl {
  /// 山林雄武二の制御用クラス
private:
    int operationThreshold = 10;
    void mtCtrl(int pin,char power); // pin は　1~3で指定
public:
    MotorCtrl();
    void setOperationThreshold(int percent); // 作動しない範囲の指定 (初期値10%, 0~100%で指定)
    void move(const unsigned char x,const unsigned char y); // 0~255で値を入れる
    void stop(){
        move(0,0);
    }
};

////////////////////////////////////////////////////////////////////////////////////
// 以下は実はcppファイルで分割したい部分
//#include "MotorCtrl.h"

MotorCtrl::MotorCtrl () {
    pinMode(PWM_PIN2,OUTPUT);
    pinMode(PWM_PIN2,OUTPUT);
    pinMode(PWM_PIN3,OUTPUT);

    pinMode(DIGITAL_PIN1,OUTPUT);
    pinMode(DIGITAL_PIN2,OUTPUT);
    pinMode(DIGITAL_PIN3,OUTPUT);
  
}

void MotorCtrl::mtCtrl(int pin,char power){
    switch (pin) {
        case 1:
            if(power >= 0){
                digitalWrite(DIGITAL_PIN1,0);
                analogWrite(PWM_PIN1,power);
            }
            else {
                digitalWrite(DIGITAL_PIN1,1);
                analogWrite(PWM_PIN1,255 - power);
            } 
        break;
        case 2:
            if(power >= 0){
                digitalWrite(DIGITAL_PIN2,1);
                analogWrite(PWM_PIN2,255 - power);
            }
            else {
                digitalWrite(DIGITAL_PIN2,1);
                analogWrite(PWM_PIN2,255 - power);
            }
        break;
        case 3:
            if(power >= 0){
                digitalWrite(DIGITAL_PIN3,1);
                analogWrite(PWM_PIN3,255 - power);
            }
            else {
                digitalWrite(DIGITAL_PIN3,1);
                analogWrite(PWM_PIN3,255 - power);
            }
        break;
        default:
        break;
    }
}

void MotorCtrl::setOperationThreshold(int percent) {
    operationThreshold = ((percent <= 100 && percent >= 0) ? percent : 20);
}

void MotorCtrl::move (const unsigned char x, const unsigned char y) {
    char correct_x = x - 128;
    char correct_y = 128 - y;
    if(hypot(correct_y,correct_x)/128*100 > operationThreshold){
        digitalWrite(DIGITAL_PIN1,0);
        digitalWrite(DIGITAL_PIN1,0);
        digitalWrite(DIGITAL_PIN1,0);

        analogWrite(PWM_PIN1 ,0);
        analogWrite(PWM_PIN2 ,0);
        analogWrite(PWM_PIN3 ,0);
    } 
    else {
        // 出力処理処理
        mtCtrl(1,(char)((sin(-1.0)*correct_x)+(cos(0.0)*correct_y)));
        mtCtrl(2,(char)((sin(1.0/2)*correct_x)+(cos(-sqrt(3.0)/2.0)*correct_y)));
        mtCtrl(3,(char)((sin(1.0/2)*correct_x)+(cos(sqrt(3.0)/2.0)*correct_y)));
    }
}

//////////////////////////////////////////////////////////////////////////////////

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
// これより下にコードは書いてね!!

MotorCtrl motor;  // これはクラスを使う例

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\n通信失敗"));
    while (1); //halt
  }
  Serial.print(F("\r\n通信開始"));

  motor.setOperationThreshold(20); // スティックの感じやすさ 0~100で調節できて100だと感じない (初期値は20)
  motor.stop(); // 緊急停止に使う
}

void loop() {
  int x,y;
  Usb.Task();

  if (Wii.wiimoteConnected) {
    digitalWrite(13,LOW);
    if (Wii.getButtonClick(HOME)) {
      Serial.print(F("\r\nHOME"));
      Wii.disconnect();
    }
  }
  if (Wii.nunchuckConnected) {
    if (Wii.getAnalogHat(HatX) > 137 ||  Wii.getAnalogHat(HatX) < 117 || Wii.getAnalogHat(HatY) > 137 || Wii.getAnalogHat(HatY) < 117) {
      Serial.print(F("\r\nHatX: "));
      Serial.print(Wii.getAnalogHat(HatX));
      Serial.print(F("\tHatY: "));
      Serial.print(Wii.getAnalogHat(HatY));
      x=Wii.getAnalogHat(HatX);
      y=Wii.getAnalogHat(HatY);
      motor.move(x,y);
    }
  }
}
