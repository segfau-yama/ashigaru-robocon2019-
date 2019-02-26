//#include "MotorCtrl.h"

/*

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

*/
