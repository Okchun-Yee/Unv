#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <windows.h>
#include <string>
#include "Instance.h"

using namespace std;

#define row_SIZE 10
#define col_SIZE 20
#define SIZE 10
#define monster_ai() (rand() % 4 + 1)   //몬스터의 행동 양식을 결정하는 매크로 함수 정의
#define spon() (rand() % 10+1)  //몬스터 생성 위치 랜덤

int rectX = 0;  // 초기 X 위치
int rectY = 0;  // 초기 Y 위치



int main(int argc, char* argv[])
{
	SDL_Window* window = nullptr;
	clock_t start = clock();
	SDL_Renderer* renderer;	//렌더러 할당
	srand(static_cast<unsigned int>(time(NULL)));  //난수 초기화

	// SDL 초기화
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		::cout << "SDL ERR " << SDL_GetError();
		return 1;
	}
	//SDL 이미지 초기화
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		::cout << IMG_GetError() << endl;
		return 1;
	}
	//TTF 초기화
	if (TTF_Init() == -1) {
		::cout << "TTF_ERR" << TTF_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	TTF_Font* pFont = TTF_OpenFont("Icon/OpenSans-Bold.ttf", 16); // 적절한 폰트와 크기 선택
	if (!pFont) {
		::cout << "Font Load Fail: " << TTF_GetError() << endl;
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
		::cout << "SDL Initialization Fail: %s\n", SDL_GetError();
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);  //렌더러 할당

	Human human1(0, 0, RECT_SIZE);  //Human 객체 생성
	Monster monster[SIZE];  //Monster 객체 생성
	Food food1(1000, 500, RECT_SIZE);   //Food 객체 생성

	SDL_Texture* humanTexture = loadTexture("Icon/matter.png", renderer); // human이미지 파일 경로
	if (!humanTexture) {
		cout << "Failed to load human texture." << endl;
		// humanTexture가 nullptr일 경우 처리
	}
	SDL_Texture* monsterTexture = loadTexture("Icon/monster.png", renderer); // monster이미지 파일 경로
	if (!monsterTexture) {
		cout << "Failed to load monster texture." << endl;
		// monsterTexture가 nullptr일 경우 처리
	}
	SDL_Texture* foodTexture = loadTexture("Icon/Food.png", renderer); // food.png 파일 경로

	// 메시지 루프
	SDL_Event event;
	int running = 0;
	int randFood = 0;
	int movecount = 0;
	int cnt = 0;
	bool check = false;
	while (!running) {
		if ( movecount % 10 == 0 && cnt < SIZE) {
			// 조건이 만족될 때만 실행
			if (!check) {
				check = true;  // 조건을 만족했음을 기록
				monster[cnt].setPos(spon()*100, monster_ai()*100, RECT_SIZE);    //10회 마다 새로운 몬스터 추가 : 최대 10번	
				//X : 100 ~ 1000, Y : 100 ~ 400 랜덤하게 생성
				cnt++;
			}
		}
		// SDL 이벤트 처리
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 1;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) running = 1;

				randFood = rand() % 5;
				if (randFood == 3 || randFood == 0) {
					food1.move(rand() % 4 + 1);
				}
				switch (event.key.keysym.sym) {
				case SDLK_a:
				case SDLK_LEFT:
					movecount++;
					human1.move(1);
					check = false;
					break;
				case SDLK_s:
				case SDLK_DOWN:
					movecount++;
					human1.move(2);
					check = false;
					break;
				case SDLK_w:
				case SDLK_UP:
					movecount++;
					human1.move(3);
					check = false;
					break;
				case SDLK_d:
				case SDLK_RIGHT:
					movecount++;
					human1.move(4);
					check = false;
					break;
				default:
					break;
				}
				// 몬스터 이동
				for (int i = 0; i < cnt; i++) {
					monster[i].move(monster_ai());
				}
			}
		}
		// 화면 지우기
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 배경색을 흰색으로  설정
		SDL_RenderClear(renderer);
		//drawWalls(renderer);    //벽 그리기

		//이미지 삽입 - Human
		SDL_Rect humanRect = { human1.getX(), human1.getY(), RECT_SIZE, RECT_SIZE };
		SDL_RenderCopy(renderer, humanTexture, NULL, &humanRect);
		//::cout << "사람 소환" << cnt << endl;

		// 네모 그리기 - Food
		//::cout <<"food" << food1.getX() << " " << food1.getY() << endl;
		SDL_Rect foodRect = { food1.getX(), food1.getY(), RECT_SIZE, RECT_SIZE };
		SDL_RenderCopy(renderer, foodTexture, NULL, &foodRect);

		//이미지 삽입 - Monster
		for (int i = 0; i < cnt; i++) {
			//::cout << "monster" << monster[i].getX()<<" "<< monster[i].getY() << endl;
			SDL_Rect monsterRect = { monster[i].getX(), monster[i].getY(), RECT_SIZE, RECT_SIZE };
			SDL_RenderCopy(renderer, monsterTexture, NULL, &monsterRect);
		}

		// 충돌 검사
		for (int i = 0; i < cnt; i++) {
			if (human1.collide(&monster[i])) {
				::cout << "Human과 Monster["<< i << "]가 충돌했습니다!" << endl;
				end_credit(movecount);
				::cout << "Game Over." << endl;
				running = 1;
				break;
			}
			if (monster[i].collide(&food1)) {
				::cout << "Monster["<<i<<"]와 Food가 충돌했습니다!" << endl;
				end_credit(movecount);
				::cout << "Game Over." << endl;
				running = 1;
				break;
			}
			//else ::cout << "pass" << endl;
		}
		if (human1.collide(&food1)) {
			::cout << "Human과 Food가 충돌했습니다!!!!!" << endl;
			end_credit(movecount);
			::cout << "Game Clear." << endl;
			running = 1;
		}

		clock_t now = clock();
		int elapsed_time = (int)(now - start) / CLOCKS_PER_SEC;
		if (elapsed_time >= 30.0) {
			end_credit(movecount);
			cout << "시간 초과, Game Over!.\n";
			running = 1;
		}

		string text = "Move Count: " + to_string(movecount);		//이동 횟수 카운트
		string time_text = "Time : " + to_string(elapsed_time);		//현재 진행중인 초 

		// 메시지 루프 안에 텍스트 렌더링 코드 추가
		SDL_Color textColor = { 0, 0, 0 }; // 검정색
		SDL_Texture* textTexture = renderText(text, pFont, textColor, renderer);
		if (textTexture) {
			SDL_Rect textRect = { 0, 0, 160, 60 }; // 위치와 크기 설정
			SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
			SDL_DestroyTexture(textTexture); // 텍스처 해제

			textTexture = renderText(time_text, pFont, textColor, renderer);
			textRect = { 1040, 0, 160, 60 }; // 위치와 크기 설정
			SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
			SDL_DestroyTexture(textTexture); // 텍스처 해제
		}

		// 화면 업데이트
		SDL_RenderPresent(renderer);
	}

	// 종료
	if (humanTexture != nullptr) {
		SDL_DestroyTexture(humanTexture);
	}
	if (monsterTexture != nullptr) {
		SDL_DestroyTexture(monsterTexture);
	}
	if (foodTexture != nullptr) {
		SDL_DestroyTexture(foodTexture);
	}
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	if (pFont != nullptr) {
		TTF_CloseFont(pFont);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
