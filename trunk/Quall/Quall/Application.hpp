/**
 * Glowna klasa aplikacji opisujaca logike programu.
 * @author marcin.kanclerz
 */
#pragma once

#include "Ogre.h"
#include "OgreWindowEventUtilities.h"
#include "btBulletDynamicsCommon.h"

#include "Types.hpp"
#include "Configuration.hpp"
#include "NullDelete.hpp"
#include "InitWorld.hpp"
#include "Input.hpp"
#include "Simulation.hpp"
#include "Description.hpp"
#include "WorldManager.hpp"


class Application
{
public:
  Application();
  ~Application();
  
  /** Uruchomienie aplikacji */
  void go();
  
private:

    // Korzen drzewa Ogra.
    OgreRootPtr ogWorld;
    
    // Glowne okno Ogra.
    OgreRenderWindowPtr ogWindow;
    
    // Opcje okna Ogra.
    Ogre::NameValuePairList ogRenderOpts;

    // Zarzadca swiata fiz-graf.
    WorldManagerPtr worldManager;

    // Tymczasowe rozwiazanie maszyny stanowej gry.
    Simulation *sim;
    // Uchwyt buforowanego wejscia.
    InputHandlerPtr oisHandler;

    // Utworzenie korzenia drzewa Ogra.
    void createOgreRoot();
    
    // Ustawienie systemu wyswietlania.
    void setupOgreRenderSystem();
    
    // Dodanie zasobow.
    void defineOgreResources();
    
    // Zdefiniowanie grup zasobow.
    void initializeOgreResourceGroups();
    
    // Utworzenie glownego okna wyswietlania.
    void createOgreRenderWindow();
    
    // Ustawienie handlerow systemu buforowanego wejscia.
    void setupInputSystem();
    
    // Ustawienie zarzadcy swiata.
    void setupWorldManager();
    
    // Glowna petla.
    void startMainLoop();

    //void setupCEGUI();
    //void createFrameListener();
    //void startRenderLoop();


};
