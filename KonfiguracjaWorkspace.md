# Details #

  * ściągamy ogreSDK, instalujemy dowolnie gdzieś na komputerze

https://sourceforge.net/projects/ogre/files/ogre/1.6.5/OgreSDKSetup1.6.5_VC90.exe/download

  * ściągamy boost'a (1.41.0), rozpakowywujemy go na C:\ (powinien stworzyć się katalog boost\_1\_41\_0)

http://sourceforge.net/projects/boost/files/boost/1.41.0/boost_1_41_0.zip/download

  * ściągamy bullet'a (2.75), rozpakowywujemy go na C:\ (powinien stworzyć się katalog bullet-2.75), wchodzimy do C:\bullet-2.75\msvc\8\ i otwieramy plik "wksbullet.sln" w Visual Studio 2008, odpalamy build'a (powinno się wszystko skompilować bez problemów)

http://bullet.googlecode.com/files/bullet-2.75.zip

  * ściągamy nasze repo, mamy tam 2 katalogi: Quall i resource, kopiujemy resource do C:\

https://quall.googlecode.com/svn/trunk/

  * otwieramy Quall.sln w Visual Studio 2008, w Project/Quall properties ustawiamy po kolei:

Debugging : Command                               = $(OutDir)\$(ProjectName).exe

Debugging : Working Directory                     = $(OutDir)

Debugging : Environment                           = PATH=%PATH%;%OGRE\_HOME%\bin\debug

C/C++ : General : Additional Include Directories  = ..\Parser;..\include;$(OGRE\_HOME)\include;$(OGRE\_HOME)\samples\include;c:\bullet-2.75\src;c:\boost\_1\_41\_0\

C/C++ : Preprocessor : Preprocessor Definitions   += _STLP\_DEBUG_

C/C++ : Code Generation : Use runtime library 	  = Multithreaded Debug DLL

Linker : General : Output File                    = $(OutDir)\$(ProjectName).exe

Linker : General : Additional Library Directories = $(OGRE\_HOME)\lib;C:\bullet-2.75\out\debug8\libs

Linker : Input : Additional Dependencies          += OgreMain\_d.lib OIS\_d.lib CEGUIBase\_d.lib OgreGUIRenderer\_d.lib libbulletcollision\_d.lib libbulletdynamics\_d.lib libbulletmath\_d.lib libbulletopenglsupport\_d.lib