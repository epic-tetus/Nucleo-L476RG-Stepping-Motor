# Nucleo-L476G 바이폴라 스테핑 모터 제어
A4988 스테핑 모터 드라이버를 이용하여 바이폴라 스테핑 모터를 제어한다.

## 스테핑 모터의 구동방식
스테핑 모터에는 크게 2가지의 구동방식이 있습니다.

![UniPolar_Bipolar](/Image/UniPolar_BiPolar.png)

+ 유니폴라(Unipolar) 방식은 각각의 상(BLACK과 GREEN)의 사이에 Tap(YELLOW)선이 추가된 형태입니다.

    Tap선에는 VCC가 공급되어 각각의 상에 신호를 주게 되면 해당 상에 전기가 통하여 코일이 동작합니다.

<br/>

+ 바이폴라(Bipolar) 방식은 유니폴라 방식에서 Tap선이 빠진 형태입니다.

    유니폴라 방식은 코일에 흐르는 전류의 방향이 언제나 일정하지만, 바이폴라는 전류의 방향을 바꿔서 모터를 동작시킵니다.

    형태를 보면 선이 2개인 DC모터를 제어 할 때 사용하는 H브릿지 형태와 같습니다.

<br/>

## A4988 스테핑모터 드라이버
A4988 스테핑모터 드라이버는 바이폴라 스테핑 모터를 제어하기에 적합한 드라이버입니다.

하지만, 선이 6개이거나 8개인 유니폴라 스테핑 모터도 제어 할 수 있습니다.

경쟁 모델로 DRV8825가 있는데, 성능면에서는 더 우수하나 A4988 가격이 더 저렴합니다.

하지만, 정격 전류가 2A를 넘어가는 경우에는 DRV8825를 추천 드립니다.

[A4988 데이터 시트](https://www.pololu.com/file/download/a4988_DMOS_microstepping_driver_with_translator.pdf?file_id=0J450)

[DRV8825 데이터 시트](http://www.ti.com/lit/ds/symlink/drv8825.pdf)

<br/>

## 배선
![Routing](/Routing/Nucleo-Bipolar-Steppingmotor_bb.png)

스텝모터와 드라이버 사이를 배선 할 때, 모터의 A, B, C, D 핀 번호를 잘 보고 배선 해야됩니다.

제가 사용한 모터의 도면은 아래와 같습니다.

![스텝모터 도면](/Image/Specific_Dimension.png)

+ 보통 A와 C가 A1상과 A2상을 의미하며, B와 D가 B1, B2입니다.

+ SLEEP핀은 대기상황에서 전류의 소모를 아끼기 위해서 사용되는 핀입니다만 현재는 사용할 필요가 없습니다.

+ RESET핀에는 HIGH신호를 주게되면 STEP핀으로 제어를 할 수 없게 됩니다. 그래서, 모터를 제어하기위해 SLEEP핀에 연결하여 LOW상태로 유지 시켜야 합니다.

+ STEP핀에는 펄스를 주어서 모터를 한 스텝만큼 움직이게해 줍니다. 펄스폭이 길면 그만큼 속도는 느리지만 힘은 세지고, 짧을수록 속도는 빠르고 힘은 약해집니다.

+ DIR핀은 모터의 회전 방향을 결정해줍니다. 제 모터에서 테스트한 결과 1이 시계방향, 0이 반시계방향이였습니다.

<br/>

## 코드

nucleo 보드를 개발할 수 IDE는 KEIL, IAR과 Mbed Compiler등 다양하지만 저는 PlatformIO라는 오픈소스 IDE를 사용합니다.

Nucleo Board 뿐만아니라 아두이노, 라즈베리파이외에 다양한 Cortex 보드들을 지원하며 VS Code, Atom의 확장기능으로 존재합니다. 

[PlatformIO 홈페이지](https://platformio.org/)

```cpp
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
```