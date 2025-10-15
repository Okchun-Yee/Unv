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

void gameClass::setRect(int x, int y, int w, int h) {
    //rect : ȭ�� ����
    this->rect = { x, y, w, h };
}
void gameClass::setXY(int valX, int valY) {
    this->dx = valX;
    this->dy = valY;
}
void gameClass::setTexture(SDL_Texture* tex) {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    this->texture = tex;
}
void gameClass::updatePosition() {
    rect.x += dx;
    rect.y += dy;
}

void clear() {
    Sleep(1000);
    system("cls");
}

void orignLoad(int cardNum, const char* cardImage, SDL_Renderer* renderer, SDL_Surface* imageSurface, gameClass& obj) {
    imageSurface = IMG_Load(cardImage);  // ���� �̹����� ��ü
    if (imageSurface == NULL) {
        cout << "Failed to load image " << endl;
        return;
    }
    obj.setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface));

    // ī�� ��ġ�� ���� �̹��� ������
    SDL_RenderCopy(renderer, obj.getTexture(), NULL, obj.getRect());
    SDL_RenderPresent(renderer);  // ���� �������� �̹��� ǥ��
}

void gameExit(int& quit, int& cnt) {
    cout << "== WIN ==" << endl;

    SDL_Delay(1000);
    quit = 1;
    cnt = 0;  // ���� �� cnt �ʱ�ȭ
}

int firstGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    cout << "== Game Start ==" << endl;
    srand(static_cast<unsigned int>(time(NULL)));  // ���� �õ� �� ���� ����

    for (int i = 0; i < CARDSIZE; i++) {
        cardNum[i] = i;
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

    // �����ϰ� ��ġ�� ī�� �� (3~7��)
    int randomCheck = 8 + rand() % 3; // 8~12 �� ���� ����

    // ȭ���� 4��� (2x2)���� ������ �� ������ �����ϰ� ī�� ��ġ
    int numRows = 4; // �� ��
    int numCols = 4; // �� ��
    vector<pair<int, int>> usedPositions;  // ��ǥ�� set���� �����Ͽ� �ߺ��� ������ Ȯ��
    vector<int> selectedCard;  //���� ���� ī���� �̹����� ������� ����

    // ī�� ��ġ
    for (int i = 0; i < randomCheck; i++) {
        // �� ī�忡 ���� �������� ��ġ�� ���� ����
        int randX = rand() % numRows;
        int randY = rand() % numCols;
        //cout << randX << "\t" << randY << endl;
        int retryCount = 0;
        //��ǥ �ߺ� �˻�, �ߺ��� ��� ���
        while (find(usedPositions.begin(),usedPositions.end(), make_pair(randX, randY))!= usedPositions.end()) {
            // �ߺ��� ��ǥ�� �߰ߵǸ� ���� ����
            randX = rand() % numRows;;
            randY = rand() % numCols;
            //cout << randX << "\t" << randY << endl;
            retryCount++;
            if (retryCount > 16) {
                cout << "Over 16time..." << endl;
                break;
            }
        }
        // ���� ��ǥ ����
        usedPositions.push_back(make_pair(randX, randY));
        // ī���� ��ġ ����
        obj[i].setRect(randX*180, randY*180, imageSurface[0]->w, imageSurface[0]->h);
        selectedCard.push_back(cardNum[i]);   //���� ī�� �̹��� ����
        //cout << selectedCard.at(i) << endl;
        // ī�� ������
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
        // ȭ�鿡 �׸���
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    //ȭ�鿡 ǥ�õ� �� ��ŭ�� ������ 
    for (int i = 0; i < randomCheck; i++) {
        cout << i + 1 << "s" << endl;
        clear();
    }

    // ���ο� �̹��� ������ (���÷� �ٸ� �̹��� ���)
    for (int i = 0; i < randomCheck; i++) {
        // ���ο� �̹��� �ε� �� ������
        imageSurface[i] = IMG_Load("images/character_0.png");  // �ٸ� �̹����� ����
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
            return 0;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }

    // ȭ�鿡 �� ��° ������ (���ο� �̹���)
    SDL_RenderPresent(renderer);

    updateFirstGame(cardNum, cardImage, renderer, imageSurface, obj, selectedCard);
    return 1;
}

void updateFirstGame(int cardNum[], const char* cardImage[],SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, vector<int>selectedCard) {
    SDL_Event event;
    int quit = 0;
    int falseCnt = 0, maxFalseCnt = 10;
    int cnt = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                //printf("Mouse : (%d,%d)\n", event.button.x, event.button.y);
                //���� ���� : falseCnt�� 30ȸ ���� �������, �Ѿ�� ���� ����
                if (cnt < selectedCard.size()&& falseCnt < maxFalseCnt) {
                    for (int i = 0; i < CARDSIZE; i++) {
                        SDL_Rect* rect = obj[i].getRect();  // ���� ��ü�� ��ġ�� ũ��
                        if (event.button.x >= rect->x && event.button.x <= rect->x + rect->w &&
                            event.button.y >= rect->y && event.button.y <= rect->y + rect->h) {
                            //printf("Card %d clicked\n", cardNum[i]); // ���õ� ī�� ��ȣ ���
                            if (cardNum[i] == selectedCard.at(cnt)) {
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " true" << endl;
                                orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);    //�̹��� ����
                                cnt++;
                                if (cnt == selectedCard.size()) gameExit(quit, cnt);
                            }
                            else {
                                falseCnt++;
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " false" << endl;
                                cout << "false : " << falseCnt << " Time, Time Left : "<< maxFalseCnt - falseCnt << endl;
                                break;
                            }
                        }
                    }
                }
                else {
                    cout << "== Game Over ==" << endl;
                    cout << "ALL Open. See you Next Time." << endl;
                    for (int i = 0; i < CARDSIZE; i++) {
                        orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);
                    }
                    quit = 1;
                }
            }
        }
    }
}