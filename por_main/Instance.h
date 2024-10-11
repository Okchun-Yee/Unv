#pragma once
#define WIDTH 1200		
#define HEIGHT 600
#define RECT_SIZE 50  // �׸��� ũ�� 50px


//�Լ� ���� ����
void clear();
void field();
//�Լ� ���� �����
SDL_Texture* renderText(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);	//��Ʈ ��� �Լ�
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);	//�̹��� ��� �Լ�
//void drawWalls(SDL_Renderer* renderer);



class Gameobject {
protected:
	int distance;   //�ѹ� �̵��Ÿ�
	int x, y;   //������ġ 
public:
	Gameobject(int startX, int startY, int distance) {  //�ʱ� ��ġ�� �̵� �Ÿ�
		this->x = startX;
		this->y = startY;
		this->distance = distance;
	}
	virtual ~Gameobject() {};   //�Ҹ���
	void move(int pos) {
		if (pos == 1) {  // �������� �̵�
			this->x = (this->x - this->distance >= 0) ? this->x - this->distance : this->x + this->distance;
		}
		else if (pos == 2) {  // �Ʒ��� �̵�
			this->y = (this->y + this->distance <= HEIGHT - 50) ? this->y + this->distance : this->y - this->distance;
		}
		else if (pos == 3) {  // ���� �̵�
			this->y = (this->y - this->distance >= 0) ? this->y - this->distance : this->y + this->distance;
		}
		else if (pos == 4) {  // ���������� �̵�
			this->x = (this->x + this->distance <= WIDTH - 50) ? this->x + this->distance : this->x - this->distance;
		}
	}


	//�̵� �� x,y �� �� ��ġ�� ����
	virtual char getShape() = 0;    //��ü�� ����� ��Ÿ���� ���� ->Human = H / Monster = M / Food = @

	//�浹 �Ǻ�
	int getX() { return x; }    //x ��ġ ��ȯ (�ڽ�, ���)
	int getY() { return y; }    //y ��ġ ��ȣ�� (�ڽ�, ���)
	bool collide(Gameobject* p) {   //*p �� ���
		if (this->x == p->getX() && this->y == p->getY()) return true;  //�ڽŰ� ����� �浹Ȯ��
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
	Monster() : Gameobject(100, 100, 0) {}	//�⺻������

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
