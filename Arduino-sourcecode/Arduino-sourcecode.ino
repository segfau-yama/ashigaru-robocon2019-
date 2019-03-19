// ここでピンの指定をする
#define PWM_PIN1    9
#define PWM_PIN2    10
#define PWM_PIN3    11

#define DIGITAL_PIN1    4
#define DIGITAL_PIN2    5
#define DIGITAL_PIN3    6

#define RIGHT   0
#define LEFT    1

#define STOP_MT 0

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
    // ３輪のベクトル制御のクラス
private:
    int operationThreshold = 10;
    void mtCtrl(int pwmPin,int digitalPin,char power);
public:
    MotorCtrl();
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

  motor.setOperationThreshold(20); // スティックの感じやすさ 0~100で調節できて100だと感じない (初期値は20)
  motor.stop(); // 緊急停止に使う
}

void loop() {
  // put your main code here, to run repeatedly:
  motor.move(255,255);
  motor.turn(RIGHT,255);
}
