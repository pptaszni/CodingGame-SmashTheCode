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

void Grid::reset(std::vector<std::string> newGrid)
{
    grid_ = newGrid;
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
    BlockPairQueue nextBlocks;
    std::vector<std::string> myNewGrid;
    std::vector<std::string> opponentNewGrid;
    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        int colorA;
        int colorB;
        input >> colorA >> colorB; input.ignore();
        nextBlocks.push_back(std::make_pair(Block(colorA), Block(colorB)));
    }
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        std::string row;
        input >> row; input.ignore();
        myNewGrid.push_back(row);
    }
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        std::string row;
        input >> row; input.ignore();
        opponentNewGrid.push_back(row);
    }
    nextBlocks_ = nextBlocks;
    currentGrid_.reset(myNewGrid);
}

void GameController::writeSolution(int column)
{
    std::cout << column << std::endl;
}

