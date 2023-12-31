#include <iostream>
#include <cstdlib>
#include <windows.h> // для задержки 
using namespace std;
void initializeGrid(bool** grid, int rows, int columns) {
    srand(int(time(0)));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            grid[row][col] = rand() % 2;  // Случайное заполнение (0 - мертвая клетка  1 - живая клетка)
        }
    }
}
int countLiveNeighbors(bool** grid, int rows, int columns, int row, int col) {
    int liveNeighborCount = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns && grid[newRow][newCol]) {
                liveNeighborCount++;
            }
        }
    }
    return liveNeighborCount;
}
void updateGrid(bool** grid, bool** newGrid, int rows, int columns) {
    bool gridChanged = false;
    bool anyCellAlive = false;
    bool periodicConfig = false;  
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            int liveNeighbors = countLiveNeighbors(grid, rows, columns, row, col);

            if (!grid[row][col] && liveNeighbors == 3) {
                newGrid[row][col] = true;  // Пустая клетка оживает
                gridChanged = true;
            }
            else if (grid[row][col] && (liveNeighbors < 2 || liveNeighbors > 3)) {
                newGrid[row][col] = false;  // Живая клетка умирает
                gridChanged = true;
            }
            else {
                newGrid[row][col] = grid[row][col];
                if (grid[row][col]) {
                    anyCellAlive = true;
                }
            }
        }
    }
    if (!anyCellAlive) {
        // Если на поле не останется ни одной живой клетки
        cout << "Игра завершена: со следующим ходом на поле не останется живых клеток" << endl;
        exit(0); 
    }
    if (!gridChanged) {
        // Если ни одна клетка не меняет своего состояния
        cout << "Игра завершена: достигнута стабильная конфигурация" << endl;
        exit(0);  
    }
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (newGrid[row][col] != grid[row][col]) {
                periodicConfig = false;  
                break;
            }
        }
    }
    if (periodicConfig) {
        // Если конфигурация на очередном шаге в точности повторяет себя же на одном из более ранних шагов
        cout << "Игра завершена: достигнута периодическая конфигурация" << endl;
        exit(0);  
    }
    for (int row = 0; row < rows; ++row) {  // Обновление состояния сетки
        for (int col = 0; col < columns; ++col) {
            grid[row][col] = newGrid[row][col];
        }
    }
}
void visualizeGrid(bool** grid, int rows, int columns) {
    system("cls");  // Очистка консоли
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (grid[row][col]) {
                cout << " * ";  // живая клетка
            }
            else {
                cout << " . ";  // мертвая клетка
            }
        }
        cout << endl;
    }
}
void playGameOfLife(int rows, int columns) {
    bool** grid = new bool* [rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new bool[columns];
    }
    initializeGrid(grid, rows, columns);
    while (true) {
        visualizeGrid(grid, rows, columns);
        bool** newGrid = new bool* [rows];
        for (int i = 0; i < rows; ++i) {
            newGrid[i] = new bool[columns];
        }
        updateGrid(grid, newGrid, rows, columns);
        for (int i = 0; i < rows; ++i) {  // Очистка
            delete[] grid[i];
        }
        delete[] grid;
        grid = newGrid; // Переназначение
        Sleep(500);  // Задержка 
    }
}
int main() {
    setlocale(LC_ALL, ""); // Локализация
    int rows = 10;
    int columns = 10;
    playGameOfLife(rows, columns);  // Запуск игры
    return 0;
}