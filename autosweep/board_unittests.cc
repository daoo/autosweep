#include "autosweep/board.h"

#include <catch2/catch.hpp>

TEST_CASE("FromString", "[Board]") {
  SECTION("example input, expected output") {
    std::string string("-1\n"
                       "10\n");

    Board board = Board::FromString(string);

    REQUIRE(board.rows() == 2);
    REQUIRE(board.cols() == 2);
    REQUIRE(board.at(0, 0).value == CELL_UNKNOWN);
    REQUIRE(board.at(0, 1).value == 1);
    REQUIRE(board.at(1, 0).value == 1);
    REQUIRE(board.at(1, 1).value == 0);
  }
}

TEST_CASE("Changes", "[Board]") {
  SECTION("one unknown cell with neighboring ones, gives new flag") {
    Board board = Board::FromString("-1\n"
                                    "10\n");

    std::unordered_set<Cell> new_flags, new_clicks;
    Changes(board, &new_flags, &new_clicks);

    REQUIRE(new_flags.size() == 1);
    REQUIRE(new_flags.cbegin()->row == 0);
    REQUIRE(new_flags.cbegin()->col == 0);
    REQUIRE(new_clicks.empty());
  }

  SECTION("case where algorithm does not work, no clicks or flags") {
    Board board = Board::FromString("------\n"
                                    "--111-\n"
                                    "-2101-\n"
                                    "--212-\n"
                                    "---1--\n"
                                    "------\n");

    std::unordered_set<Cell> new_flags, new_clicks;
    Changes(board, &new_flags, &new_clicks);

    REQUIRE(new_flags.empty());
    REQUIRE(new_clicks.empty());
  }
}

TEST_CASE("ACellWithMostNeighboringMines", "[Board]") {
  SECTION("example input, expected cell") {
    Board board = Board::FromString("--1\n"
                                    "--2\n"
                                    "12f\n");

    Cell cell = ACellWithMostNeighboringMines(board);

    REQUIRE(cell.row == 1);
    REQUIRE(cell.col == 1);
  }
}
