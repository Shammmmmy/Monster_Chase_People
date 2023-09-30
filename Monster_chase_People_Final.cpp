#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>

#define MapTime 300
#define WIDTH 95
#define HEIGHT 25
#define TRUE 1
#define FALSE 0
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define Eat_Props 200
#define Eat_Monster 500
#define UPWALL 0
#define DOWNWALL 1
#define LEFTWALL 2
#define RIGHTWALL 3
#define PlayerS 0.1
#define DoublePlayerS 0.05
#define MonsterS 0.5
#define SlowMonsterS 1
#define GodModG 71
#define GodModg 103
#define Died 0
#define Exist 1
#define BeEaten 2
#define PropTime 5

using namespace std;

void gotoxy(int, int);
void HideCursor();

void gotoxy(int x, int y){
	COORD pos = {x, y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 獲取標準輸出裝置控制代碼
	SetConsoleCursorPosition(hOut, pos);//兩個引數分別是指定哪個窗體，具體位置
}
void HideCursor(){
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

class Player;

class Object{
protected:
    Object *next;
    clock_t start;
    clock_t finish;
    char Type;
    int State;
    static float Monster_Speed;
    static int Monster_GetOut;

public:
    Object *Getnext(){
        return next;
    }
    void setNext(Object *_next){
        next = _next;
    }

    virtual int Countdown() = 0;
    virtual void ChasePlayer(Player *P) = 0;
    virtual void KeepAwayPlayer(Player *P) = 0;

    char GetType(){
        return Type;
    }
    void SetType(char t){
        Type = t;
    }
    int GetState(){
        return State;
    }
    void SetState(int ss){
        State = ss;
    }
    int Get_Monster_GetOut(){
        return Monster_GetOut;
    }

};

float Object::Monster_Speed = MonsterS;
int Object::Monster_GetOut = 0;

class Map{
protected:
    static int MAP[WIDTH][HEIGHT];
    static int Map_Time;
    int map_x;
    int map_y;
    int expand_x;
    int expand_y;
    int howmanyblock;
public:
    Map(){
    }
    void PrintObstacles(){
        for(int i = 0; i <= WIDTH; i++){
            MAP[i][0] = TRUE;
            gotoxy(i, 0);
            cout << "*";
        }
        for(int i = 0; i <= WIDTH; i++){
            MAP[i][HEIGHT] = TRUE;
            gotoxy(i, HEIGHT);
            cout << "*";
        }
        for(int i = 0; i <= HEIGHT; i++){
            MAP[0][i] = TRUE;
            gotoxy(0, i);
            cout << "*";
        }
        for(int i = 0; i <= HEIGHT; i++){
            MAP[WIDTH][i] = TRUE;
            gotoxy(WIDTH, i);
            cout << "*";
        }
        Status();
        howmanyblock = rand() % 6 + 10;
        for(int i = 1; i <= howmanyblock; i++){
            expand_x = rand() % 10 + 3;
            expand_y = rand() % 8 + 2;
            map_x = rand() % WIDTH;
            map_y = rand() % HEIGHT;
            while(MAP[map_x][map_y] == TRUE){
                map_x = rand() % WIDTH;
                map_y = rand() % HEIGHT;
            }
            for(int j = map_x; j < map_x + expand_x; j++){
                for(int k = map_y; k < map_y + expand_y; k++){
                    if(j > WIDTH || k > HEIGHT)
                        break;
                    else{
                        gotoxy(j, k);
                        MAP[j][k] = TRUE;
                        cout << "*";
                    }
                }
            }
        }
    }
    void Status(){
        gotoxy(WIDTH + 1, 0);
        cout << "P:Player";
        gotoxy(WIDTH + 1, 1);
        cout << "Press(G.g):GodMod";
        gotoxy(WIDTH + 1, 3);
        cout << "0:Move_Roadblock";
        gotoxy(WIDTH + 1, 4);
        cout << "1:Hit_the_wall";
        gotoxy(WIDTH + 1, 5);
        cout << "2:Poison_Monster";
        gotoxy(WIDTH + 1, 6);
        cout << "3:Hide_Props_Monster";
        gotoxy(WIDTH + 1, 7);
        cout << "4:Basic_Monster";
        gotoxy(WIDTH + 1, 10);
        cout<<"F：玩家移動速度變快兩倍";
        gotoxy(WIDTH + 1, 11);
        cout<<"S：monster移動速度減半";
        gotoxy(WIDTH + 1, 12);
        cout<<"B：所有monster立刻死亡";
        gotoxy(WIDTH + 1, 13);
        cout<<"T：轉換規則";
        gotoxy(WIDTH + 1, 14);
        cout<<"A：倒數時間增加，30秒";
        gotoxy(WIDTH + 1, 15);
        cout<<"H：回復部分血量，HP加100";
    }
    bool CheckXY(int x, int y){
        if(MAP[x][y] == TRUE)
            return TRUE;
        else
            return FALSE;
    }
    void MinusTime(){
        Map_Time--;
    }
    void PlusTime(int t){
        Map_Time = Map_Time + t;
    }
};

int Map::MAP[WIDTH][HEIGHT];
int Map::Map_Time = MapTime;

class Player:public Map, public Object{
private:
    int P_x;
    int P_y;
    int HP;
    int Score;
    clock_t start;
    clock_t finish;
    float Player_Speed;

public:
    Player(){
        P_x = rand() % WIDTH;
        P_y = rand() % HEIGHT;
        while(CheckXY(P_x, P_y)){
            P_x = rand() % WIDTH;
            P_y = rand() % HEIGHT;
        }
        gotoxy(P_x, P_y);
        cout << "P";
        HP = 1000;
        Score = 0;
        Player_Speed = PlayerS;
        start = clock();
        Type = 'P';
        State = Exist;
    }
    ~Player(){};
    void ClearPlayer(){
        gotoxy(P_x, P_y);
        cout << " ";
    }
    void PrintPlayer(){
        gotoxy(P_x, P_y);
        cout << "P";
    }
    void ChasePlayer(Player *P){
        if(kbhit()){
            int key = getch();
            switch(key){
                case UP:
                    if(CheckXY(P_x, P_y - 1))
                        break;
                    else{
                        ClearPlayer();
                        P_y = P_y - 1;
                        PrintPlayer();
                    }
                    break;
                case DOWN:
                    if(CheckXY(P_x, P_y + 1))
                        break;
                    else{
                        ClearPlayer();
                        P_y = P_y + 1;
                        PrintPlayer();
                    }
                    break;
                case LEFT:
                    if(CheckXY(P_x - 1, P_y))
                        break;
                    else{
                        ClearPlayer();
                        P_x = P_x - 1;
                        PrintPlayer();
                    }
                    break;
                case RIGHT:
                    if(CheckXY(P_x + 1, P_y))
                        break;
                    else{
                        ClearPlayer();
                        P_x = P_x + 1;
                        PrintPlayer();
                    }
                    break;
                case GodModG:case GodModg:
                    HP = 1000000;
                    Map_Time = 10000;
                    Player_Speed = 0;
                    break;
            }
        }
    }
    void Status(){
        gotoxy(0, HEIGHT + 1);
        cout << "Score : " << Score << endl;
        cout << "HP : " << HP;
        cout << "     Time : " << Map_Time / 60 << ":" << Map_Time % 60 << endl;
    }
    void UnderAttack(){
        for(int i = 0; i < WIDTH; i++){
            gotoxy(i, HEIGHT + 2);
            cout << " ";
        }
        HP = HP - 5;
        if(HP <= 0)
            delete this;
    }
    void UnderAttack(int damage){
        for(int i = 0; i < WIDTH; i++){
            gotoxy(i, HEIGHT + 2);
            cout << " ";
        }
        HP = HP - damage;
        if(HP <= 0)
            delete this;
    }
    void PlusScore(int s){
        for(int i = 0; i < WIDTH; i++){
            gotoxy(i, HEIGHT + 2);
            cout << " ";
        }
        Score = Score + s;
    }
    int Countdown(){
        double duration;
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        if(duration >= Player_Speed){
            start = clock();
            return TRUE;
        }
        else
            return FALSE;
    }
    int GetX(){
        return P_x;
    }
    int GetY(){
        return P_y;
    }
    void SetHP(int h){
        HP = HP + h;
    }
    void Set_Player_Speed(float s){
        Player_Speed = s;
    }
    void KeepAwayPlayer(Player *P){}
};

class Monster:public Map, public Object{
protected:
    int M_x;
    int M_y;
    int ATK;
    int TargetX;
    int TargetY;
    clock_t start;
    clock_t finish;

public:
    Monster(int a){
        M_x = rand() % WIDTH;
        M_y = rand() % HEIGHT;
        while(CheckXY(M_x, M_y)){
            M_x = rand() % WIDTH;
            M_y = rand() % HEIGHT;
        }
        gotoxy(M_x, M_y);
        ATK = a;
        start = clock();
    }
    void PrintMonster(){
        gotoxy(M_x, M_y);
        cout << Type;
    }
    void ClearMonster(){
        gotoxy(M_x, M_y);
        cout << " ";
    }
    int Countdown(){
        double duration;
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        if(duration >= Monster_Speed){
            start = clock();
            return TRUE;
        }
        else
            return FALSE;
    }
    void SetTarget(int x, int y){
        TargetX = x;
        TargetY = y;
    }
    void KeepAwayPlayer(Player *P){}
};

class Move_Roadblock_Monster:public Monster{
private:
    int Initial_x;
    int Initial_y;
public:
    Move_Roadblock_Monster(int a):Monster(a){
        Initial_x = M_x;
        Initial_y = M_y;
        TargetX = M_x + 10;
        Type = '0';
        PrintMonster();
        State = Exist;
    }
    void SetTarget(int tx){
        TargetX = tx;
    }
    void ChasePlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->UnderAttack(ATK);
        }
        if(CheckXY(M_x + 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x - 1;
            PrintMonster();
            SetTarget(Initial_x);
        }
        else{
            gotoxy(M_x, M_y);
            cout << " ";
            if(M_x < TargetX)
                M_x++;
            else if(M_x > TargetX)
                M_x--;
            PrintMonster();
        }
        if(M_x == TargetX)
            SetTarget(Initial_x);

        if(M_x == Initial_x)
            SetTarget(M_x + 10);
    }
    void KeepAwayPlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->PlusScore(Eat_Monster);
            State = Died;
        }
        if(CheckXY(M_x + 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x - 1;
            PrintMonster();
            SetTarget(Initial_x);
        }
        else{
            gotoxy(M_x, M_y);
            cout << " ";
            if(M_x < TargetX)
                M_x++;
            else if(M_x > TargetX)
                M_x--;
            PrintMonster();
        }
        if(M_x == TargetX)
            SetTarget(Initial_x);

        if(M_x == Initial_x)
            SetTarget(M_x + 10);
    }

};

class Hit_the_wall_Monster:public Monster{
private:
    int Count = 0;
public:
    Hit_the_wall_Monster(int a):Monster(a){
        Type = '1';
        PrintMonster();
        State = Exist;
    }
    void SetTarget(int tx, int ty){
        TargetX = tx;
        TargetY = ty;
    }
    void ChasePlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->UnderAttack(ATK);
        }
        SetTarget(P->GetX(), P->GetY());
        if(Count > 10){
            gotoxy(M_x,M_y);
            cout<<" ";
            if(M_x < TargetX)
                M_x++;
            else if(M_x > TargetX)
                M_x--;
            if(M_y < TargetY)
                M_y++;
            else if(M_y > TargetY)
                M_y--;
            PrintMonster();
            MAP[M_x][M_y] = FALSE;
        }
        else if(CheckXY(M_x + 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x + 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x + 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else{
            gotoxy(M_x,M_y);
            cout<<" ";
            if(M_x < TargetX)
                M_x++;
            else if(M_x > TargetX)
                M_x--;
            if(M_y < TargetY)
                M_y++;
            else if(M_y > TargetY)
                M_y--;
            PrintMonster();
        }
    }
    void KeepAwayPlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->PlusScore(Eat_Monster);
            State = Died;
        }
        else if(CheckXY(M_x + 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x + 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x + 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y - 1;
            PrintMonster();
            Count++;
        }
        else if(CheckXY(M_x - 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y + 1;
            PrintMonster();
            Count++;
        }
        else{
            gotoxy(M_x,M_y);
            cout<<" ";
            if(M_x < P->GetX())
           	    M_x--;
           	else if(M_x > P->GetX())
               	M_x++;
           	if(M_y < P->GetY())
               	M_y--;
           	else if(M_y > P->GetY())
           	    M_y++;
            PrintMonster();
        }
    }
};


class Poison_Monster:public Monster{
private:
	int direction;
public:
	Poison_Monster(int a):Monster(a){
        Type = '2';
        PrintMonster();
        State = Exist;
    }
	void single_wall(int wall){
		direction = rand() %4;
	   	while(direction==wall){
	   		direction = rand() %4;
		}
    	switch(direction){
	        case 0:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y + 1;
	           	PrintMonster();
	            break;
	        case 1:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y - 1;
	           	PrintMonster();
	           	break;
	        case 2:
	        	gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x - 1;
	            PrintMonster();
	            break;
	        case 3:
	            gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x + 1;
	            PrintMonster();
	            break;

	   }
	}
	void corner_wall(int wall_1,int wall_2){
	   	direction = rand() %4;
	   	while(direction==wall_1||direction==wall_2){
	   		direction = rand() %4;
		}
	    switch(direction){
	        case 0:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y + 1;
	           	PrintMonster();
	            break;
	        case 1:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y - 1;
	           	PrintMonster();
	           	break;
	        case 2:
	            gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x - 1;
	            PrintMonster();
	            break;
	        case 3:
	            gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x + 1;
	            PrintMonster();
	            break;

		   }
	}
	void outside_corner(){
		direction = rand() %4;
	    switch(direction){
	       	case 0:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y + 1;
	           	PrintMonster();
	            break;
	        case 1:
	            gotoxy(M_x, M_y);
	            cout << " ";
	           	M_y = M_y - 1;
	           	PrintMonster();
	           	break;
	        case 2:
	           	gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x - 1;
	           	PrintMonster();
	            break;
	        case 3:
	            gotoxy(M_x, M_y);
	           	cout << " ";
	           	M_x = M_x + 1;
	            PrintMonster();
	            break;
	        }
	}


    void ChasePlayer(Player *P){
       	if(CheckXY(M_x + 1, M_y)){ //右邊牆
       		if(CheckXY(M_x , M_y - 1)){//下邊牆
           	corner_wall(RIGHTWALL,DOWNWALL);
            }
            else if(CheckXY(M_x , M_y + 1)){//上邊牆
            corner_wall(RIGHTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x - 1 , M_y)){//左邊牆
           	corner_wall(RIGHTWALL,LEFTWALL);
       		}
       		else
       		single_wall(RIGHTWALL);
       	}
       	else if(CheckXY(M_x - 1, M_y)){//左邊牆
       		if(CheckXY(M_x , M_y + 1)){//上邊牆
            corner_wall(LEFTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x, M_y - 1)){//下邊牆
           	corner_wall(LEFTWALL,DOWNWALL);
       		}
           	else single_wall(LEFTWALL);
       	}
       	else if(CheckXY(M_x , M_y + 1)){//上邊牆
       		if(CheckXY(M_x , M_y - 1)){//下邊牆
           	corner_wall(UPWALL,DOWNWALL);
            }
            else single_wall(UPWALL);
       	}
       	else if(CheckXY(M_x , M_y - 1)){//下邊牆
           	single_wall(DOWNWALL);
       	}
       	else if(CheckXY(M_x + 1 , M_y - 1)){
           	outside_corner();
       	}
       	else if(CheckXY(M_x + 1 , M_y + 1)){
            outside_corner();
       	}
       	else if(CheckXY(M_x - 1 , M_y + 1)){
        	outside_corner();
       	}
       	else if(CheckXY(M_x - 1 , M_y - 1)){
           	outside_corner();
       	}
       	else{
           	gotoxy(M_x,M_y);
           	cout<<" ";
           	if(M_x < P->GetX())
           	    M_x++;
           	else if(M_x > P->GetX())
               	M_x--;
           	if(M_y < P->GetY())
               	M_y++;
           	else if(M_y > P->GetY())
           	    M_y--;
           		PrintMonster();
       	}
       	Distance_Between_Monster_And_player(P);
    }
    void Distance_Between_Monster_And_player(Player *P){
    	if(sqrt(pow(M_x-P->GetX(),2)+pow(M_y-P->GetY(),2))<4)
    		P->UnderAttack();
	}
	void KeepAwayPlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->PlusScore(Eat_Monster);
            State = Died;
        }
        if(CheckXY(M_x + 1, M_y)){ //右邊牆
       		if(CheckXY(M_x , M_y - 1)){//下邊牆
           	corner_wall(RIGHTWALL,DOWNWALL);
            }
            else if(CheckXY(M_x , M_y + 1)){//上邊牆
            corner_wall(RIGHTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x - 1 , M_y)){//左邊牆
           	corner_wall(RIGHTWALL,LEFTWALL);
       		}
       		else
       		single_wall(RIGHTWALL);
       	}
       	else if(CheckXY(M_x - 1, M_y)){//左邊牆
       		if(CheckXY(M_x , M_y + 1)){//上邊牆
            corner_wall(LEFTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x, M_y - 1)){//下邊牆
           	corner_wall(LEFTWALL,DOWNWALL);
       		}
           	else single_wall(LEFTWALL);
       	}
       	else if(CheckXY(M_x , M_y + 1)){//上邊牆
       		if(CheckXY(M_x , M_y - 1)){//下邊牆
           	corner_wall(UPWALL,DOWNWALL);
            }
            else single_wall(UPWALL);
       	}
       	else if(CheckXY(M_x , M_y - 1)){//下邊牆
           	single_wall(DOWNWALL);
       	}
       	else if(CheckXY(M_x + 1 , M_y - 1)){
           	outside_corner();
       	}
       	else if(CheckXY(M_x + 1 , M_y + 1)){
            outside_corner();
       	}
       	else if(CheckXY(M_x - 1 , M_y + 1)){
        	outside_corner();
       	}
       	else if(CheckXY(M_x - 1 , M_y - 1)){
           	outside_corner();
       	}
       	else{
           	gotoxy(M_x,M_y);
           	cout<<" ";
           	if(M_x < P->GetX())
           	    M_x--;
           	else if(M_x > P->GetX())
               	M_x++;
           	if(M_y < P->GetY())
               	M_y--;
           	else if(M_y > P->GetY())
           	    M_y++;
           		PrintMonster();
       	}
       	Distance_Between_Monster_And_player(P);
    }
};



class Hide_Props_Monster:public Monster{
protected:
	int skinMode;
	int Add_x;
	int Add_y;
	int Sub_x;
	int Sub_y;
public:
	Hide_Props_Monster(int a):Monster(a){
		Add_x=M_x+5;
		Add_y=M_y+5;
		Sub_x=M_x-5;
		Sub_y=M_y-5;
		Type = '3';
		PrintMonster();
        State = Exist;
		Changeskin();
	}

	void ChasePlayer(Player *player){//移動
		//Changeskin();
		if(M_x == player->GetX() && M_y == player->GetY()){
            player->UnderAttack(ATK);
        }else if( Add_x>=player->GetX() && Sub_x<=player->GetX() && Add_y>=player->GetY() && Sub_y<=player->GetY()){//追逐
        	Type='3';
			if(CheckXY(M_x + 1, M_y)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	PrintMonster();
        	}else if(CheckXY(M_x - 1, M_y)){
           		gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x + 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x , M_y + 1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
           		M_y = M_y - 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x , M_y - 1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_y = M_y + 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x + 1, M_y+1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	M_y=M_y-1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x-1, M_y+1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x +1;
            	M_y=M_y-1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x+1, M_y-1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	M_y=M_y+1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x-1, M_y-1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x+1;
            	M_y=M_y+1;
            	PrintMonster();
        	}
        	else{
        		if( (M_x!=player->GetX()) && (M_y!=player->GetY()) ){
					gotoxy(M_x,M_y);
					cout<<" ";
					if(M_x<player->GetX()){
						M_x++;
					}else if(M_x>player->GetX()){
						M_x--;
					}
					if(M_y<player->GetY()){
						M_y++;
					}else if(M_y>player->GetY()){
						M_y--;
					}

					gotoxy(M_x,M_y);
					PrintMonster();
				}
			}

		}else if(Type=='3'){
			if(CheckXY(M_x + 1, M_y)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	PrintMonster();
        	}else if(CheckXY(M_x - 1, M_y)){
           		gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x + 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x , M_y + 1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
           		M_y = M_y - 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x , M_y - 1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_y = M_y + 1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x + 1, M_y+1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	M_y=M_y-1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x-1, M_y+1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x +1;
            	M_y=M_y-1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x+1, M_y-1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x -1;
            	M_y=M_y+1;
            	PrintMonster();
        	}
        	else if(CheckXY(M_x-1, M_y-1)){
            	gotoxy(M_x, M_y);
            	cout << " ";
            	M_x = M_x+1;
            	M_y=M_y+1;
            	PrintMonster();
        	}
        	else{
					gotoxy(M_x,M_y);
					cout<<" ";
					if(M_x<player->GetX()){
						M_x++;
					}else if(M_x>player->GetX()){
						M_x--;
					}
					if(M_y<player->GetY()){
						M_y++;
					}else if(M_y>player->GetY()){
						M_y--;
					}

					gotoxy(M_x,M_y);
					PrintMonster();
			}
		}else if(Type!=3){//一開始偽裝成道具的時候
			gotoxy(M_x,M_y);
			cout<<" ";
			gotoxy(M_x,M_y);
			PrintMonster();
		}
	}
	void Changeskin(){
			skinMode=rand()%6+1;
			switch(skinMode){
				case 1:
					Type='F';
					break;
				case 2:
					Type='S';
					break;
				case 3:
					Type='B';
					break;
				case 4:
					Type='T';
					break;
				case 5:
					Type='A';
					break;
				case 6:
					Type='H';
					break;
			}

	}
	void KeepAwayPlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->PlusScore(Eat_Monster);
            State = Died;
        }
        else if(CheckXY(M_x + 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            PrintMonster();
        }
        else if(CheckXY(M_x - 1, M_y)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            PrintMonster();
        }
        else if(CheckXY(M_x , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y - 1;
            PrintMonster();
        }
        else if(CheckXY(M_x , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_y = M_y + 1;
            PrintMonster();
        }
        else if(CheckXY(M_x + 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y + 1;
            PrintMonster();
        }
        else if(CheckXY(M_x + 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x -1;
            M_y = M_y - 1;
            PrintMonster();
        }
        else if(CheckXY(M_x - 1 , M_y + 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y - 1;
            PrintMonster();
        }
        else if(CheckXY(M_x - 1 , M_y - 1)){
            gotoxy(M_x, M_y);
            cout << " ";
            M_x = M_x + 1;
            M_y = M_y + 1;
            PrintMonster();
        }
        else{
            gotoxy(M_x,M_y);
            cout<<" ";
            if(M_x < P->GetX())
           	    M_x--;
           	else if(M_x > P->GetX())
               	M_x++;
           	if(M_y < P->GetY())
               	M_y--;
           	else if(M_y > P->GetY())
           	    M_y++;
            PrintMonster();
        }
	}
};


class Basic_Monster:public Monster{
//倒數30秒後，monsters速度變為原本2倍
public:
	int mp;

	Basic_Monster(int a):Monster(a){
		Type = '4';
		State = Exist;
		PrintMonster();
	}
	void SetTarget(int tx, int ty){
       	TargetX = tx;
       	TargetY = ty;
   	}
   	void rightwall(){
   		mp = rand() %3;
            switch(mp){
                case 0:
            gotoxy(M_x, M_y);
            cout << " ";
           	M_y = M_y + 1;
           	PrintMonster();

                    break;
                case 1:
            gotoxy(M_x, M_y);
           	cout << " ";
           	M_y = M_y - 1;
             PrintMonster();
                    break;
                case 2:
            gotoxy(M_x, M_y);
           	cout << " ";
           	M_x = M_x - 1;
             PrintMonster();
                    break;

	   }
    }
    void leftwall(){
   		mp = rand() %3;
            switch(mp){
                case 0:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y + 1;
                    PrintMonster();
                    break;
                case 1:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y - 1;
                     PrintMonster();
                    break;
                case 2:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x + 1;
                     PrintMonster();
                    break;

            }
    }
    void upwall(){
   		mp = rand() %3;
            switch(mp){
                case 0:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y - 1;
                    PrintMonster();
                    break;
                case 1:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x + 1;
                     PrintMonster();
                    break;
                case 2:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x - 1;
                     PrintMonster();
                    break;

	   }
    }
    void downwall(){
   		mp = rand() %3;
            switch(mp){
                case 0:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y + 1;
                    PrintMonster();
                    break;
                case 1:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x + 1;
                     PrintMonster();
                    break;
                case 2:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x - 1;
                     PrintMonster();
                    break;
	   }
    }
    void cornerwall(){
        mp = rand() %4;
        switch(mp){
                case 0:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y + 1;
                    PrintMonster();
                    break;
                case 1:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y - 1;
                    PrintMonster();
                case 2:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x + 1;
                     PrintMonster();
                    break;
                case 3:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x - 1;
                     PrintMonster();
                    break;

            }
    }
    void twowall(int one,int two){
        mp=rand()%4;
   		while(mp==one||mp==two){
	   		mp = rand() %4;
		}
            switch(mp){
                case 0:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y + 1;
                    PrintMonster();
                    break;
                case 1:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_y = M_y - 1;
                    PrintMonster();
                case 2:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x + 1;
                     PrintMonster();
                    break;
                case 3:
                    gotoxy(M_x, M_y);
                    cout << " ";
                    M_x = M_x - 1;
                     PrintMonster();
                    break;

            }
    }

	void ChasePlayer(Player *P){
       	if(M_x == P->GetX() && M_y == P->GetY()){
           	P->UnderAttack(ATK);
       	}
       	//SetTarget(P->GetX(), P->GetY());
       	if(CheckXY(M_x + 1, M_y)){
       		if(CheckXY(M_x , M_y - 1)){
           	twowall(RIGHTWALL,DOWNWALL);
            }
            else if(CheckXY(M_x , M_y + 1)){
            twowall(RIGHTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x - 1 , M_y)){
           	twowall(RIGHTWALL,LEFTWALL);
       		}
       		else
       		rightwall();
       	}
       	else if(CheckXY(M_x - 1, M_y)){
       		if(CheckXY(M_x , M_y + 1)){
            twowall(LEFTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x, M_y - 1)){
           	twowall(LEFTWALL,DOWNWALL);
       		}
           	else leftwall();
       	}
       	else if(CheckXY(M_x , M_y + 1)){
       		if(CheckXY(M_x , M_y - 1)){
           	twowall(UPWALL,DOWNWALL);
            }
            else upwall();
       	}
       	else if(CheckXY(M_x , M_y - 1)){
           	downwall();
       	}
       	else if(CheckXY(M_x + 1 , M_y - 1)){
           	cornerwall();
       	}
       	else if(CheckXY(M_x + 1 , M_y + 1)){
            cornerwall();
       	}
       	else if(CheckXY(M_x - 1 , M_y + 1)){
        	cornerwall();
       	}
       	else if(CheckXY(M_x - 1 , M_y - 1)){
           	cornerwall();
       	}
       	else{
           	gotoxy(M_x,M_y);
           	cout<<" ";
           	if(M_x < P->GetX())
           	    M_x++;
           	else if(M_x > P->GetX())
               	M_x--;
           	if(M_y < P->GetY())
               	M_y++;
           	else if(M_y > P->GetY())
           	    M_y--;
           		PrintMonster();
       	}
	}
   	void KeepAwayPlayer(Player *P){
        if(M_x == P->GetX() && M_y == P->GetY()){
            P->PlusScore(Eat_Monster);
            State = Died;
        }
        if(CheckXY(M_x + 1, M_y)){
       		if(CheckXY(M_x , M_y - 1)){
           	twowall(RIGHTWALL,DOWNWALL);
            }
            else if(CheckXY(M_x , M_y + 1)){
            twowall(RIGHTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x - 1 , M_y)){
           	twowall(RIGHTWALL,LEFTWALL);
       		}
       		else
       		rightwall();
       	}
       	else if(CheckXY(M_x - 1, M_y)){
       		if(CheckXY(M_x , M_y + 1)){
            twowall(LEFTWALL,UPWALL);
       		}
       		else if(CheckXY(M_x, M_y - 1)){
           	twowall(LEFTWALL,DOWNWALL);
       		}
           	else leftwall();
       	}
       	else if(CheckXY(M_x , M_y + 1)){
       		if(CheckXY(M_x , M_y - 1)){
           	twowall(UPWALL,DOWNWALL);
            }
            else upwall();
       	}
       	else if(CheckXY(M_x , M_y - 1)){
           	downwall();
       	}
       	else if(CheckXY(M_x + 1 , M_y - 1)){
           	cornerwall();
       	}
       	else if(CheckXY(M_x + 1 , M_y + 1)){
            cornerwall();
       	}
       	else if(CheckXY(M_x - 1 , M_y + 1)){
        	cornerwall();
       	}
       	else if(CheckXY(M_x - 1 , M_y - 1)){
           	cornerwall();
       	}
       	else{
           	gotoxy(M_x,M_y);
           	cout<<" ";
           	if(M_x < P->GetX())
           	    M_x--;
           	else if(M_x > P->GetX())
               	M_x++;
           	if(M_y < P->GetY())
               	M_y--;
           	else if(M_y > P->GetY())
           	    M_y++;
           		PrintMonster();
       	}
   	}
};

class Props:public Map, public Object{
private:
    int Props_x;
    int Props_y;

public:
    Props(){
        Props_x = rand() % WIDTH;
        Props_y = rand() % HEIGHT;
        while(CheckXY(Props_x, Props_y)){
            Props_x = rand() % WIDTH;
            Props_y = rand() % HEIGHT;
        }
        State = Exist;
        switch(rand() % 6){
            case 0:
                Type = 'F';
                PrintProps();
                break;
            case 1:
                Type = 'S';
                PrintProps();
                break;
            case 2:
                Type = 'B';
                PrintProps();
                break;
            case 3:
                Type = 'T';
                PrintProps();
                break;
            case 4:
                Type = 'A';
                PrintProps();
                break;
            case 5:
                Type = 'H';
                PrintProps();
                break;
        }

    }
    void PrintProps(){
        gotoxy(Props_x, Props_y);
        cout << Type;
    }
    void F(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
            start = clock();
            if(State == Exist)
                P->PlusScore(Eat_Props);
            P->Set_Player_Speed(DoublePlayerS);
            State = BeEaten;
        }

        if(Countdown() && State == BeEaten){
            P->Set_Player_Speed(PlayerS);
            State = Died;
        }
    }
    void S(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
            start = clock();
            if(State == Exist)
                P->PlusScore(Eat_Props);
            P->Monster_Speed = SlowMonsterS;
            State = BeEaten;
        }

        if(Countdown() && State == BeEaten){
            P->Monster_Speed = MonsterS;
            State = Died;
        }
    }
    void B(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
             P->PlusScore(Eat_Props);
             State = Died;
        }
    }
    void T(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
            start = clock();
            if(State == Exist)
                P->PlusScore(Eat_Props);
            P->Monster_GetOut = 1;
            State = BeEaten;
        }

        if(Countdown() && State == BeEaten){
            P->Monster_GetOut = 0;
            State = Died;
        }
    }
    void A(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
            P->PlusTime(30);
            P->PlusScore(Eat_Props);
            State = Died;
        }
    }
    void H(Player *P){
        if(State == Exist)
            PrintProps();

        if(Props_x == P->GetX() && Props_y == P->GetY()){
            P->SetHP(100);
            P->PlusScore(Eat_Props);
            State = Died;
        }
    }
    int Countdown(){
        double duration;
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        if(duration >= PropTime){
            return TRUE;
        }
        else
            return FALSE;
    }
    void ChasePlayer(Player *P){
        if(Type == 'F'){
            F(P);
        }
        else if(Type == 'S'){
            S(P);
        }
        else if(Type == 'B'){
            B(P);
        }
        else if(Type == 'T'){
            T(P);
        }
        else if(Type == 'A'){
            A(P);
        }
        else if(Type == 'H'){
            H(P);
        }
    }
    void KeepAwayPlayer(Player *P){}
};

class LinkedList{
private:
    Object *p;
public:
    LinkedList(){
        p = NULL;
    }
    void InsertObject(Object *newObj){
        if(p == NULL){
            p = newObj;
            p->setNext(p);
        }
        else{
            newObj->setNext(p->Getnext());
            p->setNext(newObj);
        }
    }
    void Action(Player *P){
        Object *q;
        q = p;
        do{
            if(p != NULL){
                if(q->GetType() == 'F' || q->GetType() == 'S' || q->GetType() == 'A' || q->GetType() == 'H')
                    q->ChasePlayer(P);

                else if(q->GetType() == 'B'){
                    q->ChasePlayer(P);
                    if(q->GetState() == Died){
                        MonsterDied(P);
                    }
                }

                else if(q->GetType() == 'T'){
                    q->ChasePlayer(P);
                }

                else if(q->GetType() == 'P'){
                    if(q->Countdown())
                        q->ChasePlayer(P);
                }

                else{
                    if(q->Get_Monster_GetOut() == 1){
                        if(q->Countdown())
                            q->KeepAwayPlayer(P);
                    }
                    else if(q->Countdown())
                        q->ChasePlayer(P);
                }
                if(q->Getnext()->GetState() == Died)
                    DeleteObject(q);
                q = q->Getnext();
            }
        }while(q != p);
    }

    void MonsterDied(Player *P){
        Object *q;
        q = p;
        do{
            if(p != NULL){
                if(q->GetType() == '0' || q->GetType() == '1' || q->GetType() == '2' || q->GetType() == '3' || q->GetType() == '4'){
                    q->SetType(' ');
                    q->ChasePlayer(P);
                    q->SetState(Died);
                    P->PlusScore(Eat_Monster);
                }
            }
            q = q->Getnext();
        }while(q != p);
    }

    void DeleteObject(Object *obj){
        Object *del;
        del = obj->Getnext();
        obj->setNext(del->Getnext());
        delete del;
    }
};

class UI{
private:
    clock_t start;
    clock_t finish;
public:
    UI(){
        start = clock();
    }
    void Action(LinkedList *list, Player *P){
        list->Action(P);
        double duration;
        double Monster_duration;
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;


        if(duration >= 10){
            Props *item = new Props;
            Monster_duration += 10;
            start = clock();
            list->InsertObject(item);
        }


        if(Monster_duration >= 20){
            int Type;
            Type = rand() % 4;
            if(Type == 0){
                Move_Roadblock_Monster *MRM = new Move_Roadblock_Monster(200);
                Monster_duration = 0;
                list->InsertObject(MRM);
            }
            else if(Type == 1){
                Hit_the_wall_Monster *HTWM = new Hit_the_wall_Monster(200);
                Monster_duration = 0;
                list->InsertObject(HTWM);
            }
            else if(Type == 2){
                Poison_Monster *PM = new Poison_Monster(5);
                Monster_duration = 0;
                list->InsertObject(PM);
            }
            else if(Type == 3){
                Hide_Props_Monster *HPM = new Hide_Props_Monster(200);
                Monster_duration = 0;
                list->InsertObject(HPM);
            }
            else if(Type == 4){
                Basic_Monster *BM = new Basic_Monster(200);
                Monster_duration = 0;
                list->InsertObject(BM);
            }
        }
        P->Status();
    }
};



int main(){
    HideCursor();
    srand(time(NULL));

    Map M;
    M.PrintObstacles();

    LinkedList *list = new LinkedList;

    Player *P = new Player;
    list->InsertObject(P);



    int Type;
    Type = rand() % 4;
    if(Type == 0){
        Move_Roadblock_Monster *MRM = new Move_Roadblock_Monster(200);
        list->InsertObject(MRM);
    }
    else if(Type == 1){
        Hit_the_wall_Monster *HTWM = new Hit_the_wall_Monster(200);
        list->InsertObject(HTWM);
    }
    else if(Type == 2){
        Poison_Monster *PM = new Poison_Monster(5);
        list->InsertObject(PM);
    }
    else if(Type == 3){
        Hide_Props_Monster *HPM = new Hide_Props_Monster(200);
        list->InsertObject(HPM);
    }
    else if(Type == 4){
        Basic_Monster *BM = new Basic_Monster(200);
        list->InsertObject(BM);
    }


    clock_t start;
    clock_t finish;
    start = clock();


    UI *ui = new UI;


    while(1){
        double duration;
        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        if(duration >= 1){
            P->MinusTime();
            for(int i = 0; i < WIDTH; i++){
                gotoxy(i, HEIGHT + 2);
                cout << " ";
            }
            start = clock();
        }
        ui->Action(list, P);
    }

    return 0;
}
