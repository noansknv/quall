#pragma once
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
};

class Mesh
{
public:
	Point p;
	string mesh_name;
	string material_name;
};

class World
{
public:
	World() { };
	Point size;
    vector<Cube> cubes;
    vector<Spawn> spawnPoints;
	vector<Mesh> meshes;
	string floor;
	string ball;
	bool tab[15][17];
	void addSpawn(Spawn s) { };
	void addCube(Cube s) { };
};

