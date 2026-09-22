#include <gtest/gtest.h>

#include <spr/p10189.hpp>

#include <sstream>
#include <string>

namespace {

std::string run(const std::string &input) {
  std::istringstream in(input);
  std::ostringstream out;

  spr::p10189::solve(in, out);

  return out.str();
}

} // namespace

TEST(p10189, sampleInput) {
  const std::string input = "4 4\n"
                            "*...\n"
                            "....\n"
                            ".*..\n"
                            "....\n"
                            "3 5\n"
                            "**...\n"
                            ".....\n"
                            ".*...\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "*100\n"
                               "2210\n"
                               "1*10\n"
                               "1110\n"
                               "\n"
                               "Field #2:\n"
                               "**100\n"
                               "33200\n"
                               "1*100\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, singleEmptyCell) {
  const std::string input = "1 1\n"
                            ".\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "0\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, singleMine) {
  const std::string input = "1 1\n"
                            "*\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "*\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, mineInCenterTouchesAllEightNeighbors) {
  const std::string input = "3 3\n"
                            "...\n"
                            ".*.\n"
                            "...\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "111\n"
                               "1*1\n"
                               "111\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, mineInCorner) {
  const std::string input = "3 3\n"
                            "*..\n"
                            "...\n"
                            "...\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "*10\n"
                               "110\n"
                               "000\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, countsUpToEightAdjacentMines) {
  const std::string input = "3 3\n"
                            "***\n"
                            "*.*\n"
                            "***\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "***\n"
                               "*8*\n"
                               "***\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, rectangularField) {
  const std::string input = "2 5\n"
                            "*...*\n"
                            ".....\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "*101*\n"
                               "11011\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, multipleFieldsAreNumberedAndSeparatedByBlankLine) {
  const std::string input = "1 2\n"
                            "*.\n"
                            "2 1\n"
                            ".\n"
                            "*\n"
                            "1 1\n"
                            ".\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "*1\n"
                               "\n"
                               "Field #2:\n"
                               "1\n"
                               "*\n"
                               "\n"
                               "Field #3:\n"
                               "0\n";

  EXPECT_EQ(run(input), expected);
}

TEST(p10189, terminationMarkerProducesNoField) { EXPECT_EQ(run("0 0\n"), ""); }

TEST(p10189, allEmptyField) {
  const std::string input = "3 4\n"
                            "....\n"
                            "....\n"
                            "....\n"
                            "0 0\n";

  const std::string expected = "Field #1:\n"
                               "0000\n"
                               "0000\n"
                               "0000\n";

  EXPECT_EQ(run(input), expected);
}