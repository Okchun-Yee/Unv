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
//콘솔창 지우기
void clear() {
	Sleep(1000);
	system("cls");
}

//필드를 그리는 함수
void field() {
	int row = row_SIZE;
	int col = col_SIZE;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			::cout << "=";
		}
		::cout << endl;
	}
	return;
}
//엔딩 출력
void end_credit(int movecount) {
	::cout << "총 이동 횟수 : " << movecount << endl;
}