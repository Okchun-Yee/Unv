#include "gameClass.h"
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <vector>
#include <algorithm> // std::find
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
    Sleep(1000);
    system("cls");
}

void orignLoad(int cardNum, const char* cardImage, SDL_Renderer* renderer, SDL_Surface* imageSurface, gameClass& obj) {
    imageSurface = IMG_Load(cardImage);  // 원본 이미지로 대체
    if (imageSurface == NULL) {
        cout << "Failed to load image " << endl;
        return;
    }
    obj.setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface));

    // 카드 위치에 원본 이미지 렌더링
    SDL_RenderCopy(renderer, obj.getTexture(), NULL, obj.getRect());
    SDL_RenderPresent(renderer);  // 새로 렌더링된 이미지 표시
}

void gameExit(int& quit, int& cnt) {
    cout << "== WIN ==" << endl;

    SDL_Delay(1000);
    quit = 1;
    cnt = 0;  // 종료 시 cnt 초기화
}

int firstGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    cout << "== Game Start ==" << endl;
    srand(static_cast<unsigned int>(time(NULL)));  // 난수 시드 한 번만 설정

    for (int i = 0; i < CARDSIZE; i++) {
        cardNum[i] = i;
    }

    // 이미지 불러오기
    for (int i = 0; i < CARDSIZE; i++) {
        imageSurface[i] = IMG_Load(cardImage[cardNum[i]]);
        //cout << cardNum[i] << endl;
        //이미지가 없는 경우 err
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
            return 0;
        }
    }

    // 텍스처 생성
    for (int i = 0; i < CARDSIZE; i++) {
        //객체에 이미지 지정하기
        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
    }

    // 랜덤하게 배치할 카드 수 (3~7장)
    int randomCheck = 8 + rand() % 3; // 8~12 중 랜덤 생성

    // 화면을 4등분 (2x2)으로 나누어 각 구역에 랜덤하게 카드 배치
    int numRows = 4; // 행 수
    int numCols = 4; // 열 수
    vector<pair<int, int>> usedPositions;  // 좌표를 set으로 관리하여 중복을 빠르게 확인
    vector<int> selectedCard;  //랜덤 생성 카드의 이미지를 순서대로 저장

    // 카드 배치
    for (int i = 0; i < randomCheck; i++) {
        // 각 카드에 대해 랜덤으로 배치할 구역 선택
        int randX = rand() % numRows;
        int randY = rand() % numCols;
        //cout << randX << "\t" << randY << endl;
        int retryCount = 0;
        //좌표 중복 검사, 중복일 경우 사용
        while (find(usedPositions.begin(),usedPositions.end(), make_pair(randX, randY))!= usedPositions.end()) {
            // 중복된 좌표가 발견되면 새로 생성
            randX = rand() % numRows;;
            randY = rand() % numCols;
            //cout << randX << "\t" << randY << endl;
            retryCount++;
            if (retryCount > 16) {
                cout << "Over 16time..." << endl;
                break;
            }
        }
        // 사용된 좌표 저장
        usedPositions.push_back(make_pair(randX, randY));
        // 카드의 위치 설정
        obj[i].setRect(randX*180, randY*180, imageSurface[0]->w, imageSurface[0]->h);
        selectedCard.push_back(cardNum[i]);   //사용된 카드 이미지 저장
        //cout << selectedCard.at(i) << endl;
        // 카드 렌더링
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
        // 화면에 그리기
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    //화면에 표시된 수 만큼의 딜레이 
    for (int i = 0; i < randomCheck; i++) {
        cout << i + 1 << "s" << endl;
        clear();
    }

    // 새로운 이미지 렌더링 (예시로 다른 이미지 사용)
    for (int i = 0; i < randomCheck; i++) {
        // 새로운 이미지 로드 및 렌더링
        imageSurface[i] = IMG_Load("images/character_0.png");  // 다른 이미지로 변경
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
            return 0;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }

    // 화면에 두 번째 렌더링 (새로운 이미지)
    SDL_RenderPresent(renderer);

    updateFirstGame(cardNum, cardImage, renderer, imageSurface, obj, selectedCard);
    return 1;
}

void updateFirstGame(int cardNum[], const char* cardImage[],SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, vector<int>selectedCard) {
    SDL_Event event;
    int quit = 0;
    int falseCnt = 0, maxFalseCnt = 10;
    int cnt = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                //printf("Mouse : (%d,%d)\n", event.button.x, event.button.y);
                //게임 조건 : falseCnt가 30회 보다 적어야함, 넘어가면 게임 실패
                if (cnt < selectedCard.size()&& falseCnt < maxFalseCnt) {
                    for (int i = 0; i < CARDSIZE; i++) {
                        SDL_Rect* rect = obj[i].getRect();  // 현재 객체의 위치와 크기
                        if (event.button.x >= rect->x && event.button.x <= rect->x + rect->w &&
                            event.button.y >= rect->y && event.button.y <= rect->y + rect->h) {
                            //printf("Card %d clicked\n", cardNum[i]); // 선택된 카드 번호 출력
                            if (cardNum[i] == selectedCard.at(cnt)) {
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " true" << endl;
                                orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);    //이미지 변경
                                cnt++;
                                if (cnt == selectedCard.size()) gameExit(quit, cnt);
                            }
                            else {
                                falseCnt++;
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " false" << endl;
                                cout << "false : " << falseCnt << " Time, Time Left : "<< maxFalseCnt - falseCnt << endl;
                                break;
                            }
                        }
                    }
                }
                else {
                    cout << "== Game Over ==" << endl;
                    cout << "ALL Open. See you Next Time." << endl;
                    for (int i = 0; i < CARDSIZE; i++) {
                        orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);
                    }
                    quit = 1;
                }
            }
        }
    }
}