#ifndef GAME_CONTROLLER
#define GAME_CONTROLLER

#include <iostream>
#include <utility>
#include <string>
#include <queue>

#define DEBUG

void printDebug(std::string msg);

struct Block
{
    int color;
};

typedef std::pair<Block, Block> BlockPair;
typedef std::queue<BlockPair> BlockPairQueue;

class Grid
{
public:
    Grid();
private:
    std::vector<std::vector<int> > grid_;
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

