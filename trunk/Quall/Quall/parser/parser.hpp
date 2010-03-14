/*!
 * Author: Czubek i Filip
 */

#include "tinyxml.h"
#include "tinystr.h"
#include "obiekty.hpp"

/*!
 * Ta klasa sluzy do przerabiania plik�w w formacie xml w obiekty c++
 * Ka�da prywatna metoda s�u�y do tworzenia pewnych obiekt�w, natomiast
 * metoda getWorld tworzy ca�y �wiat.
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
    Point parseWorld(TiXmlElement* pElem);
    Bonus parseBonus(TiXmlElement* pElem);
    Flag parseFlag(TiXmlElement* pElem);

	TiXmlDocument doc;
	vector<string> names;
};