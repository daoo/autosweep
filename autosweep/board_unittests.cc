#include <catch2/catch.hpp>

#include "autosweep/board.h"

TEST_CASE("FromString", "[Board]") {
  SECTION("example input, expected output") {
    std::string string(
        "0123\n"
        "4567\n"
        "89fm\n"
        "----\n");

    Board board = Board::FromString(string);

    REQUIRE(board.rows() == 4);
    REQUIRE(board.cols() == 4);
    REQUIRE(board.at(0, 0).value == CELL_0);
    REQUIRE(board.at(0, 1).value == CELL_1);
    REQUIRE(board.at(0, 2).value == CELL_2);
    REQUIRE(board.at(0, 3).value == CELL_3);
    REQUIRE(board.at(1, 0).value == CELL_4);
    REQUIRE(board.at(1, 1).value == CELL_5);
    REQUIRE(board.at(1, 2).value == CELL_6);
    REQUIRE(board.at(1, 3).value == CELL_7);
    REQUIRE(board.at(2, 0).value == CELL_8);
    REQUIRE(board.at(2, 1).value == CELL_9);
    REQUIRE(board.at(2, 2).value == CELL_FLAG);
    REQUIRE(board.at(2, 3).value == CELL_MINE);
    REQUIRE(board.at(3, 0).value == CELL_UNKNOWN);
    REQUIRE(board.at(3, 1).value == CELL_UNKNOWN);
    REQUIRE(board.at(3, 2).value == CELL_UNKNOWN);
    REQUIRE(board.at(3, 3).value == CELL_UNKNOWN);
  }
}

TEST_CASE("ComputeKnownNeighboringCellStates", "[Board]") {
  SECTION("one unknown cell with neighboring ones, gives new flag") {
    Board board = Board::FromString(
        "-1\n"
        "10\n");

    std::unordered_set<Cell> new_flags, new_clicks;
    ComputeKnownNeighboringCellStates(board, &new_flags, &new_clicks);

    REQUIRE(new_flags.size() == 1);
    REQUIRE(new_flags.cbegin()->row == 0);
    REQUIRE(new_flags.cbegin()->col == 0);
    REQUIRE(new_clicks.empty());
  }

  SECTION("case where algorithm does not work, no clicks or flags") {
    Board board = Board::FromString(
        "------\n"
        "--111-\n"
        "-2101-\n"
        "--212-\n"
        "---1--\n"
        "------\n");

    std::unordered_set<Cell> new_flags, new_clicks;
    ComputeKnownNeighboringCellStates(board, &new_flags, &new_clicks);

    REQUIRE(new_flags.empty());
    REQUIRE(new_clicks.empty());
  }
}

TEST_CASE("UnknownCells", "[Board]") {
  SECTION("example input, expected cell") {
    Board board = Board::FromString(
        "1m\n"
        "--\n");

    auto result = UnknownCells(board);

    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == Cell{1, 0, CELL_UNKNOWN});
    REQUIRE(result[1] == Cell{1, 1, CELL_UNKNOWN});
  }
}
