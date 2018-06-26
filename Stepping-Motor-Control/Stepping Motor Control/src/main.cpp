#include <mbed.h>

DigitalOut DIR(D4);
DigitalOut STEP(D5);

int main() {

    while(1) {
        DIR.write(1);   // 시계 방향

        /*
            360 / 1.8 ( 스텝 앵글 ) = 200 ( 한바퀴 )
        */

        for(int i = 0; i < 200; i++){   // 한바퀴 돌리기
            STEP.write(1);              // 펄스 발생
            wait_us(500);               // 딜레이를 늘리면 회전 속도 저하, 줄이면 회전 속도 증가
            STEP.write(0);
            wait_us(500);  
        }   

        DIR.write(0);   // 반시계 방향

        for(int i = 0; i < 100; i++){   // 180도 회전
            STEP.write(1);
            wait_us(1000);
            STEP.write(0);
            wait_us(1000); 
        }
    }
}