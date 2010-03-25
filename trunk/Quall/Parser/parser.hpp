/*!
 * Author: Czubek i Filip
 */
#pragma once

#include "tinyxml.h"
#include "tinystr.h"
#include "obiekty.hpp"

/*!
 * Ta klasa sluzy do przerabiania plików w formacie xml w obiekty c++
 * Ka¿da prywatna metoda s³u¿y do tworzenia pewnych obiektów, natomiast
 * metoda getWorld tworzy ca³y œwiat.
 */
class Parser
{
public:
	Parser(string file_);
	
	void readFile();
	World getWorld();

	string file;
private:
	Spawn parseSpawn(TiXmlElement* pElem);
	Cube parseCube(TiXmlElement* pElem);

	TiXmlDocument doc;
	vector<string> names;
};