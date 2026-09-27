#include <gtest/gtest.h>

#include <spr/p10196.hpp>

#include <sstream>
#include <string>

namespace {

auto run(const std::string& input) -> std::string {
    std::istringstream in(input);
    std::ostringstream out;

    spr::p10196::solve(in, out);

    return out.str();
}

auto game(std::initializer_list<const char*> rows) -> std::string {
    std::string board;
    for (const auto* row : rows) {
        board += row;
        board += '\n';
    }
    return board;
}

const auto empty_board = game({
  "........",
  "........",
  "........",
  "........",
  "........",
  "........",
  "........",
  "........",
});

} // namespace

TEST(p10196, sampleInput) {
    const std::string input = game(
                                  {"..k.....",
                                   "ppp.pppp",
                                   "........",
                                   ".R...B..",
                                   "........",
                                   "........",
                                   "PPPPPPPP",
                                   "K......."}
                              )
                            + "\n"
                            + game(
                                  {"rnbqkbnr",
                                   "pppppppp",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "PPPPPPPP",
                                   "RNBQKBNR"}
                            )
                            + "\n"
                            + game(
                                  {"rnbqk.nr",
                                   "ppp..ppp",
                                   "....p...",
                                   "...p....",
                                   ".bPP....",
                                   ".....N..",
                                   "PP..PPPP",
                                   "RNBQKB.R"}
                            )
                            + "\n" + empty_board;

    const std::string expected = "Game #1: black king is in check.\n"
                                 "Game #2: no king is in check.\n"
                                 "Game #3: white king is in check.\n";

    EXPECT_EQ(run(input), expected);
}

TEST(p10196, emptyBoardTerminatesImmediately) {
    EXPECT_EQ(run(empty_board), "");
}

TEST(p10196, rookChecksBlackKingAlongRow) {
    const std::string input = game(
                                  {"R...k...",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "....K..."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: black king is in check.\n");
}

TEST(p10196, rookBlockedByOwnPiece) {
    const std::string input = game(
                                  {"R.P.k...",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "....K..."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: no king is in check.\n");
}

TEST(p10196, bishopChecksWhiteKingDiagonally) {
    // b at (4,1), diagonal (5,2) (6,3) (7,4)=K
    const std::string input = game(
                                  {"k.......",
                                   "........",
                                   "........",
                                   "........",
                                   ".b......",
                                   "........",
                                   "........",
                                   "....K..."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: white king is in check.\n");
}

TEST(p10196, knightChecksBlackKingOverPieces) {
    // n at (5,3) attacks (7,4)=K; blockers on path are ignored
    const std::string input = game(
                                  {"........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "...n....",
                                   "PPPPPPPP",
                                   "....K..k"}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: white king is in check.\n");
}

TEST(p10196, queenChecksWhiteKingAlongColumn) {
    // q at (2,4) same column as K at (7,4), path clear
    const std::string input = game(
                                  {"....k...",
                                   "........",
                                   "....q...",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "....K..."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: white king is in check.\n");
}

TEST(p10196, blackPawnChecksWhiteKingDiagonallyDown) {
    // p at (6,3) takes on (7,2) and (7,4)=K
    const std::string input = game(
                                  {"........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "...p....",
                                   "....K..k"}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: white king is in check.\n");
}

TEST(p10196, whitePawnChecksBlackKingDiagonallyUp) {
    // P at (1,3) takes on (0,2) and (0,4)=k
    const std::string input = game(
                                  {"....k...",
                                   "...P....",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "....K..."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: black king is in check.\n");
}

TEST(p10196, whitePawnDoesNotCheckBackwards) {
    // P at (6,5) takes up-diagonal only; K at (7,4) is behind it
    const std::string input = game(
                                  {"........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   ".....P..",
                                   "....K..k"}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: no king is in check.\n");
}

TEST(p10196, bothKingsSafe) {
    const std::string input = game(
                                  {"....k...",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "........",
                                   "..K....."}
                              )
                            + "\n" + empty_board;

    EXPECT_EQ(run(input), "Game #1: no king is in check.\n");
}

TEST(p10196, gameNumberingIncrements) {
    const auto board1 = game(
        {"R...k...",
         "........",
         "........",
         "........",
         "........",
         "........",
         "........",
         "....K..."}
    );
    const auto board2 = game(
        {"....k.R.",
         "........",
         "........",
         "........",
         "........",
         "........",
         "........",
         "....K..."}
    );

    const std::string input = board1 + "\n" + board2 + "\n" + empty_board;

    const std::string expected = "Game #1: black king is in check.\n"
                                 "Game #2: black king is in check.\n";

    EXPECT_EQ(run(input), expected);
}
