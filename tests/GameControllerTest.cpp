#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <fstream>
#include <cmath>
#include "GameController.hpp"

using namespace std;


TEST(SampleTest, shouldPass)
{
    ASSERT_EQ(0,0);
}

TEST(BlockConstructorShould, castIntToChar)
{
    char refCharNull = '.';
    char refChar = '1';
    int refInt = 1;
    Block b1, b2(refChar), b3(refInt);
    ASSERT_EQ(refCharNull, b1.color);
    ASSERT_EQ(refChar, b2.color);
    ASSERT_EQ(refChar, b3.color);
}

class GameControllerShould: public testing::Test
{
public:
    GameController sut_;
};

class GridShould: public testing::Test
{
public:
    Grid sut_;
};

class GridAnalysisShould: public testing::Test
{
public:
    Grid mockGrid_;
};


TEST_F(GameControllerShould, readDataFromInputStream)
{
    ifstream inputData;
    inputData.open("data/mockEmptyData.dat");
    ASSERT_TRUE(inputData.is_open());
    sut_.readData(inputData);

    char pair11Ref = '1';
    char pair12Ref = '1';
    char pair81Ref = '3';
    char pair82Ref = '3';
    char gridEmptyCell = '.';
    char gridError = '!';
    BlockPairQueue nextBlocks = sut_.getNextBlocks();
    Grid currentGrid = sut_.getCurrentGrid();
    Grid opponentGrid = sut_.getOpponentGrid();

    ASSERT_EQ(QUEUE_LENGTH, nextBlocks.size());
    ASSERT_EQ(GRID_HEIGHT, currentGrid.size1());
    ASSERT_EQ(GRID_HEIGHT, opponentGrid.size1());
    ASSERT_EQ(GRID_WIDTH, currentGrid.size2());
    ASSERT_EQ(GRID_WIDTH, opponentGrid.size2());
    ASSERT_TRUE(currentGrid.isGood());
    ASSERT_TRUE(opponentGrid.isGood());

    ASSERT_EQ(std::make_pair(Block(pair11Ref), Block(pair12Ref)), *(nextBlocks.begin()));
    ASSERT_EQ(std::make_pair(Block(pair81Ref), Block(pair82Ref)), *(nextBlocks.end()-1));

    ASSERT_EQ(gridEmptyCell, currentGrid.get(4,4));
    ASSERT_EQ(gridError, currentGrid.get(4,7));

    inputData.close();
}

TEST_F(GridShould, returnCorrectNumOfNeighboursForDifferentBlockPairs)
{
    ifstream mockGridStream;
    mockGridStream.open("data/mockGrid2.dat");
    ASSERT_TRUE(mockGridStream.is_open());
    sut_.reset(mockGridStream);
    mockGridStream.close();

    ASSERT_EQ(GRID_HEIGHT, sut_.size1());
    ASSERT_EQ(GRID_WIDTH, sut_.size2());


    BlockPair bp1 = std::make_pair(Block('1'), Block('1'));
    BlockPair bp2 = std::make_pair(Block('2'), Block('2'));
    BlockPair bp3 = std::make_pair(Block('3'), Block('3'));
    BlockPair bp4 = std::make_pair(Block('4'), Block('4'));
    BlockPair bp5 = std::make_pair(Block('5'), Block('5'));
    BlockPair bpx = std::make_pair(Block('0'), Block('0'));

    ASSERT_EQ(6, sut_.getNumNeighboursAfterInsert(4, bp1));
    ASSERT_EQ(0, sut_.getNumNeighboursAfterInsert(1, bp2));
    ASSERT_EQ(0, sut_.getNumNeighboursAfterInsert(3, bp3));
    ASSERT_EQ(2, sut_.getNumNeighboursAfterInsert(2, bp4));
    ASSERT_EQ(0, sut_.getNumNeighboursAfterInsert(4, bp5));
    ASSERT_EQ(0, sut_.getNumNeighboursAfterInsert(0, bpx));
    ASSERT_EQ(0, sut_.getNumNeighboursAfterInsert(5, bp4));
}

TEST_F(GridAnalysisShould, findTheShortestColumn)
{
    ifstream mockGridStream;
    mockGridStream.open("data/mockGrid1.dat");
    ASSERT_TRUE(mockGridStream.is_open());
    mockGrid_.reset(mockGridStream);
    mockGridStream.close();

    ASSERT_EQ(GRID_HEIGHT, mockGrid_.size1());
    ASSERT_EQ(GRID_WIDTH, mockGrid_.size2());

    int refShortestColumnNo = 1;
    int shortestColumnNo = GridAnalysis::findShortestColumn(mockGrid_);

    ASSERT_EQ(refShortestColumnNo, shortestColumnNo);
}

TEST_F(GridAnalysisShould, select4thColAsMostAttractive)
{
    ifstream mockGridStream;
    mockGridStream.open("data/mockGrid1.dat");
    ASSERT_TRUE(mockGridStream.is_open());
    mockGrid_.reset(mockGridStream);
    mockGridStream.close();

    ASSERT_EQ(GRID_HEIGHT, mockGrid_.size1());
    ASSERT_EQ(GRID_WIDTH, mockGrid_.size2());

    BlockPair refBlockPair = std::make_pair(Block('1'), Block('1'));
    int refBestCol = 4;
    int bestCol = GridAnalysis::findColWithHighestSimpleScore(mockGrid_, refBlockPair).first;

    ASSERT_EQ(refBestCol, bestCol);
}