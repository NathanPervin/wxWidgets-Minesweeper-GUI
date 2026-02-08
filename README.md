# __wxWidgets Minesweeper GUI__
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/ImagesAndGifs/9x9titleicon.png" alt="Alt Text" width="500" height="500">

## Description
This is a basic minesweeper game that follows the same standard rules:
- Each number represents how many mines are in the surrounding 8 cells
- If the player clicks on a mine, the player loses
- If the player clears all non-mine cells, the player wins 

The GUI was created using the wxWidgets framework. 

This game has a main menu that allows the user to choose a grid size from the following options:
- 9x9 with 15 mines
- 16x16 with 50 mines
- 30x16 with 100 mines
- Custom

If the user selects the Custom option, they can enter a grid width, height, and number of bombs

<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/ImagesAndGifs/minesweeperMainMenuCustom.jpg" alt="Alt Text" width="250" height="250">

The game includes the following features
- Home button that opens the main menu
- Flag button and right click to flag cells
- Hint button to show position of a hidden mine
- Restart button to restart the game with the same grid size and # of mines
- Vertical and Horizontal scroll for larger than 9x9 grid sizes
- Left click on a numbered cell to reveal all surrounding cells (if the number of the cell is the same as the number of flags surrounding it)

##### Flag, Hint, and Restart
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/ImagesAndGifs/9x9FlagHintRestartGIF.gif" alt="Alt Text" width="250" height="250">

##### Horizontal and Vertical Scroll
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/ImagesAndGifs/MinesweeperScrollDemoGIF.gif" alt="Alt Text" width="250" height="250">

##### Full Game Playthrough
<img src="https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI/blob/master/ImagesAndGifs/9x9PlaythroughGIF.gif" alt="Alt Text" width="250" height="250">

## Installation 

### Linux

Clone this repo
```bash
git clone https://github.com/NathanPervin/wxWidgets-Minesweeper-GUI.git
```

Install wxWidgets
```bash
apt install libwxgtk3.2-dev
```

Compile Program
```bash
g++ App.cpp MainFrame.cpp `wx-config --cxxflags --libs` -o minesweeper.exe
```

Run
```bash
./minesweeper.exe
```

If the icons don't show properly try running
```bash
apt install fonts-noto-color-emoji
```

### Windows
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
