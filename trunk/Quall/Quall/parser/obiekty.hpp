#include <vector>
#include <string>

using namespace std;

class Point
{
public:
	float x;
	float y;
	float z;
};

class Cube
{
public:
	Point p1; // x, y, z
    Point p2; // x, y, z
    float weight;
    string texture;
    string material;
};

class Spawn
{
public:
    Point p;
    string team;
};

class Flag
{
public:
    Point p;
    string team;
};

class Bonus 
{
public:
    Point p;
    string type;
};

class World
{
public:
	World() { };
	Point size;
    vector<Cube> cubes;
    vector<Spawn> spawnPoints;
    vector<Flag> flags;
    vector<Bonus> bonuses;
};

