#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <windows.h>
#include <SDL_image.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <stdlib.h>
#include "Instance.h"

using namespace std;

#define row_SIZE 10
#define col_SIZE 20


int rectX = 0;  // 초기 X 위치
int rectY = 0;  // 초기 Y 위치

void clear() {
	Sleep(1000);
	system("cls");
}

void field() {
	int row = row_SIZE;
	int col = col_SIZE;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << "=";
		}
		cout << endl;
	}
	return;
}
// 테두리에 벽을 그리는 함수
void drawWalls(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 벽 색상을 흰색으로 설정

    // 위쪽 벽
    SDL_Rect topWall = { 0, 0, WIDTH, WALL_SIZE };
    SDL_RenderFillRect(renderer, &topWall);

    // 아래쪽 벽
    SDL_Rect bottomWall = { 0, HEIGHT - WALL_SIZE, WIDTH, WALL_SIZE };
    SDL_RenderFillRect(renderer, &bottomWall);

    // 왼쪽 벽
    SDL_Rect leftWall = { 0, 0, WALL_SIZE, HEIGHT };
    SDL_RenderFillRect(renderer, &leftWall);

    // 오른쪽 벽
    SDL_Rect rightWall = { WIDTH - WALL_SIZE, 0, WALL_SIZE, HEIGHT };
    SDL_RenderFillRect(renderer, &rightWall);
}

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Rect destRect;	//위치 지정(X축, Y축)

    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL Initialization Fail: %s\n", SDL_GetError();
        return 1;
    }

    // 윈도우 창 생성
    window = SDL_CreateWindow("Human, Monster", //윈도우 제목
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);

    //윈도우 생성 초기화 에러
    if (!window) {
        cout << "SDL Initialization Fail: %s\n", SDL_GetError();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Human human1(100, 100, RECT_SIZE);  //Human 객체 생성
    Monster monster1(400, 100, RECT_SIZE);  //Monster 객체 생성
    Monster monster2(200, 600, RECT_SIZE);  //Monster 객체 생성
    Food food1(1000, 500, RECT_SIZE);   //Food 객체 생성

    // 메시지 루프
    SDL_Event event;
    bool running = true;
    int ranMonster1 = 0;
    int ranMonster2 = 0;
    int randFood = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                ranMonster1 = rand() % 4 + 1;
                ranMonster2 = rand() % 4 + 1;
                randFood = rand() % 5;
                if (randFood == 3 || randFood == 0) {
                    food1.move(rand() % 4 + 1);
                }
                switch (event.key.keysym.sym)
                {
                case SDLK_a:
                    human1.move(1);
                    monster1.move(ranMonster1);
                    monster2.move(ranMonster2);
                    break;
                case SDLK_s:
                    human1.move(2);
                    monster1.move(ranMonster1);
                    monster2.move(ranMonster2);
                    break;
                case SDLK_d:
                    human1.move(3);
                    monster1.move(ranMonster1);
                    monster2.move(ranMonster2);
                    break;
                case SDLK_f:
                    human1.move(4);
                    monster1.move(ranMonster1);
                    monster2.move(ranMonster2);
                    break;
                default:
                    break;
                }
           }
        }
        // 화면 지우기
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 배경색을 검정으로 설정
        SDL_RenderClear(renderer);

        drawWalls(renderer);    //벽 그리기

        // 네모 그리기 - Human
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 네모 색상을 빨강으로 설정
        SDL_Rect rect = { human1.getX(), human1.getY(), RECT_SIZE, RECT_SIZE};
        SDL_RenderFillRect(renderer, &rect);
        // 네모 그리기 - Monster1
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // 네모 색상을 빨강으로 설정
        rect = { monster1.getX(), monster1.getY(), RECT_SIZE, RECT_SIZE };
        SDL_RenderFillRect(renderer, &rect);
        // 네모 그리기 - Monster2
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // 네모 색상을 빨강으로 설정
        rect = { monster2.getX(), monster2.getY(), RECT_SIZE, RECT_SIZE };
        SDL_RenderFillRect(renderer, &rect);
        // 네모 그리기 - Food
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // 네모 색상을 빨강으로 설정
        rect = { food1.getX(), food1.getY(), RECT_SIZE, RECT_SIZE };
        SDL_RenderFillRect(renderer, &rect);

        if (human1.collide(&monster1)) {
            cout << "Human과 Monster가 충돌했습니다!" << endl;
            cout << "Game Over." << endl;
            running = false;
        }
        if (monster1.collide(&food1)) {
            cout << "Monster와 Food가 충돌했습니다!" << endl;
            cout << "Game Over." << endl;
            running = false;
        }
        if (human1.collide(&food1)) {
            cout << "Human과 Food가 충돌했습니다!!!!!" << endl;
            cout << "Game Clear." << endl;
            running = false;
        }
        // 화면 업데이트
        SDL_RenderPresent(renderer);
    }

    // 종료
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Sleep(5000);

    return 0;
}