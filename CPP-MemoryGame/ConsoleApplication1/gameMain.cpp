#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_messagebox.h>
#include <string>
#include <Windows.h>
#include "gameClass.h"

using namespace std;

void startWindow(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface* imageSurface[],int windowSize) {
	// 윈도우 창 생성
	window = SDL_CreateWindow("SDL Memory Game",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH * windowSize, HEIGHT * 4,
		SDL_WINDOW_SHOWN);
	//윈도우 초기화
	if (!window) {
		cout << "SDL Initialization Fail: " << SDL_GetError() << endl;
		SDL_Quit();
		return;
	}

	// SDL 초기화
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL Initialization Fail: " << SDL_GetError() << endl;
		return;
	}
	//렌더러 초기화
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		cout << "Renderer Creation Fail: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
}

int main(int argc, char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* imageSurface[CARDSIZE];
	gameClass* Card4Memory = new gameClass[CARDSIZE];
	thirdClass* blackJackGame = new thirdClass[CARDSIZE];
	const char* cardImage[CARDSIZE] = {
		"images/character_1.png", "images/character_2.png", "images/character_3.png", "images/character_4.png", "images/character_5.png",
		"images/character_6.png", "images/character_7.png", "images/character_8.png", "images/character_9.png", "images/character_10.png",
		"images/character_11.png", "images/character_12.png", "images/character_13.png", "images/character_14.png","images/character_15.png", "images/character_16.png"
	};
	int cardNum[CARDSIZE] = { 0 };
	int gameCheck = 999;
	cout
		<< "게임을 선택하여 주십시요." << endl
		<< "1번 : 위치 기억 게임" << endl
		<< "2번 : 짝 찾기 게임" << endl
		<< "3번 : 블루 잭 게임" << endl
		<< "게임 선택 : ";
	while (gameCheck > 3) {
		cin >> gameCheck;
		if (gameCheck <= 3) cout << gameCheck << "번 게임이 선택 되었습니다." << endl;
		else cout << "다시 입력하여 주십시요 : ";
	}
	switch (gameCheck) {
	case 1:
		cout << "위치 기억 게임이 시작됩니다.";
		Sleep(500);
		startWindow(window, renderer, imageSurface,4);
		firstGame(cardNum, cardImage, renderer, imageSurface, Card4Memory);
		break;
	case 2:
		cout << "짝 찾기 게임이 시작됩니다." << endl;
		Sleep(500);
		startWindow(window, renderer, imageSurface,4);
		secondGame(cardNum, cardImage, renderer, imageSurface, Card4Memory);
		break;
	case 3:
		cout << "블루잭 게임이 시작됩니다." << endl;
		Sleep(500);
		startWindow(window, renderer, imageSurface, 5);
		thirdGame(cardNum, cardImage, renderer, imageSurface, blackJackGame);
		break;
	default:
		cout << "Err" << endl;
		break;
	}

	// 화면 업데이트 (렌더링 내용 표시)
	if (renderer != NULL) {
		SDL_RenderPresent(renderer);
	}

	//SDL_Delay(60000);

	// 리소스 해제
	if (window != NULL) {
		for (int i = 0; i < CARDSIZE; i++) {
			//cout << "Free : " << i << endl;
			if (imageSurface[i] != NULL) {
				SDL_FreeSurface(imageSurface[i]);
			}
		}
		// 종료
		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	delete[] Card4Memory;
	return 0;
}
