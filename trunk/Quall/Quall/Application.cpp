#include "Application.hpp"


Application::Application()
{
	sim = new Simulation();
}


Application::~Application()
{
}


/**
* Utworzenie okna Ogra, inicjalizacja zasobow, utworzenie zarzadcy swiata, inputu i odpalenie glownej petli.
*/
void Application::go()
{
  createOgreRoot();
  setupOgreRenderSystem();
  defineOgreResources();
  initializeOgreResourceGroups();
  createOgreRenderWindow();

  setupWorldManager();

  setupInputSystem();
  startMainLoop();

  //setupCEGUI();
  //createFrameListener();
  //startRenderLoop();
}


/**
* Utworzenie korzenia drzewa Ogra.
*/
void Application::createOgreRoot()
{
  // nadpisanie dwoch pierwszych argumentow umozliwia nam odreczne zdefiniowanie pluginow i zasobow
  ogWorld.reset(new Ogre::Root("", ""), NullDelete());
}


/**
* Ustawienie systemu wyswietlania dla Ogra.
* Korzystamy z OpenGL.
*/
void Application::setupOgreRenderSystem()
{
  ogWorld->loadPlugin("RenderSystem_GL_d");

  //Ogre::RenderSystemList *renderSystems = NULL;
  //Ogre::RenderSystemList::iterator rIt;
  //renderSystems = ogWorld->getAvailableRenderers();
  //rIt = renderSystems->begin();
  //ogWorld->setRenderSystem(*rIt);

  // bardzo silowe rozwiazanie wyszukania renderera
  ogWorld->setRenderSystem(*(ogWorld->getAvailableRenderers()->begin()));
  ogWorld->initialise(false);
}


/**
* Definiowanie zasobow Ogra.
*/
void Application::defineOgreResources()
{
  // standardowe pluginy
  ogWorld->loadPlugin("Plugin_CgProgramManager_d");
  ogWorld->loadPlugin("Plugin_OctreeSceneManager_d");

  // dodanie lokacji zasobow
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("resource", "FileSystem", "General");
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(RESOURCE_GUI, "Zip", "GUI");
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(RESOURCE_FONT, "FileSystem", "GUI");
}


/**
* Utworzenie grup zasobow Ogra.
*/
void Application::initializeOgreResourceGroups()
{
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
}


/**
* Utworzenie okna do wyswietlania.
*/
void Application::createOgreRenderWindow()
{
  // zakodowane na twardo opcje - przyspiesza proces testowania
  //boost::shared_ptr<Ogre::NameValuePairList> opts;
  //opts.reset(new Ogre::NameValuePairList());
  ogRenderOpts["resolution"] = "1024x768";
  ogRenderOpts["fullscreen"] = "false";
  ogRenderOpts["vsync"] = "false";

  // tworzy okno z nazwa APPNAME
  ogWindow.reset(ogWorld->createRenderWindow(APPNAME, 1024, 768, false, &ogRenderOpts));
}


/**
* Korzystajac z przykladowego opisu swiata (Description), tworzony jest zarzadca.
*/
void Application::setupWorldManager()
{
  InitWorldPtr worldDesc(new Description(ogWorld, ogWindow, sim));
  worldManager.reset(new WorldManager(worldDesc));
}


/**
* Uruchamiamy buforowany system wejscia.
*/
void Application::setupInputSystem()
{
  // Na uzytek handlera.
  unsigned long hWnd;
  ogWindow->getCustomAttribute("WINDOW", &hWnd);
  oisHandler.reset(new InputHandler(worldManager, sim, hWnd), NullDelete());
}


/**
* Tymczasowe rozwiazanie polegajace na hardkodowaniu jedynej planszy i symulowaniu wydarzen majacych na niej miejsce.
* Nastepnie zlapanie eventow generowanych przez system wejscia.
*/
void Application::startMainLoop()
{
  sim->requestStateChange(SIMULATION);
  // Uzywana jest tymczasowa maszyna stanowa.
  while (sim->getCurrentState() != SHUTDOWN)
  {
    //worldDesc->oneStep();
    worldManager->oneStep();
    oisHandler->capture();
  }
}