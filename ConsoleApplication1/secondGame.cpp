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
    int selectedCard[CARDSIZE] = { 0 };   // �߰�
    int clickedCell[2] = { -1, -1 };   // �߰�
    srand(static_cast<unsigned int>(time(NULL)));  // ���� �õ� �� ���� ����

    // ������ ����� ũ�⸦ �����ϰ� ������, ���� ������ �Ұ����ϰ� ����
    for (int i = 0; i < CARDSIZE; i++) cardtemp[i] = rand() % 100;

    // ���� ũ���� ����
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

    // �̹��� �ҷ�����
    for (int i = 0; i < CARDSIZE; i++) {
        imageSurface[i] = IMG_Load(cardImage[cardNum[i]]);
        //cout << cardNum[i] << endl;
        //�̹����� ���� ��� err
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
            return 0;
        }
    }

    // �ؽ�ó ����
    for (int i = 0; i < CARDSIZE; i++) {
        //��ü�� �̹��� �����ϱ�
        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            obj[4 * i + j].setRect(j*imageSurface[0]->w + (j+1), i*imageSurface[0]->h + (j + 1), imageSurface[0]->w, imageSurface[0]->h);
        }
    }
    // ȭ�鿡 �׸���
    for (int i = 0; i < CARDSIZE; i++) {
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }
    SDL_RenderPresent(renderer);
    for (int i = 0; i < 10; i++) {
        cout << 10 - i << "�� �� ������ ���۵˴ϴ�." << endl;
        clear();
    }

    //ȭ�� ������
    for (int i = 0; i < CARDSIZE; i++) {
        // ���ο� �̹��� �ε� �� ������
        imageSurface[i] = IMG_Load("images/character_0.png");  // �ٸ� �̹����� ����
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
            return 0;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }
    //�ι�° ������
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
                // ù��° ī�带 ����������
                //printf("First %s\n", cardImage[card[clickedCell[0]]]);
                imageSurface[clickedCell[0]] = IMG_Load(cardImage[cardNum[clickedCell[0]]]);
                obj[clickedCell[0]].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[0]]));
                SDL_RenderCopy(renderer, obj[clickedCell[0]].getTexture(), NULL, obj[clickedCell[0]].getRect());
                SDL_RenderPresent(renderer);
            }
            else if (clickedCell[0] != -1 && clickedCell[1] != -1) {
                // �ι�° ī�带 ����������
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