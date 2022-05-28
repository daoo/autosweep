#include <catch2/catch.hpp>

#include "autosweep/board.h"
#include "autosweep/bruteforce.h"

TEST_CASE("aoeu", "[Bruteforce]") {
  Board board = Board::FromString(
      "01\n"
      "0-");

  auto solutions = Bruteforce(board);

  REQUIRE(solutions.size() == 1);
}
