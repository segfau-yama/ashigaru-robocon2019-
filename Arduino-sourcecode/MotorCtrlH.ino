/*
// ここでピンの指定をする
#define PWM_PIN1    9
#define PWM_PIN2    10
#define PWM_PIN3    11

#define DIGITAL_PIN1    4
#define DIGITAL_PIN2    5
#define DIGITAL_PIN3    6
*/


/* ピンの位置
       (1)
        |

    |       |
   (2)      (3)
*/


/*
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

*/
