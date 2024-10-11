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
#define monster_ai() (rand() % 4 + 1)   //������ �ൿ ����� �����ϴ� ��ũ�� �Լ� ����
#define spon() (rand() % 10+1)  //���� ���� ��ġ ����

// ��Ʈ�� �ؽ�Ʈ ������ �Լ�
SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
		::cout << "Text Rendering Error: " << TTF_GetError() << endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);  // ���ǽ� �޸� ����
	return texture;
}

// �̹��� �ε� �Լ�
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer) {
	SDL_Surface* tempSurface = IMG_Load(filePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);  // �����̽� �޸� ����
	return texture;
}

//// �׵θ��� ���� �׸��� �Լ�
//void drawWalls(SDL_Renderer* renderer) {
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // �� ������ ������� ����
//
//	// ���� ��
//	SDL_Rect topWall = { 0, 0, WIDTH, WALL_SIZE };
//	SDL_RenderFillRect(renderer, &topWall);
//
//	// �Ʒ��� ��
//	SDL_Rect bottomWall = { 0, HEIGHT - WALL_SIZE, WIDTH, WALL_SIZE };
//	SDL_RenderFillRect(renderer, &bottomWall);
//
//	// ���� ��
//	SDL_Rect leftWall = { 0, 0, WALL_SIZE, HEIGHT };
//	SDL_RenderFillRect(renderer, &leftWall);
//
//	// ������ ��
//	SDL_Rect rightWall = { WIDTH - WALL_SIZE, 0, WALL_SIZE, HEIGHT };
//	SDL_RenderFillRect(renderer, &rightWall);
//}
