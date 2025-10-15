#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>
#include <fcntl.h>
#include <stdbool.h>
#include "sensor.h"
#include "segment.h"
#include "motor.h"

#define BUZ_PIN 18

double distances[] = { 0, 0 };
double prevDist = 0.0;
double nowDist = 0.0;

// 부저 비프 함수 (변경 없음)
void buzzer_beep(int times, int interval_ms) {
    for (int i = 0; i < times; i++) {
        gpioSetPWMfrequency(BUZ_PIN, 3500);
        gpioPWM(BUZ_PIN, 240);
        gpioDelay(interval_ms * 1000);
        gpioPWM(BUZ_PIN, 0);
        gpioDelay(interval_ms * 1000);
    }
}

// 논블로킹 키 입력 함수: 스페이스바 감지용
int getch() {
    int ch, oldf;
    struct termios oldattr, newattr;

    // 터미널 속성 저장 및 비캐노니컬·에코 비활성화
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    // 논블로킹 모드 설정
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // 원래 터미널 설정 복구
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return (ch == EOF) ? 0 : ch;
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio init failed\n");
        return 1;
    }

    gpioSetMode(BUZ_PIN, PI_OUTPUT);
    init_gpio();
    initSegment();

    bool paused = false;

    while (1) {
        // 1) 스페이스바 입력 확인 → 정지/재개 토글
        int key = getch();
        if (key == ' ') {
            paused = !paused;
            if (paused) {
                stop();
                printf("== PAUSED ==\n");
            }
            else {
                printf("== RESUME ==\n");
            }
        }
        if (paused) {
            // 일시정지 상태: 센서·모터 로직 스킵
            gpioDelay(100000);
            continue;
        }

        // 2) 거리 측정 및 출력
        double nowDist;
        double prevDist;
        getAllDistances(distances);
        nowDist = distances[0];
        double delta = nowDist - prevDist;
        double speed = (delta < 0) ? -delta / 0.5 : 0.0;

        printf("[Dist: %.2f cm, ΔDist: %.2f cm, Speed: %.2f cm/s]\n",
            nowDist, fabs(delta), speed);

        // 3) 7세그먼트 한 자리 출력
        int digit = (int)(speed / 10.0);
        if (digit > 9) digit = 9;
        usePinCheck(digit - 1);
        displayDigitAt(digit8, 0);

        // 4) 장애물 회피 로직
        if (nowDist <= 30.0) {
            buzzer_beep(3, 200);
            stop();

            // 왼쪽 측정
            turn_left();
            gpioDelay(500000);
            getAllDistances(distances);
            double leftDist = distances[0];

            // 오른쪽 측정
            turn_right(); turn_right();
            gpioDelay(500000);
            getAllDistances(distances);
            double rightDist = distances[0];

            // 더 먼 쪽으로 최종 회전
            if (leftDist >= rightDist) {
                // 현재 오른쪽→왼쪽(180°)
                turn_left(); turn_left();
            }
            // else: 이미 오른쪽 바라보고 있음

            forward();
            prevDist = nowDist;
            continue;
        }
        else {
            gpioPWM(BUZ_PIN, 0);
        }

        // 5) 정상 전진
        forward();
        prevDist = nowDist;
        gpioDelay(500000);
    }

    cleanup();
    gpioTerminate();
    return 0;
}
