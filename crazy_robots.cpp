#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

struct Robot 
{
	char name;
    int hp;
    int x1;
    int y1;
    int x2;
    int y2;

    Robot(char name, int hp, int x1, int y1, int x2, int y2) :
    	name(name),
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

int numberOfRobots;
int robotsRemaining;
vector<Robot> robots;
vector<Shoot> shoots;

bool locationIsInRobot(const Robot& robot, const int& xPosition, const int& yPosition)
{
    return xPosition >= robot.x1 && xPosition <= robot.x2 && 
            yPosition >= robot.y1 && yPosition <= robot.y2;
}

bool isShootLocation(const int& xPosition,const int& yPosition)
{
    for (Shoot &shoot : shoots)
        if (xPosition == shoot.x && yPosition == shoot.y)
            return true;

    return false;
}

void printRobotHP(const Robot& robot)
{
    for (int hp = 0; hp < robot.hp; hp++)
        cout << "#";
    for (int hits = 0; hits < (10 - robot.hp); hits++)
        cout << " ";
}

void printGameField()
{
    for (int yPosition = 0; yPosition < 20; yPosition++) {
        for (int xPosition = 0; xPosition < 20; xPosition++) {
        	bool printFlag = false;
        	for (Robot &robot : robots) {
        		if (locationIsInRobot(robot,xPosition,yPosition)) {
        			cout << robot.name;
        			printFlag = true;
        			break;
        		}
        	}
        	if (!printFlag)
        		cout << (isShootLocation(xPosition, yPosition) ? "*" : "_");
        }
        cout << "|" << endl;
    }
}

void printHPArea()
{
    for (Robot &robot : robots) {
    	cout << "HP BOT " << robot.name << ": ";
		printRobotHP(robot);
		cout << "|" << endl;
    }    	
}

void printscreen()
{
#ifdef _WIN32
	system("CLS"); //clears screen for Windows OS
#else
	system("clear"); //clears screen for UNIX OS
#endif
    printGameField();
    printHPArea();
}

void robot_movements(string acts[], Robot& r)
{
    string c = acts[rand() % 8];
    if (c == "a") {
        if (r.x1 > 0) {
            r.x1--;
            r.x2--;
        }
    }
    else if (c == "d") {
        if (r.x2 < 19) {
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
        if (r.y2 < 19) {
            r.y1++;
            r.y2++;
        }
    }
    else if (c == "f1") { // esq
        shoots.push_back(Shoot(r.x1 - 1, r.y1, 0));
    }
    else if (c == "f2") { // dir
        shoots.push_back(Shoot(r.x2 + 1, r.y1, 1));
    }
    else if (c == "f3") { // cima
        shoots.push_back(Shoot(r.x1, r.y1 - 1, 2));
    }
    else if (c == "f4") { // baixo
        shoots.push_back(Shoot(r.x1, r.y2 + 1, 3));
    }
}

void removeDeadRobots() {
    robots.erase(remove_if(robots.begin(), 
                              robots.end(),
		[](Robot& x) {robotsRemaining--; return x.hp < 1; }), robots.end());
}

void handleHits() {
    for (auto it = shoots.begin(); it != shoots.end(); ) {
        	for (Robot &robot : robots) {
                if (locationIsInRobot(robot,(*it).x,(*it).y)) {
		                robot.hp--;
		                it = shoots.erase(it);
		                continue;
		            }
        	}
            ++it;
        }
}

bool thereIsOneLastSurvivor(){
    //check winning condition: last survivor
    if (robots.size() == 1) {
        cout << "ROBOT "<< robots[0].name <<" WINS!!!" << endl;
        return true;
    }
	else if (robots.size() == 0) {
		cout << "ALL ROBOTS HAVE BEEN DESTROYED. IT'S A DRAW" << endl;
		return true;
	}


    return false;
}

void doRobotMovement() {
    static string acts[] = {"a", "d", "w", "s", "f1", "f2", "f3", "f4"};

    for (Robot &robot : robots) {
        robot_movements(acts, robot);
    }
}

void doShootMovement() {
    
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
            if (s.x < 19)
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
            if (s.y < 19)
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


void checkCrash() {
	//crashFlags handles when multiple robots crash at once 
	int crashFlags[] = { 0,0,0,0 };
	int counter = 0; //flag counter for parent loop
	int next = 0; //flag counter for child loop and other robot

	for (auto it = robots.begin();it<robots.end();it++){
			for (auto inIt = it; inIt < robots.end(); inIt++) {
					if (inIt != it && (locationIsInRobot((*inIt),(*it).x1, (*it).y1) || locationIsInRobot((*inIt), (*it).x1, (*it).y2)
						|| locationIsInRobot((*inIt), (*it).x2, (*it).y1) || locationIsInRobot((*inIt), (*it).x2, (*it).y2))){ 
						if (crashFlags[counter]!=1){
							(*it).hp--; //remove health from Robot closer to the front of vector 
							crashFlags[counter] = 1;
						}
						if (crashFlags[counter + next] != 1){
							(*inIt).hp--; //remove the health from Robot it crashed into
							crashFlags[counter + next] = 1;
						}
					}
				next++;
			}
		next = 0; 
		counter++;
	}
}


int main(int argc, char *argv[])
{
	if (argc == 2) {
		numberOfRobots = atoi(argv[1]);
		robotsRemaining = numberOfRobots;
		if (numberOfRobots < 1 || numberOfRobots > 4) {
			cout << "1 <= Number of robots <= 4\n";
			return 1;
		}
	} else {
		cout << "One argument expected: number of robots.\n";
		return 1;
	}

    srand(time(0));

    if (numberOfRobots >= 1)
    	robots.push_back(Robot('X',10, 0, 0, 1, 1));
    if (numberOfRobots >= 2)
    	robots.push_back(Robot('Y',10, 18, 18, 19, 19));
    if (numberOfRobots >= 3)
    	robots.push_back(Robot('A',10, 0, 18, 1, 19));
    if (numberOfRobots >= 4)
    	robots.push_back(Robot('B',10, 18, 0, 19, 1));

    while (true) {
        this_thread::sleep_for(chrono::microseconds(50000));

        handleHits();
        removeDeadRobots();
        

		printscreen();
        if(thereIsOneLastSurvivor()){
            return 0;
        }

		doRobotMovement();
        doShootMovement();
        checkCrash();


    }
    
    return 0;
}
