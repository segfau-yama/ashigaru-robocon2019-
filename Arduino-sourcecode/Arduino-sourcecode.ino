// こいつが置いてあるディレクトリにreadme.txtがあるから読め!!
// これと, ここのソースコードを直接いじって!!!! おねがい♡

// ここでピンの指定をする
#define PWM_PIN1    9
#define PWM_PIN2    10
#define PWM_PIN3    11

#define DIGITAL_PIN1    4
#define DIGITAL_PIN2    5
#define DIGITAL_PIN3    6

#define TURN_RIGHT   0
#define TURN_LEFT    1

#define STOP_MT 0

/* ピンの位置
       (1)
        |

    |       |
   (2)      (3)
*/

// 通信回りの宣言
//////////////////////////////////////////////////////////////////////////
#include <PS3BT.h>
//#include <Wii.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb); 
PS3BT PS3(&Btd); 
//WII Wii(&Btd, PAIR);
bool printTemperature,printAngle;


/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
<<         分 割 失 敗 し た ! !　   　  メインまで読み飛ばしてもいい!!                 >>
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//*/

///////////////////////////////////////////////////////////////////////////
// 実はｈファイルに分割したい部分
/////////////////////////

// 以下くらす宣言
class MotorCtrl {
    // ３輪のベクトル制御のクラス
private:
    int operationThreshold = 10;
    void mtCtrl(int pwmPin,int digitalPin,char power);
public:
    MotorCtrl(); // コンストラクタ
    void setOperationThreshold(int percent); // 作動しない範囲の指定 (初期値10%, 0~100%で指定)
    void move(const unsigned char x,const unsigned char y); // 0~255で値を入れる
    void move(const unsigned char debug_code); // デバッグ用隠し機能? (オーバーロード)
    void stop() { move(STOP_MT); } // モーター停止
    void turn(const unsigned char RIGHT_OR_LEFT,const unsigned char power); // define に RIGHT と LEFT があるからそれを入れる
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

void MotorCtrl::mtCtrl(int pwmPin,int digitalPin,char power){
    if(power >= 0){
        digitalWrite(digitalPin,0);
        analogWrite(pwmPin,power);
    }
    else {
        digitalWrite(digitalPin,1);
        analogWrite(pwmPin,255 - power);
    }
}

void MotorCtrl::setOperationThreshold(int percent) {
    operationThreshold = ((percent <= 100 && percent >= 0) ? percent : 20);
}

void MotorCtrl::move (const unsigned char x, const unsigned char y) {
    char correct_x = x - 128;
    char correct_y = 128 - y;
    if(hypot(correct_y,correct_x)/128*100 < operationThreshold){
        digitalWrite(DIGITAL_PIN1,0);
        digitalWrite(DIGITAL_PIN1,0);
        digitalWrite(DIGITAL_PIN1,0);

        analogWrite(PWM_PIN1 ,0);
        analogWrite(PWM_PIN2 ,0);
        analogWrite(PWM_PIN3 ,0);
    } 
    else {
        // 出力処理処理
        mtCtrl(PWM_PIN1,DIGITAL_PIN1,(char)((sin(-1.0)*correct_x)+(cos(0.0)*correct_y)));
        mtCtrl(PWM_PIN2,DIGITAL_PIN2,(char)((sin(1.0/2)*correct_x)+(cos(-sqrt(3.0)/2.0)*correct_y)));
        mtCtrl(PWM_PIN3,DIGITAL_PIN3,(char)((sin(1.0/2)*correct_x)+(cos(sqrt(3.0)/2.0)*correct_y)));
    }
}

void MotorCtrl::move(const unsigned char debug_code){
    if(debug_code == STOP_MT){
        mtCtrl(PWM_PIN1,DIGITAL_PIN1,0);
        mtCtrl(PWM_PIN2,DIGITAL_PIN2,0);
        mtCtrl(PWM_PIN3,DIGITAL_PIN3,0);
    }
    
}

void MotorCtrl::turn(const unsigned char RIGHT_OR_LEFT, const unsigned char power){
    if(RIGHT_OR_LEFT == RIGHT){
        mtCtrl(PWM_PIN1,DIGITAL_PIN1,power);
        mtCtrl(PWM_PIN2,DIGITAL_PIN2,power);
        mtCtrl(PWM_PIN3,DIGITAL_PIN3,power);
    }else if(RIGHT_OR_LEFT == LEFT){
        mtCtrl(PWM_PIN1,DIGITAL_PIN1,255 - power);
        mtCtrl(PWM_PIN2,DIGITAL_PIN2,255 - power);
        mtCtrl(PWM_PIN3,DIGITAL_PIN3,255 - power);
    }
}

//////////////////////////////////////////////////////////////////////////////////

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\
// これより下にコードは書いてね!!

MotorCtrl motor;  // これはクラスを使う例 使うときは消してね

void setup() {
  // put your setup code here, to run once

    //通信回りの初期化
    //pinMode(13,OUTPUT);
    Serial.begin(115200);
#if !defined(__MIPSEL__)
    while (!Serial);
#endif
    if (Usb.Init() == -1) {
        Serial.print(F("\r\n通信失敗"));
        while (1); //halt
    }
    Serial.print(F("\r\n通信開始"));

}

void loop() {
  // put your main code here, to run repeatedly:
    //int x,y;
    Usb.Task();

    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
      if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
        if (PS3.PS3Connected) { // The Navigation controller only have one joystick
          Serial.print(F("\tRightHatX: "));
          Serial.print(PS3.getAnalogHat(RightHatX));
          Serial.print(F("\tRightHatY: "));
          Serial.print(PS3.getAnalogHat(RightHatY));
          motor.move(PS3.getAnalogHat(RightHatX),PS3.getAnalogHat(RightHatY));
        }
      }
      if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
        Serial.print(F("\r\nL2: "));
        Serial.print(PS3.getAnalogButton(L2));
        motor.turn(TURN_LEFT, 100);
        if (PS3.PS3Connected) {
          Serial.print(F("\tR2: "));
          Serial.print(PS3.getAnalogButton(R2));
          motor.turn(TURN_RIGHT, 100);
        }
      }
    }

    // 通信の確認？
    /*if (Wii.wiimoteConnected) {
        digitalWrite(13,LOW);
        if (Wii.getButtonClick(HOME)) {
            Serial.print(F("\r\nHOME"));
            Wii.disconnect();
        }
    }
    
    if (Wii.nunchuckConnected) {
            Serial.print(F("\r\nHatX: "));
            Serial.print(Wii.getAnalogHat(HatX));
            Serial.print(F("\tHatY: "));
            Serial.print(Wii.getAnalogHat(HatY));

            // xとyの値取り出し であってる? motor.move(Wii.getAnalogHat(HatX),Wii.getAnalogHat(HatY)); でもいいかもね
            x=Wii.getAnalogHat(HatX);
            y=Wii.getAnalogHat(HatY);

            // 移動処理だよねこれ
            motor.move(x,y);*/
}
