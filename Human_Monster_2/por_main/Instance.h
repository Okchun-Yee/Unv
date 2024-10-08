#pragma once
#define WIDTH 1271			
#define HEIGHT 721

//�Լ� ���� ����
void clear();
void field();

const int WALL_SIZE = 1;   // �� �β�
const int RECT_SIZE = 50;  // �׸��� ũ��

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
	virtual void move(int pos) = 0;    //�̵� �� x,y �� �� ��ġ�� ����
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
	void move(int pos)override {
		switch (pos)
		{
		case 1:	//��
			if (x - distance >= WALL_SIZE) x += -distance;
			break;
		case 2:	//��
			if (y + distance <= HEIGHT - WALL_SIZE - RECT_SIZE) y += distance;
			break;
		case 3:	//��
			if (y - distance >= WALL_SIZE) y += -distance;
			break;
		case 4:	//��
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
		case 1:	//��
			if (x - distance >= WALL_SIZE) x += -distance * 2;
			break;
		case 2:	//��
			if (y + distance < HEIGHT - WALL_SIZE - RECT_SIZE) y += distance * 2;
			break;
		case 3:	//��
			if (y - distance >= WALL_SIZE) y += -distance * 2;
			break;
		case 4:	//��
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
		case 1:	//��
			if (x - distance >= WALL_SIZE) x += -distance;
			break;
		case 2:	//��
			if (y + distance <= HEIGHT - WALL_SIZE - RECT_SIZE) y += distance;
			break;
		case 3:	//��
			if (y - distance >= WALL_SIZE) y += -distance;
			break;
		case 4:	//��
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