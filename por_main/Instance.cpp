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
#define SIZE 6
#define monster_ai() (rand() % 4 + 1)   //몬스터의 행동 양식을 결정하는 매크로 함수 정의
#define spon() (rand() % 10+1)  //몬스터 생성 위치 랜덤

// 폰트와 텍스트 렌더링 함수
SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
		::cout << "Text Rendering Error: " << TTF_GetError() << endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);  // 서피스 메모리 해제
	return texture;
}

// 이미지 로드 함수
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer) {
	SDL_Surface* tempSurface = IMG_Load(filePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);  // 서페이스 메모리 해제
	return texture;
}

//// 테두리에 벽을 그리는 함수
//void drawWalls(SDL_Renderer* renderer) {
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 벽 색상을 흰색으로 설정
//
//	// 위쪽 벽
//	SDL_Rect topWall = { 0, 0, WIDTH, WALL_SIZE };
//	SDL_RenderFillRect(renderer, &topWall);
//
//	// 아래쪽 벽
//	SDL_Rect bottomWall = { 0, HEIGHT - WALL_SIZE, WIDTH, WALL_SIZE };
//	SDL_RenderFillRect(renderer, &bottomWall);
//
//	// 왼쪽 벽
//	SDL_Rect leftWall = { 0, 0, WALL_SIZE, HEIGHT };
//	SDL_RenderFillRect(renderer, &leftWall);
//
//	// 오른쪽 벽
//	SDL_Rect rightWall = { WIDTH - WALL_SIZE, 0, WALL_SIZE, HEIGHT };
//	SDL_RenderFillRect(renderer, &rightWall);
//}
