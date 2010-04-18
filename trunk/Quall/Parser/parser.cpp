/*!
 * Autor:Czooobbbaaasss!!
 */
#include <iostream>
#include <cctype>
#include <algorithm>

#include "parser.hpp"

using namespace std;

Parser::Parser(string file_)
{
	file = file_;
};

void Parser::readFile()
{
	TiXmlDocument doc_(file);

	doc = doc_;
	bool load = doc.LoadFile();
	if (!load)
		cout << "Error loading file " << file << endl;
};

World Parser::getWorld()
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem = hDoc.FirstChildElement().Element();

	string node(pElem->Value());
	transform(node.begin(), node.end(), node.begin(), tolower);

	if (node.compare("world") != 0)
	{
		cout << "<world> is not first element of the file" << endl;
		abort();
	}
	
	World res;

	TiXmlAttribute* pAttrib = pElem->FirstAttribute();
	int i = 0;

	while(pAttrib) {
		string a(pAttrib->Name());
		transform(a.begin(), a.end(), a.begin(), tolower);
		
		if (a.compare("x") == 0)
			res.size.x = (float)atof(pAttrib->Value());
		else if (a.compare("y") == 0)
			res.size.z = (float)atof(pAttrib->Value());
		else if (a.compare("z") == 0)
			res.size.y = (float)atof(pAttrib->Value());
		else
			cout << "Unknown attribute in World" << endl;

		pAttrib = pAttrib->Next();
		i++;
	}
	
	if (i != 3)
		cout << "Wrong number of attributes in World" << endl;

	hRoot = TiXmlHandle(pElem);
	pElem = hRoot.FirstChild().Element();

	for(pElem; pElem; pElem = pElem->NextSiblingElement()) 
	{
		node = pElem->Value();
		transform(node.begin(), node.end(), node.begin(), tolower);
		if (node.compare("spawn") == 0)
		{
			Spawn s;
			s = parseSpawn(pElem);
			//res.addSpawn(s);
			res.spawnPoints.push_back(s);
		}
		else if (node.compare("cube") == 0)
		{
			Cube c;
			c = parseCube(pElem);
			//res.addCupe(c);
			res.cubes.push_back(c);
		}
		else 
			cout << "Unknown node in file " << file << endl;
	}

	return res;
};

Spawn Parser::parseSpawn(TiXmlElement* pElem) 
{
	TiXmlAttribute* pAttrib = pElem->FirstAttribute();
	Spawn res;
	Point p;

	int i = 0;

	while(pAttrib) {
		string a(pAttrib->Name());
		transform(a.begin(), a.end(), a.begin(), tolower);
		
		if (a.compare("x") == 0)
			p.x = (float)atof(pAttrib->Value());
		else if (a.compare("y") == 0)
			p.z = (float)atof(pAttrib->Value());
		else if (a.compare("z") == 0)
			p.y = (float)atof(pAttrib->Value());
		else
			cout << "Unknown attribute in Spawn" << endl;

		pAttrib = pAttrib->Next();
		i++;
	}

	if (i != 3)
		cout << "Wrong amount of attributes in Spawn" << endl;

	res.p = p;

	return res;
};

Cube Parser::parseCube(TiXmlElement* pElem)
{
	TiXmlAttribute* pAttrib = pElem->FirstAttribute();
	Cube res;
	Point p1, p2;
	int i = 0;

	while(pAttrib) {
		string a(pAttrib->Name());
		transform(a.begin(), a.end(), a.begin(), tolower);
		
		if (a.compare("x1") == 0)
			p1.x = (float)atof(pAttrib->Value());
		else if (a.compare("y1") == 0)
			p1.z = (float)atof(pAttrib->Value());
		else if (a.compare("z1") == 0)
			p1.y = (float)atof(pAttrib->Value());
		else if (a.compare("x2") == 0)
			p2.x = (float)atof(pAttrib->Value());
		else if (a.compare("y2") == 0)
			p2.z = (float)atof(pAttrib->Value());
		else if (a.compare("z2") == 0)
			p2.y = (float)atof(pAttrib->Value());
		else
			cout << "Unknown attribute in Spawn" << endl;

		pAttrib = pAttrib->Next();
		i++;
	}

	if (i != 6)
		cout << "Wrong amount of attributes in Spawn" << endl;
	
	res.p1 = p1;
	res.p2 = p2;

	TiXmlHandle hRoot(0);
	hRoot = TiXmlHandle(pElem);
	pElem = hRoot.FirstChild().Element();
	string node;

	for(pElem; pElem; pElem = pElem->NextSiblingElement()) 
	{
		node = pElem->Value();
		transform(node.begin(), node.end(), node.begin(), tolower);
		if (node.compare("texture") == 0)
			res.texture = pElem->GetText();
		else if (node.compare("material") == 0)
			res.material = pElem->GetText();
		else if (node.compare("weight") == 0)
			res.weight = (float)atof(pElem->GetText());
	}

	return res;
};
//
//int main() {
//	int n;
//	Parser p("..\\..\\resource\\plansza.xml");
//	World w;
//	p.readFile();
//	w = p.getWorld();
//	cout << w.spawnPoints[0].p.x << " " << w.spawnPoints[0].p.y << " " << w.spawnPoints[0].p.z << endl;
//	cout << w.cubes[0].material << " " << w.cubes[0].p1.x << " " << w.cubes[0].weight << endl;
//	cin >> n;
//	return 0;
//}
//
