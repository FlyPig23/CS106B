// Hangxiao Zhu

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
#include "vector.h"
using namespace std;

Vector<int> getGridSize(ifstream& gridFile) {
    Vector<int> size;
    string line;
    int lineIndex = 0;

    while (lineIndex <= 1) {
        getline(gridFile, line);
        size.add(stringToInteger(line));
        lineIndex++;
    }

    return size;
}

Grid<char> getGrid(ifstream& gridFile, Vector<int>& size) {
    Grid<char> myGrid(size[0], size[1]);
    int lineIndex = 0;
    string line;

    /* Notice:
     * After the ifstream read the first two lines in getGridSize(ifstream& gridFile),
     * now the ifstream continues reading the rest of the file,
     * i.e. the first two lines will not be read again.
     **/
    while (lineIndex < size[0]) {
        getline(gridFile, line);
        for (int i = 0; i < size[1]; i++) {
            myGrid[lineIndex][i] = line[i];
        }
        lineIndex++;
    }

    return myGrid;
}

void showGrid(Grid<char>& myGrid, Vector<int>& size) {
    int row = size[0];
    int col = size[1];
    int rowCount = 0;
    int colCount = 0;

    while (rowCount < row) {
        while (colCount < col) {
            cout << myGrid[rowCount][colCount];
            colCount++;
        }
        cout << endl;
        colCount = 0;
        rowCount++;
    }
}

Grid<int> countNeighbors(bool wrap, Grid<char>& myGrid, Vector<int>& size) {
    Grid<int> neighboNumGrid(size[0], size[1]);
    int rowAbove;
    int rowSame;
    int rowBelow;
    int colLeft;
    int colSame;
    int colRight;

    if (!wrap) {
        for (int row = 0; row < size[0]; row++) {
            for (int col = 0; col < size[1]; col++) {
                int count = 0;
                if (row == 0) {
                    rowSame = 0;
                    rowBelow = 1;
                    if (col == 0) {
                        colSame = 0;
                        colRight = 1;
                        count = int(myGrid[rowSame][colRight] == 'X') + int(myGrid[rowBelow][colSame] == 'X')
                                + int(myGrid[rowBelow][colRight] == 'X');
                    } else if (col == size[1] - 1) {
                        colSame = size[1] - 1;
                        colLeft = size[1] - 2;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowBelow][colSame] == 'X')
                                + int(myGrid[rowBelow][colLeft] == 'X');
                    } else {
                        colLeft = col - 1;
                        colSame = col;
                        colRight = col + 1;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowSame][colRight] == 'X')
                                + int(myGrid[rowBelow][colLeft] == 'X') + int(myGrid[rowBelow][colSame] == 'X')
                                + int(myGrid[rowBelow][colRight] == 'X');
                    }
                } else if (row == size[0] - 1) {
                    rowAbove = size[0] - 2;
                    rowSame = size[0] - 1;
                    if (col == 0) {
                        colSame = 0;
                        colRight = 1;
                        count = int(myGrid[rowSame][colRight] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colRight] == 'X');
                    } else if (col == size[1] - 1) {
                        colSame = size[1] - 1;
                        colLeft = size[1] - 2;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colLeft] == 'X');
                    } else {
                        colLeft = col - 1;
                        colSame = col;
                        colRight = col + 1;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowSame][colRight] == 'X')
                                + int(myGrid[rowAbove][colLeft] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colRight] == 'X');
                    }
                } else {
                    rowAbove = row - 1;
                    rowSame = row;
                    rowBelow = row + 1;
                    if (col == 0) {
                        colSame = 0;
                        colRight = 1;
                        count = int(myGrid[rowSame][colRight] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colRight] == 'X') + int(myGrid[rowBelow][colSame] == 'X')
                                + int(myGrid[rowBelow][colRight] == 'X');
                    } else if (col == size[1] - 1) {
                        colSame = size[1] - 1;
                        colLeft = size[1] - 2;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colLeft] == 'X') + int(myGrid[rowBelow][colSame] == 'X')
                                + int(myGrid[rowBelow][colLeft] == 'X');
                    } else {
                        colLeft = col - 1;
                        colSame = col;
                        colRight = col + 1;
                        count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowSame][colRight] == 'X')
                                + int(myGrid[rowAbove][colLeft] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                                + int(myGrid[rowAbove][colRight] == 'X') + int(myGrid[rowBelow][colLeft] == 'X')
                                + int(myGrid[rowBelow][colSame] == 'X') + int(myGrid[rowBelow][colRight] == 'X');
                    }
                }
                neighboNumGrid[row][col] = count;
            }
        }
    } else {
        for (int row = 0; row < size[0]; row++) {
            rowAbove = (row - 1 + size[0]) % size[0];
            rowSame = row;
            rowBelow = (row + 1 + size[0]) % size[0];
            for (int col = 0; col < size[1]; col++) {
                int count = 0;
                colLeft = (col - 1 + size[1]) % size[1];
                colSame = col;
                colRight = (col + 1 + size[1]) % size[1];
                count = int(myGrid[rowSame][colLeft] == 'X') + int(myGrid[rowSame][colRight] == 'X')
                        + int(myGrid[rowAbove][colLeft] == 'X') + int(myGrid[rowAbove][colSame] == 'X')
                        + int(myGrid[rowAbove][colRight] == 'X') + int(myGrid[rowBelow][colLeft] == 'X')
                        + int(myGrid[rowBelow][colSame] == 'X') + int(myGrid[rowBelow][colRight] == 'X');
                neighboNumGrid[row][col] = count;
            }
        }
    }

    return neighboNumGrid;
}

Grid<char> updateGrid(Grid<char>& myGrid, Grid<int>& neighboNumGrid, Vector<int>& size) {
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            if (neighboNumGrid[i][j] <= 1 || neighboNumGrid[i][j] >= 4) {
                myGrid[i][j] = '-';
            } else if (neighboNumGrid[i][j] == 2) {
                myGrid[i][j] = myGrid[i][j];
            } else if (neighboNumGrid[i][j] == 3) {
                myGrid[i][j] = 'X';
            }
        }
    }

    return myGrid;
}

string numberToWord(int number) {
    Vector<string> nums;
    nums.add("zero");
    nums.add("one");
    nums.add("two");
    nums.add("three");
    nums.add("four");
    nums.add("five");
    nums.add("six");
    nums.add("seven");
    nums.add("eight");
    nums.add("nine");

    if (number >= 0 && number <= 9) {
        return nums[number];
    } else {
        return "Number out of range";
    }
}

int main() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << endl;

    ifstream gridFile;
    string fileName = promptUserForFile(gridFile, "Grid input file name?");

    bool wrap = getYesOrNo("Should the simulation wrap around the grid (y/n)?");
    Vector<int> size = getGridSize(gridFile);
    Grid<char> myGrid(size[0], size[1]);
    myGrid = getGrid(gridFile, size);
    showGrid(myGrid, size);

    Grid<int> neighboNumGrid(size[0], size[1]);

    string command;
    do {
        command = getLine("a)nimate, t)ick, q)uit?");
        command = toLowerCase(command);
        if (command == "a") {
           int frameNum = getInteger("How many frames?", "Illegal integer format. Try again.");
           string frameNumStr = numberToWord(frameNum);
           if (frameNum >= 10) {
                cout << frameNumStr << endl;
           } else {
                cout << "(" << frameNumStr << " new generations are shown, with screen clear and 50ms pause before each)" << endl;
           }
           for (int frame = 0; frame < frameNum; frame++) {
                clearConsole();
                neighboNumGrid = countNeighbors(wrap, myGrid, size);
                myGrid = updateGrid(myGrid, neighboNumGrid, size);
                showGrid(myGrid, size);
                pause(50);
           }
        } else if (command == "t") {
           neighboNumGrid = countNeighbors(wrap, myGrid, size);
           myGrid = updateGrid(myGrid, neighboNumGrid, size);
           showGrid(myGrid, size);
        } else if (command != "q") {
            cout << "Illegal command. Try again." << endl;
        }
    } while (command != "q");

    cout << "Have a nice Life!" << endl;

    gridFile.close();

    return 0;
}
