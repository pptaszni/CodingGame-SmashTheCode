#include <set>
#include "GameController.hpp"

void printDebug(std::string msg)
{
    #ifdef DEBUG
    std::cerr << msg << std::endl;
    #endif
}

void printDebug(std::ostringstream& msg)
{
    #ifdef DEBUG
    std::cerr << msg.str() << std::endl;
    #endif
}

Grid::Grid()
{
    grid_.resize(6);
    updateSize();
}

void Grid::reset(std::vector<std::string> newGrid)
{
    grid_ = newGrid;
    updateSize();
}

void Grid::reset(std::istream& newDataStream)
{
    grid_.clear();
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        std::string row;
        newDataStream >> row; newDataStream.ignore();
        grid_.push_back(row);
    }
    updateSize();
}

int Grid::size1() const
{
    return size1_;
}

int Grid::size2() const
{
    return size2_;
}

bool Grid::isGood() const
{
    return isGood_;
}

GridType Grid::getGrid() const
{
    return grid_;
}

char Grid::get(int height, int width) const
{
    if (height >= size1_ || width >= size2_)
    {
        return '!'; // error!
    }
    return grid_[height][width];
}

int Grid::getNumNeighboursAfterInsert(int colNum, BlockPair blockPair) const
{
    // simple version !!!
    std::pair<Point, Point> newPositions = getPositionsAfterInsert(colNum);
    std::set<Point> visitedPositions;
    visitedPositions.insert(newPositions.first);
    visitedPositions.insert(newPositions.second);
    fillNeighboursOf(newPositions.first, blockPair.first, visitedPositions);
    fillNeighboursOf(newPositions.second, blockPair.second, visitedPositions);
    visitedPositions.erase(newPositions.first);
    visitedPositions.erase(newPositions.second);
    std::ostringstream dbgMsg;
    return visitedPositions.size();
}

void Grid::updateSize()
{
    size1_ = grid_.size();
    if (size1_ == 0)
    {
        isGood_ = false;
        size2_ = 0;
        return;
    }
    size2_ = grid_[0].size();
    for (const auto& gridLine : grid_)
    {
        if (gridLine.size() != size2_)
        {
            isGood_ = false;
            size2_ = 0;
            return;
        }
    }
    isGood_ = true;
}

std::pair<Point, Point> Grid::getPositionsAfterInsert(int colNum) const
{
    std::ostringstream dbgMsg;
    if (!inBounds(0, colNum))
    {
        return std::make_pair(std::make_pair(-1,-1), std::make_pair(-1,-1));
    }
    if (grid_[0][colNum] != '.' || grid_[1][colNum] != '.')
    {
        return std::make_pair(std::make_pair(-1,-1), std::make_pair(-1,-1));
    }
    int verticalLowerPos;
    for (int i=0; i<size1_; i++)
    {
        verticalLowerPos = i;
        if (grid_[i][colNum] != '.')
        {
            verticalLowerPos = i-1;
            break;
        }
    }
    Point lowerPos = std::make_pair(verticalLowerPos, colNum);
    Point higherPos = std::make_pair(verticalLowerPos-1, colNum);
    return std::make_pair(lowerPos, higherPos);
}

void Grid::fillNeighboursOf(Point position, Block block, std::set<Point>& visitedPositions) const
{
    std::vector<Point> neighbours = getNeighbours(position, block);
    for (auto neighbour : neighbours)
    {
        auto result = visitedPositions.insert(neighbour);
        if (result.second)
        {
            fillNeighboursOf(neighbour, block, visitedPositions);
        }
    }
}

std::vector<Point> Grid::getNeighbours(Point position, Block block) const
{
    if (!inBounds(position.first, position.second))
    {
        return std::vector<Point>();
    }
    std::vector<Point> neighbours;
    for (int i=-1; i<=1; i++)
    {
        for (int j=-1; j<=1; j++)
        {
            if (i!=0 && j!=0)
            {
                continue;
            }
            int height = position.first + i;
            int width = position.second + j;
            if (!inBounds(height, width))
            {
                continue;
            }
            if (grid_[height][width] == block.color)
            {
                neighbours.push_back(std::make_pair(height, width));
            }
        }
    }
    return neighbours;
}

bool Grid::inBounds(int height, int width) const
{
    if (height < 0 || height >= size1_ || width < 0 || width >= size2_)
    {
        return false;
    }
    return true;
}

namespace GridAnalysis
{
int findShortestColumn(const Grid& grid)
{
    std::vector<int> numDotsInCols(grid.size2(), 0);
    for (int i=0; i<grid.size1(); i++)
    {
        for (int j=0; j<grid.size2(); j++)
        {
            if (grid.get(i,j) == '.')
            {
                (numDotsInCols[j])++;
            }
        }
    }
    int maxDotsInCol = 0;
    int retColNum = 0;
    for (int i=0; i<numDotsInCols.size(); i++)
    {
        if (numDotsInCols[i] > maxDotsInCol)
        {
            maxDotsInCol = numDotsInCols[i];
            retColNum = i;
        }
    }
    return retColNum;
}

std::pair<int, bool> findColWithHighestSimpleScore(const Grid& grid, BlockPair blockPair)
{
    int bestScore = 0;
    int bestCol = 0;
    for (int i=0; i<GRID_WIDTH; i++)
    {
        int score = grid.getNumNeighboursAfterInsert(i, blockPair);
        if (score > bestScore)
        {
            bestScore = score;
            bestCol = i;
        }
    }
    if (bestScore > 0)
    {
        return std::make_pair(bestCol, true);
    }
    else
    {
        return std::make_pair(bestCol, false);
    }
}
}



void GameController::startGame()
{
    printDebug("Game started!");
    simpleStrategy();
}

void GameController::simpleStrategy()
{
    int shortestColNum = 0;
    while(1)
    {
        readData(std::cin);
        fullDebugPrint();
        auto result = GridAnalysis::findColWithHighestSimpleScore(currentGrid_, nextBlocks_[0]);
        if (result.second)
        {
            writeSolution(result.first);
        }
        else
        {
            shortestColNum = GridAnalysis::findShortestColumn(currentGrid_);
            writeSolution(shortestColNum);
        }
    }
}

BlockPairQueue GameController::getNextBlocks()
{
    return nextBlocks_;
}

Grid GameController::getCurrentGrid()
{
    return currentGrid_;
}

Grid GameController::getOpponentGrid()
{
    return opponentGrid_;
}

void GameController::readData(std::istream& input)
{
    BlockPairQueue nextBlocks;
    GridType myNewGrid;
    GridType opponentNewGrid;
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
    opponentGrid_.reset(opponentNewGrid);
}

void GameController::writeSolution(int column)
{
    std::cout << column << std::endl;
}

void GameController::fullDebugPrint()
{
    printDebug("Next blocks:");
    for (const auto& blockPair : nextBlocks_)
    {
        std::string blockPairStr;
        blockPairStr += '[';
        blockPairStr += blockPair.first.color;
        blockPairStr += ' ';
        blockPairStr += blockPair.second.color;
        blockPairStr += ']';
        printDebug(blockPairStr);
    }
    printDebug("My current grid: ");
    for (const auto& gridLine : currentGrid_.getGrid())
    {
        printDebug(gridLine);
    }
    printDebug("My opponent's grid: ");
    for (const auto& gridLine : opponentGrid_.getGrid())
    {
        printDebug(gridLine);
    }
}
