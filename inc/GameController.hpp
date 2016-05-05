#ifndef GAME_CONTROLLER
#define GAME_CONTROLLER

#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <queue>
#include <set>

#define QUEUE_LENGTH 8
#define GRID_WIDTH 6
#define GRID_HEIGHT 12

#define DEBUG

void printDebug(std::string msg);
void printDebug(std::ostringstream& msg);

struct Block
{
    Block(): Block('.') {}
    Block(char c): color(c) {}
    Block(int c): color('0'+c) {}
    bool operator==(const Block &obj) const { return obj.color==color; }
    char color;
};

typedef std::pair<Block, Block> BlockPair;
typedef std::vector<BlockPair> BlockPairQueue;
typedef std::vector<std::string> GridType;
typedef std::pair<int, int> Point;

class Grid
{
public:
    Grid();

    // setters

    void reset(std::vector<std::string> newGrid);
    void reset(std::istream& input);

    // getters

    int size1() const;
    int size2() const;
    bool isGood() const;
    GridType getGrid() const;
    char get(int height, int width) const;

    // algorithms
    int getNumNeighboursAfterInsert(int colNum, BlockPair blockPair) const;
protected:
    void updateSize();
private:
    std::pair<Point, Point> getPositionsAfterInsert(int colNum) const;
    void fillNeighboursOf(Point position, Block block, std::set<Point>& visitedPositions) const;
    std::vector<Point> getNeighbours(Point position, Block block) const;
    bool inBounds(int height, int width) const;
    int size1_;
    int size2_;
    bool isGood_;
    GridType grid_;
};

namespace GridAnalysis
{
int findShortestColumn(const Grid& grid);
std::pair<int, bool> findColWithHighestSimpleScore(const Grid& grid, BlockPair blockPair);
}

class GameController
{
public:
    void startGame();

    // strategies
    void simpleStrategy();

    // getters
    BlockPairQueue getNextBlocks();
    Grid getCurrentGrid();
    Grid getOpponentGrid();

    // in - out operations
    void readData(std::istream& input);
    void writeSolution(int column);
    void fullDebugPrint();
private:
    BlockPairQueue nextBlocks_;
    Grid currentGrid_;
    Grid opponentGrid_;
};

#endif  // GAME_CONTROLLER

