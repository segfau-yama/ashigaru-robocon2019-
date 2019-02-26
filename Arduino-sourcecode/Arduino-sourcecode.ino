//#include "MotorCtrl.h"

// ここでピンの指定をする
#define PWM_PIN1    9
#define PWM_PIN2    10
#define PWM_PIN3    11

#define DIGITAL_PIN1    4
#define DIGITAL_PIN2    5
#define DIGITAL_PIN3    6

/* ピンの位置
       (1)
        |

    |       |
   (2)      (3)
*/


/////////////////////////
// 以下くらす宣言
class MotorCtrl {
private:
    int operationThreshold = 10;
    void mtCtrl(int pin,char power); // pin は　1~3で指定
public:
    MotorCtrl();
    void setOperationThreshold(int percent); // 作動しない範囲の指定 (初期値10%, 0~100%で指定)
    void move(const unsigned char x,const unsigned char y);
    void stop(){
        move(0,0);
    }
};

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
    char correct_y = y - 128;
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

MotorCtrl mt;

void setup() {
  // put your setup code here, to run once
}

void loop() {
  // put your main code here, to run repeatedly:
  mt.move(2,4);
}
