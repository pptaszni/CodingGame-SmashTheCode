#ifndef GAME_CONTROLLER
#define GAME_CONTROLLER

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <queue>

#define QUEUE_LENGTH 8
#define GRID_WIDTH 6
#define GRID_HEIGHT 12

#define DEBUG

void printDebug(std::string msg);

struct Block
{
    Block(): Block('.') {}
    Block(char c): color(c) {}
    Block(int c): color(static_cast<char>(c)) {}
    char color;
};

typedef std::pair<Block, Block> BlockPair;
typedef std::vector<BlockPair> BlockPairQueue;

class Grid
{
public:
    Grid();
    void reset(std::vector<std::string> newGrid);
private:
    std::vector<std::string> grid_;
};


class GameController
{
public:
    void startGame();

    // in - out operations
    void readData(std::istream& input);
    void writeSolution(int column);
private:
    Grid currentGrid_;
    BlockPairQueue nextBlocks_;
};

#endif  // GAME_CONTROLLER

