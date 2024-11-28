#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_messagebox.h>
#include <string>
#include "gameClass.h"

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;

    gameClass* Card4Memory = new gameClass[CARDSIZE];

    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL Initialization Fail: " << SDL_GetError() << endl;
        return 1;
    }

    // 윈도우 창 생성
    window = SDL_CreateWindow("SDL Memory Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window) {
        cout << "SDL Initialization Fail: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = NULL;
    SDL_Surface* imageSurface[CARDSIZE];
    renderer = SDL_CreateRenderer(window, -1, 0);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    const char* cardImage[CARDSIZE] = {
        "images/character_1.png", "images/character_2.png", "images/character_3.png", "images/character_4.png", "images/character_5.png",
        "images/character_6.png", "images/character_7.png", "images/character_8.png", "images/character_9.png", "images/character_10.png",
        "images/character_11.png", "images/character_12.png", "images/character_13.png", "images/character_14.png",
    };
    int cardNum[CARDSIZE] = { 0 };
    for (int i = 0; i < CARDSIZE; i++) {
        cardNum[i] = i;
    }
    if (gameBase(cardNum, cardImage, renderer, imageSurface, Card4Memory)) {
        cout << "== Game end ==" << endl;
    }

    // 리소스 해제
    for (int i = 0; i < CARDSIZE; i++) {
        SDL_FreeSurface(imageSurface[i]);
    }

    // 종료
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete[] Card4Memory;
    return 0;
}
