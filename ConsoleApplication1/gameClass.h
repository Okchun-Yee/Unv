#pragma once
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>
#include <vector>

#define WIDTH 180     //720
#define HEIGHT 180    //720
#define CARDSIZE 16

class gameClass
{
    SDL_Rect rect;
    SDL_Texture* texture;
    int dx, dy;
public:
    //�⺻ ������
    gameClass()
        : rect{ 0, 0, 0, 0 }, dx(0), dy(0), texture(nullptr) {}

    // ������: �ʱ�ȭ
    gameClass(SDL_Rect r, int valX, int valY, SDL_Texture* tex)
        : rect(r), dx(valX), dy(valY), texture(tex) {}

    ~gameClass() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    void setRect(int x, int y, int w, int h);
    void setXY(int valX, int valY);
    void setTexture(SDL_Texture* tex);
    void updatePosition();

    SDL_Texture* getTexture() {
        return texture;
    }

    SDL_Rect* getRect() {
        return &rect;
    }
};

class thirdClass : public gameClass
{
    bool clickable;

public:
    // �⺻ ������
    thirdClass() : gameClass(), clickable(false) {}

    // ������: �ʱ�ȭ
    thirdClass(SDL_Rect r, int valX, int valY, SDL_Texture* tex, bool isClickable = false)
        : gameClass(r, valX, valY, tex), clickable(isClickable) {}

    // setClickable �Լ� �߰�
    void setClickable(bool isClickable) {
        clickable = isClickable;
    }

    // getClickable �Լ� �߰�
    bool isClickable() const {
        return clickable;
    }
};

//�Լ� ����
void startWindow(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface* imageSurface[], int windowSize);
int firstGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj);
int secondGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj);
int thirdGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj);
void updateFirstGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, std::vector<int>);
void updateSecondGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, int selectedCard[], int clickedCell[]);
void updateThridGame(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj, int, int[]);
void orignLoad(int cardNum, const char* cardImage, SDL_Renderer* renderer, SDL_Surface* imageSurface, gameClass& obj);
void gameExit(int& quit, int &cnt);
void clear();
int newImage(int index, int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], thirdClass* obj, int cardCheck[], int* newCol);