/**
 * Ta klasa sluzy do przerabiania plikow w formacie xml w obiekty c++.
 * Poszczegolne funkcje prywatne tworza odpowiadajace im obiekty.
 * @author piotr.czubak filip.mazowiecki
 */
#pragma once

#include "tinyxml.h"
#include "tinystr.h"
#include "obiekty.hpp"


class Parser
{
public:
	Parser(string file_);
	
	void readFile();
	/** Tworzy caly swiat na podstawie opisu. */
	World getWorld();

	string file;
private:
	Spawn parseSpawn(TiXmlElement* pElem);
	Cube parseCube(TiXmlElement* pElem, bool do_tablicy, World *wrld);

	TiXmlDocument doc;
	vector<string> names;
};