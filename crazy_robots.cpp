#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;

struct Robot 
{
    int hp;
    int x1;
    int y1;
    int x2;
    int y2;

    Robot(int hp, int x1, int y1, int x2, int y2) :
        hp(hp),
        x1(x1),
        y1(y1),
        x2(x2),
        y2(y2)
    {
    }
};

struct Shoot
{
    int x;
    int y;
    int dir;

    Shoot(int x, int y, int dir) :
        x(x),
        y(y),
        dir(dir)
    {
    }
};

vector<Shoot> shoots;

void printscreen(Robot &robot, Robot &robot2)
{
    system("clear");
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            if (j >= robot.x1 && j <= robot.x2 && i >= robot.y1 && i <= robot.y2)
                cout << "X";
            else if (j >= robot2.x1 && j <= robot2.x2 && i >= robot2.y1 && i <= robot2.y2)
                cout << "Y";
            else {
                bool flag = false;
                for (Shoot &shoot : shoots) {
                    if (j == shoot.x && i == shoot.y) {
                        flag = true;
                        cout << "*";
                    }
                }

                if (!flag) cout << "_";
            }
        }
        cout << endl;
    }
    cout << "LIFE ROBOT 1: " << robot.hp << endl;
    cout << "LIFE ROBOT 2: " << robot2.hp << endl;
}

int main()
{
    srand(time(0));
    Robot robot(10, 0, 0, 1, 1);
    Robot robot2(10, 38, 38, 39, 39);

    string acts[] = {"a", "d", "w", "s", "f1", "f2", "f3", "f4"};

    bool run = false;

    string c;
    while (true) {
        usleep(100000);

        printscreen(robot, robot2);

        Robot &r = run ? robot : robot2;
        run = !run;

        c = acts[rand() % 8];

        if (c == "a") {
            if (r.x1 > 0) {
                r.x1--;
                r.x2--;
            }
        }
        else if (c == "d") {
            if (r.x2 < 39) {
                r.x1++;
                r.x2++;
            }
        }
        else if (c == "w") {
            if (r.y1 > 0) {
                r.y1--;
                r.y2--;
            }
        }
        else if (c == "s") {
            if (r.y2 < 39) {
                r.y1++;
                r.y2++;
            }
        }
        else if (c == "f1") { // esq
            shoots.push_back(Shoot(r.x2 + 1, r.y1, 0));
        }
        else if (c == "f2") { // dir
            shoots.push_back(Shoot(r.x1 - 1, r.y1, 1));
        }
        else if (c == "f3") { // cima
            shoots.push_back(Shoot(r.x1, r.y1 - 1, 2));
        }
        else if (c == "f4") { // baixo
            shoots.push_back(Shoot(r.x1, r.y2 + 1, 3));
        }

        for (Shoot &s : shoots) {
            if (s.dir == 0) {
                if (s.x > 0)
                    s.x--;
                else {
                    s.x = -1;
                    s.y = -1;
                }
            }
            else if (s.dir == 1) {
                if (s.x < 39)
                    s.x++;
                else {
                    s.x = -1;
                    s.y = -1;
                }
            }
            else if (s.dir == 2) {
                if (s.y > 0)
                    s.y--;
                else {
                    s.x = -1;
                    s.y = -1;
                }
            }
            else if (s.dir == 3) {
                if (s.y < 39)
                    s.y++;
                else {
                    s.x = -1;
                    s.y = -1;
                }
            }
        }

        vector<Shoot> tmp;
        for (Shoot &s : shoots)
            if (s.x != -1 && s.y != -1)
                tmp.push_back(s);
        shoots.clear();
        for (Shoot &s : tmp)
            shoots.push_back(s);
    }
    
    return 0;
}
