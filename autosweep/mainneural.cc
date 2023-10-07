#include <iostream>
#include <opencv4/opencv2/core.hpp>

#include "autosweep/board.h"
#include "autosweep/boardstream.h"
#include "autosweep/desktop.h"
#include "autosweep/detector.h"
#include "autosweep/network.h"
#include "autosweep/neuralboard.h"

namespace {

void LeftClick(
    const Desktop& desktop, const BoardLocation& location, Cell cell) {
  /* std::cout << "LeftClick(" << cell.row << ", " << cell.col << ")\n"; */
  desktop.LeftClick(location.CellCenter(cell.row, cell.col));
}

size_t Inner(
    const Desktop& desktop, const BoardLocation& location, Network& network,
    double eta) {
  size_t clicks = 0;
  while (true) {
    /* std::cout << "Taking screenshot of " << location.Board() << "\n"; */
    Board board = ParseBoard(desktop.Capture(location.Board()));
    if (board.IsWon()) {
      /* std::cout << "Victory!\n"; */
      return clicks;
    }
    if (board.IsLost()) {
      /* std::cout << "Lost!\n"; */
      return clicks;
    }

    cv::Mat input = GenerateInputLayer(board);
    cv::Mat result = ReshapeResult(board, network.FeedForward(input));

    double minVal;
    cv::Point minLoc;
    cv::minMaxLoc(
        result, &minVal, nullptr, &minLoc, nullptr, UnknownCellMask(board));
    Cell lowestRiskCell = board.at(minLoc.y, minLoc.x);
    /* std::cout << "Lowest risk of mine at " << minLoc << " (" << minVal <<
     * ")\n"; */
    LeftClick(desktop, location, lowestRiskCell);
    ++clicks;

    Board board2 = ParseBoard(desktop.Capture(location.Board()));

    cv::Mat output = GenerateOutputLayer(board2);
    network.Update(network.BackPropagate(input, output), eta);
  }
}
}  // namespace

int main() {
  Desktop desktop;
  cv::Rect display_rectangle = desktop.GetDisplayRectangle();
  /* std::cout << "Taking screenshot of " << display_rectangle << "\n"; */
  cv::Mat initial_screenshot = desktop.Capture(display_rectangle);
  BoardLocation location = BoardLocation::Find(initial_screenshot);

  // TODO: Count cells without parsing
  Board board = ParseBoard(initial_screenshot(location.Board()));
  size_t cells = board.rows() * board.cols();
  Network network({cells, cells, cells});
  constexpr double eta = 3;
  size_t games = 0;
  size_t total = 0;
  while (true) {
    desktop.LeftClick(location.SmileyCenter());
    LeftClick(desktop, location, board.at(board.rows() / 2, board.cols() / 2));
    auto clicks = Inner(desktop, location, network, eta);

    total += clicks;
    ++games;
    std::cout << "Game: " << games;
    std::cout << ", Clicks: " << clicks;
    std::cout << ", Avg: " << static_cast<double>(total) / games;
    std::cout << '\n';
  }
}
