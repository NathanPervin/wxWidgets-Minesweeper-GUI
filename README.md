# __wxWidgets Minesweeper GUI__
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/9x9img2.jpg" alt="Alt Text" width="500" height="500">

## Description
This is a basic minesweeper game that follows the same standard rules:
- Each number represents how many mines are in the surrounding 8 cells
- If the player clicks on a mine, the player loses
- If they player clears all non-mine cells, the player wins 

This program was created using the wxWidgets framework. 

This game has a main menu that allows the user to choose a grid size from the following options:
- 9x9 with 15 mines
- 16x16 with 50 mines
- 30x16 with 100 mines
- Custom

If the user selects the Custom option, they can enter a grid width, height, and number of bombs
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/minesweeperMainMenuCustom.jpg" alt="Alt Text" width="250" height="250">

The game includes the following features
- Home button that opens the main menu
- Flag button and right click to flag cells
- Hint button to show position of a hidden mine
- Restart button to restart the game with the same grid size and # of mines
- Vertical and Horizontal scroll for larger than 9x9 grid sizes

##### Flag, Hint, and Restart
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/minesweeperDemo.gif" alt="Alt Text" width="250" height="250">

##### Horizontal and Vertical Scroll
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/30x16GIF.gif" alt="Alt Text" width="250" height="250">

## Installation and dependencies 
Software Used:
- Visual Studio Community 2022

Dependencies:
- wxWidgets Library

This video explains the wxWidgets installation and linking process well:
[Video by OttoBotCode](https://youtu.be/ONYW3hBbk-8?si=DCXSBozoQEiIZvvM)

The files for creating the window are App.h and App.cpp. The files for UI and gameplay are MainFrame.h and MainFrame.cpp.
The program can then be compiled and run in Visual Studio.

## License

MIT License

Copyright (c) 2025 Nathan Pervin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

