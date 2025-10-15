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

int thirdGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj) {
	cout << "== Game Start ==" << endl;
	for (int i = 0; i < CARDSIZE; i++) {
		cardNum[i] = i;
	}
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
	int blackJack = 21 + rand() % 15;   //10 ~ 54 까지의 랜덤 값 생성
	/*cout
		<< "목표 점수를 설정해 주세요." << endl
		<< "최솟 값 : 10" << endl
		<< "최댓 값 : 54" << endl
		<< "목표 점수 : ";
	while ((blackJack < 10)||(blackJack>54)) {
		cin >> blackJack;
		if ((blackJack >= 10) && (blackJack <= 54)) {
			cout << "Goal : " << blackJack << endl;
			break;
		}
		else cout << "다시 입력해 주세요 : ";
	}*/

	// 이미지 불러오기 (필요한 5개만 로드)
	for (int i = 0; i < CARDSIZE; i++) {
		imageSurface[i] = IMG_Load(cardImage[cardNum[i]]);
		//cout << cardNum[i] << endl;
		//이미지가 없는 경우 err
		if (imageSurface[i] == NULL) {
			cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
			return 0;
		}
	}
	// 텍스처 생성 (필요한 5개만 생성)
	for (int i = 0; i < 5; i++) {
		//객체에 이미지 지정하기
		obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
	}
	for (int i = 0; i < 5; i++) {
		obj[i].setRect(imageSurface[0]->w * i, imageSurface[0]->w * 3, imageSurface[0]->w, imageSurface[0]->h);
	}
	cout << "내 카드 : ";
	int cardCheck[5] = { 0 };
	// 화면에 그리기
	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
		//cout << cardNum[i] + 1 << " ";
		cardCheck[i] = cardNum[i];

	}
	cout << endl;
	SDL_RenderPresent(renderer);

	updateThridGame(cardNum, cardImage, renderer, imageSurface, obj, blackJack, cardCheck);

	return 1;
}
#define maxCnt 4
void updateThridGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj,int goal, int cardCheck[]) {
	SDL_Event event;
	int quit = 0;
	int newCol = 0;
	int gameCheck = 0, gameCnt = 0;
	cout
		<< "----------------------------------------------------------------------------------------------------------\n"
		<< "Goal :\t" << goal << endl;
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
				for (int i = 0; i < 5; ++i) {
					SDL_Rect* rect = obj[i].getRect();  // 현재 객체의 위치와 크기
					// 실제 렌더링된 rect의 위치와 클릭 좌표를 비교
					if (event.button.x >= rect->x && event.button.x <= (rect->x + rect->w) &&
						event.button.y >= rect->y && event.button.y <= (rect->y + rect->h)) {
						if (imageSurface[i] != nullptr&&obj[i].isClickable() == false) {
							obj[i].setClickable(true);
							//cout << "Card : " << cardNum[i] + 1 << endl;
							// 원래 이미지 지우기
							SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 배경색 (검은색)으로 설정
							SDL_RenderFillRect(renderer, rect);
							// 이미지의 좌표를 2칸 위로 이동, 열은 1열부터 채우는 방식
							obj[i].setRect(newCol * rect->w, rect->h - 2, rect->w, rect->h);
							newCol++;
							// 화면에 다시 그리기
							SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
							SDL_RenderPresent(renderer);
							gameCheck = gameCheck + cardNum[i] + 1 + newImage(i, cardNum, cardImage, renderer, imageSurface, obj, cardCheck, &newCol);
							gameCnt+=2;
						}
						else {
							cout << "False" << endl;
						}
					}
					if (gameCnt == maxCnt) {
						cout << "----------------------------------------------------------------------------------------------------------\n";
						cout << gameCheck << "\tgameFinish" << endl;
						if (gameCheck > goal) {
							cout << "Goal :\t" << goal << "\tScore :\t" << gameCheck << " YOU LOSE" << endl;
							quit = true;
						}
						else if (gameCheck < goal) {
							cout << "Goal :\t" << goal << "\tScore :\t" << gameCheck << " YOU WIN" << endl;
							quit = true;
						}
						else {
							cout << "Goal :\t" << goal << "\tScore :\t" << gameCheck << " IS WONDERFUL!!!!!!!!!!!" << endl;
							quit = true;
						}
						break;
					}
				}
			}
		}
	}
}

int newImage(int index, int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj, int cardCheck[], int* newCol) {
	vector<int>num;
	int randomValue = 0;
	for (int i = 0; i < CARDSIZE; i++) {
		num.push_back(i);
	}
	for (int i = 0; i < 5; ++i) {
		num.erase(remove(num.begin(), num.end(), cardCheck[i]), num.end());
	}
	if (!num.empty()) {
		int randomIndex = rand() % num.size();
		randomValue = num[randomIndex];
	}

	//새로운 이미지 생성
	SDL_Surface* newImageSurface = IMG_Load(cardImage[randomValue]);
	if (newImageSurface == NULL) {
		cout << "Failed to load image " << cardImage[randomValue] << ": " << IMG_GetError() << endl;
		return 0;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, newImageSurface);
	SDL_FreeSurface(newImageSurface);
	//새로운 이미지 배치
	int newX = obj[index].getRect()->w * *newCol;
	int newY = obj[index].getRect()->y;
	//cout << newX << " " << newY << " " << index << endl;
	obj[index + 5].setRect(newX, newY, obj[index].getRect()->w, obj[index].getRect()->h);
	obj[index + 5].setTexture(newTexture);

	Sleep(1000);
	// Render the new image
	SDL_RenderCopy(renderer, obj[index + 5].getTexture(), NULL, obj[index + 5].getRect());
	SDL_RenderPresent(renderer);

	// 새로 생성된 이미지는 클릭할 수 없도록 설정
	obj[index + 5].setClickable(true);
	*newCol += 1;
	return randomValue;
}

