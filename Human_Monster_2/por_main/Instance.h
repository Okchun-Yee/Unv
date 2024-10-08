#pragma once
#define WIDTH 1271			
#define HEIGHT 721

//함수 원형 선언
void clear();
void field();

const int WALL_SIZE = 1;   // 벽 두께
const int RECT_SIZE = 50;  // 네모의 크기

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
	virtual void move(int pos) = 0;    //이동 후 x,y 를 새 위치로 변경
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
	void move(int pos)override {
		switch (pos)
		{
		case 1:	//좌
			if (x - distance >= WALL_SIZE) x += -distance;
			break;
		case 2:	//하
			if (y + distance <= HEIGHT - WALL_SIZE - RECT_SIZE) y += distance;
			break;
		case 3:	//상
			if (y - distance >= WALL_SIZE) y += -distance;
			break;
		case 4:	//우
			if (x + distance <= WIDTH - WALL_SIZE - RECT_SIZE) x += distance;
			break;
		default:
			break;
		}
	}
	char getShape() override {
		return 'H';
	}
	~Human() {};
};

class Monster :public Gameobject {
public:
	Monster(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
	void move(int pos)override {
		switch (pos)
		{
		case 1:	//좌
			if (x - distance >= WALL_SIZE) x += -distance * 2;
			break;
		case 2:	//하
			if (y + distance < HEIGHT - WALL_SIZE - RECT_SIZE) y += distance * 2;
			break;
		case 3:	//상
			if (y - distance >= WALL_SIZE) y += -distance * 2;
			break;
		case 4:	//우
			if (x + distance < WIDTH - WALL_SIZE - RECT_SIZE) x += distance * 2;
			break;
		default:
			break;
		}
	}
	char getShape() override {
		return 'M';
	}
	~Monster() {};
};

class Food :public Gameobject {
public:
	Food(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
	void move(int pos)override {
		switch (pos)
		{
		case 1:	//좌
			if (x - distance >= WALL_SIZE) x += -distance;
			break;
		case 2:	//하
			if (y + distance <= HEIGHT - WALL_SIZE - RECT_SIZE) y += distance;
			break;
		case 3:	//상
			if (y - distance >= WALL_SIZE) y += -distance;
			break;
		case 4:	//우
			if (x + distance <= WIDTH - WALL_SIZE - RECT_SIZE) x += distance;
			break;
		default:
			break;
		}
	}
	char getShape() override {
		return '@';
	}
	~Food() {};
};