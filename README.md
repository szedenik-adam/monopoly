﻿# monopoly
Browser based online multiplayer monopoly game written in C++ (backend) and Typescript (frontend).

## Compile guide
### Frontend
1. Step into the frontend folder.
2. Run **npm  install** to install the dependencies.
3. Install grunt with the **npm install –g grunt-cli** command.
4. Build the frontend with the **grunt build** command.
### Backend - Windows
1. Install dependencies: **vcpkg install libevent:x64-windows-static curl:x64-windows**
 > Note: libcurl can be ommitted when not using emails.
2. Update .h/.lib/.dll files at **x64-win-dep** with the previously installed ones (should be at vcpkg root dir/installed/x64-windows-static or x64-windows).
3. Open the Visual Studio solution at **backend/Monopoly.sln**
4. Set the solution configuration to Release and the solution platform to x64.
5. Under Build menu, click Build Solution.
### Backend - Linux
1. Step into the backend folder.
2. Install cmake: **apt install cmake**
3. Install libevent: **apt install libevent-dev**
4. Add FindLibEvent.cmake if missing (also check cmake version and update "cmake-2.8" dir accordingly): **cp build_scripts/FindLibEvent.cmake /usr/share/cmake-2.8/Modules/FindLibEvent.cmake**
5. Copy the CMakeLists file to the project root directory: **cp build_scripts/LinuxCMakeLists.txt CMakeLists.txt**
6. Generate makefile: **cmake . -DCMAKE_CXX_COMPILER=/usr/bin/g++**
7. Build: **make**


## Screenshots
![table demo image](doc/image/table_demo_1.png)

![table demo image](doc/image/table_demo_2.png)

## Authors
Márton Bálint - frontend

Ádám Szedenik - backend