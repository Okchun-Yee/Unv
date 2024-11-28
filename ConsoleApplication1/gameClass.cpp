#include "gameClass.h"
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <vector>
#include <set>
#include <Windows.h>

using namespace std;

void gameClass::setRect(int x, int y, int w, int h) {
    //rect : 화면 설정
    this->rect = { x, y, w, h };
}
void gameClass::setXY(int valX, int valY) {
    this->dx = valX;
    this->dy = valY;
}
void gameClass::setTexture(SDL_Texture* tex) {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    this->texture = tex;
}
void gameClass::updatePosition() {
    rect.x += dx;
    rect.y += dy;
}

void clear() {
    Sleep(500);
    system("cls");
}

void gameBase(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    int cardtemp[CARDSIZE];
    srand(static_cast<unsigned int>(time(NULL)));  // 난수 시드 한 번만 설정

    // 난수를 사용해 크기를 랜덤하게 지정해, 순서 예측을 불가능하게 생성
    for (int i = 0; i < CARDSIZE; i++) cardtemp[i] = rand() % 100;

    // 난수 크기대로 정렬
    for (int i = CARDSIZE - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (cardtemp[j] > cardtemp[j + 1]) {
                int temp = cardtemp[j];
                cardtemp[j] = cardtemp[j + 1];
                cardtemp[j + 1] = temp;

                temp = cardNum[j];
                cardNum[j] = cardNum[j + 1];
                cardNum[j + 1] = temp;
            }
        }
    }

    // 이미지 불러오기
    for (int i = 0; i < CARDSIZE; i++) {
        imageSurface[i] = IMG_Load(cardImage[cardNum[i]]);
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
        }
    }

    // 텍스처 생성
    for (int i = 0; i < CARDSIZE; i++) {
        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
    }

    // 랜덤하게 배치할 카드 수 (5~7장)
    int randomCheck = 5 + rand() % 3; // 5~7 중 랜덤 생성

    // 화면을 4등분 (2x2)으로 나누어 각 구역에 랜덤하게 카드 배치
    int numRows = 4; // 행 수
    int numCols = 4; // 열 수
    set<pair<int, int>> usedPositions;  // 좌표를 set으로 관리하여 중복을 빠르게 확인

    // 카드 배치
    for (int i = 0; i < randomCheck; i++) {
        // 각 카드에 대해 랜덤으로 배치할 구역 선택
        int randX = rand() % numRows;
        int randY = rand() % numCols;
        cout <<"pos_1 : " << randX << "\t" << randY << endl;
        int retryCount = 0;
        while (usedPositions.find(make_pair(randX, randY)) != usedPositions.end()) {
            // 중복된 좌표가 발견되면 새로 생성
            randX = rand() % numRows;;
            randY = rand() % numCols;
            cout << "pos_2 : " << randX << "\t" << randY << endl;
            retryCount++;
            if (retryCount > 16) {
                cout << "Too many retries, adjusting the position randomly..." << endl;
                break;
            }
        }
        // 사용된 좌표 저장
        usedPositions.insert(make_pair(randX, randY));
        // 카드의 위치 설정
        obj[i].setRect(randX*180, randY*180, imageSurface[0]->w, imageSurface[0]->h);
        // 카드 렌더링
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
        // 화면에 그리기
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    //화면에 표시된 수 만큼의 딜레이
    for (int i = 0; i < randomCheck; i++) {
        clear();
        cout << i + 1 << "s" << endl;
    }

    // 새로운 이미지 렌더링 (예시로 다른 이미지 사용)
    for (int i = 0; i < randomCheck; i++) {
        // 새로운 이미지 로드 및 렌더링
        imageSurface[i] = IMG_Load("images/free01.png");  // 다른 이미지로 변경
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }

    // 화면에 두 번째 렌더링 (새로운 이미지)
    SDL_RenderPresent(renderer);
}




