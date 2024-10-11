#pragma once
#define WIDTH 1200		
#define HEIGHT 600
#define RECT_SIZE 50  // 네모의 크기 50px


//함수 원형 선언
void clear();
void field();
//함수 원형 선언부
SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);	//폰트 사용 함수
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);	//이미지 사용 함수
//void drawWalls(SDL_Renderer* renderer);



class Gameobject {
protected:
	int distance;   //한번 이동거리
	int x, y;   //현재위치 
public:
	Gameobject(int startX, int startY, int distance) {  //초기 위치와 이동 거리
		this->x = startX;
		this->y = startY;
		this->distance = distance;
	}
	virtual ~Gameobject() {};   //소멸자
	void move(int pos) {
		if (pos == 1) {  // 왼쪽으로 이동
			this->x = (this->x - this->distance >= 0) ? this->x - this->distance : this->x + this->distance;
		}
		else if (pos == 2) {  // 아래로 이동
			this->y = (this->y + this->distance <= HEIGHT - 50) ? this->y + this->distance : this->y - this->distance;
		}
		else if (pos == 3) {  // 위로 이동
			this->y = (this->y - this->distance >= 0) ? this->y - this->distance : this->y + this->distance;
		}
		else if (pos == 4) {  // 오른쪽으로 이동
			this->x = (this->x + this->distance <= WIDTH - 50) ? this->x + this->distance : this->x - this->distance;
		}
	}


	//이동 후 x,y 를 새 위치로 변경
	virtual char getShape() = 0;    //객체의 모양을 나타내는 문자 ->Human = H / Monster = M / Food = @

	//충돌 판별
	int getX() { return x; }    //x 위치 반환 (자신, 상대)
	int getY() { return y; }    //y 위치 반호나 (자신, 상대)
	bool collide(Gameobject* p) {   //*p 는 상대
		if (this->x == p->getX() && this->y == p->getY()) return true;  //자신과 상대의 충돌확인
		else return false;
	}
};

class Human :public Gameobject {
public:
	Human(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
	char getShape() override {
		return 'H';
	}
	~Human() {};
};

class Monster :public Gameobject {
public:
	Monster(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
	Monster() : Gameobject(100, 100, 0) {}	//기본생성자

	void setPos(int newX, int newY, int newDistance) {
		x = newX;
		y = newY;
		distance = newDistance;
	}
	char getShape() override {
		return 'M';
	}
	~Monster() {};
};

class Food :public Gameobject {
public:
	Food(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
	char getShape() override {
		return '@';
	}
	~Food() {};
};
