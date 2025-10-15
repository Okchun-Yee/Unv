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

int secondGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    cout << "== Game Start ==" << endl;
    for (int i = 0; i < CARDSIZE; i+=2) {
        cardNum[i] = i;
        cardNum[i + 1] = i;
    }
    int cardtemp[CARDSIZE];
    int selectedCard[CARDSIZE] = { 0 };   // 추가
    int clickedCell[2] = { -1, -1 };   // 추가
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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            obj[4 * i + j].setRect(j*imageSurface[0]->w + (j+1), i*imageSurface[0]->h + (j + 1), imageSurface[0]->w, imageSurface[0]->h);
        }
    }
    // 화면에 그리기
    for (int i = 0; i < CARDSIZE; i++) {
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }
    SDL_RenderPresent(renderer);
    for (int i = 0; i < 10; i++) {
        cout << 10 - i << "초 후 게임이 시작됩니다." << endl;
        clear();
    }

    //화면 가리기
    for (int i = 0; i < CARDSIZE; i++) {
        // 새로운 이미지 로드 및 렌더링
        imageSurface[i] = IMG_Load("images/character_0.png");  // 다른 이미지로 변경
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
            return 0;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }
    //두번째 렌더링
    SDL_RenderPresent(renderer);
    updateSecondGame(cardNum, cardImage, renderer, imageSurface, obj, selectedCard,clickedCell);
    return 0;
}

void updateSecondGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, int selectedCard[], int clickedCell[]) {
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                //cout << event.button.x << " : " << event.button.y << endl;
                for (int i = 0; i < CARDSIZE; ++i) {
                    int row = i / 4;
                    int col = i % 4;
                    if ((event.button.x >= (col * imageSurface[0]->w + col + 1) && event.button.x <= ((col + 1) * imageSurface[0]->w + col + 1)) &&
                        (event.button.y >= (row * imageSurface[0]->h + row + 1) && event.button.y <= ((row + 1) * imageSurface[0]->h + row + 1))) {
                        if (!selectedCard[i]) {
                            if (clickedCell[0] == -1) clickedCell[0] = i;
                            else {
                                if (clickedCell[0] != i) clickedCell[1] = i;
                            }
                        }
                    }
                }
            }
            if (clickedCell[0] != -1 && clickedCell[1] == -1) {
                // 첫번째 카드를 선택했을때
                //printf("First %s\n", cardImage[card[clickedCell[0]]]);
                imageSurface[clickedCell[0]] = IMG_Load(cardImage[cardNum[clickedCell[0]]]);
                obj[clickedCell[0]].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[0]]));
                SDL_RenderCopy(renderer, obj[clickedCell[0]].getTexture(), NULL, obj[clickedCell[0]].getRect());
                SDL_RenderPresent(renderer);
            }
            else if (clickedCell[0] != -1 && clickedCell[1] != -1) {
                // 두번째 카드를 선택했을때
                //printf("Second %s\n", cardImage[card[clickedCell[1]]]);
                imageSurface[clickedCell[1]] = IMG_Load(cardImage[cardNum[clickedCell[1]]]);
                obj[clickedCell[1]].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[1]]));
                SDL_RenderCopy(renderer, obj[clickedCell[1]].getTexture(), NULL, obj[clickedCell[1]].getRect());
                SDL_RenderPresent(renderer);

                if (cardNum[clickedCell[0]] == cardNum[clickedCell[1]]) {
                    //printf("Same\n");
                    //printf("%d, %d\n", clickedCell[0], clickedCell[1]);
                    selectedCard[clickedCell[0]] = 1;
                    selectedCard[clickedCell[1]] = 1;

                    clickedCell[0] = clickedCell[1] = -1;

                    int sum = 0;
                    for (int i = 0; i < CARDSIZE; i++) sum += selectedCard[i];
                    if (sum == CARDSIZE) {
                        cout << "== Game Clear ==";
                        printf(" Success = %d\n", sum);
                        quit = true;
                    }
                }
                else {
                    //printf("Difference\n");
                    SDL_Delay(1000);

                    imageSurface[clickedCell[0]] = IMG_Load("images/character_0.png");
                    obj[clickedCell[0]].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[0]]));
                    SDL_RenderCopy(renderer, obj[clickedCell[0]].getTexture(), NULL, obj[clickedCell[0]].getRect());
                    SDL_RenderPresent(renderer);

                    imageSurface[clickedCell[1]] = IMG_Load("images/character_0.png");
                    obj[clickedCell[1]].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[1]]));
                    SDL_RenderCopy(renderer, obj[clickedCell[1]].getTexture(), NULL, obj[clickedCell[1]].getRect());
                    SDL_RenderPresent(renderer);

                    clickedCell[0] = clickedCell[1] = -1;
                }
            }
        }
    }
    for (int i = 0; i < CARDSIZE; i++) SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    SDL_RenderPresent(renderer);

}