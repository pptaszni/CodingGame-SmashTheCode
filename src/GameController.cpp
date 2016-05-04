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
}
