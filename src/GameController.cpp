#include "GameController.hpp"

void printDebug(std::string msg)
{
    #ifdef DEBUG
    std::cerr << msg << std::endl;
    #endif
}

Grid::Grid()
{
    grid_.resize(6);
}

void GameController::startGame()
{
    printDebug("Game started!");
    while(1)
    {
        readData(std::cin);
        writeSolution(0);
    }
}

void GameController::readData(std::istream& input)
{
    for (int i = 0; i < 8; i++)
    {
        int colorA;
        int colorB;
        input >> colorA >> colorB; input.ignore();
    }
    for (int i = 0; i < 12; i++)
    {
        std::string row;
        input >> row; input.ignore();
    }
    for (int i = 0; i < 12; i++)
    {
        std::string row;
        input >> row; input.ignore();
    }
}

void GameController::writeSolution(int column)
{
    std::cout << column << std::endl;
}

