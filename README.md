# TicTacToe

A C++ learning project from when I was young. I followed this tutorial: http://www.sdltutorials.com/sdl-tutorial-tic-tac-toe but I added some computer player code. Then I recently decided to revisit it after going to university, and clean it up, adding some more functionality. I also wanted to create an unbeatable AI, which I have started but not yet finished.

# Setting up on new machine

To set up this project on a new Windows machine:

1. Install Code::Blocks with MinGW compiler built in.
2. Download SDL2 development lib here: http://libsdl.org/download-2.0.php
2.1. Select the SDL2-devel-2.x.x-mingw.tar.gz link under Windows
2.2. Extract the files
2.3. Copy i686-w64-mingw32 directory to C:\mingw_dev_lib
3. Copy C:\mingw_dev_lib\bin\SDL2.dll to an appropriate runtime directory
3.1. On 64-bit, this will be C:\Windows\SysWOW64, on 32-bit C:\WINDOWS\SYSTEM32
3.2. Alternatively, put the dll in the folder with the executable
4. Open Tic Tac Toe.cbp, and compile and run it
