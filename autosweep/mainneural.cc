#include <iostream>
#include <opencv4/opencv2/core.hpp>

#include "autosweep/board.h"
#include "autosweep/desktop.h"
#include "autosweep/detector.h"
#include "autosweep/network.h"
#include "autosweep/neuralboard.h"
#include "autosweep/stream.h"

namespace {

void LeftClick(
    const Desktop& desktop, const BoardLocation& location, Cell cell) {
  std::cout << "LeftClick(" << cell.row << ", " << cell.col << ")\n";
  desktop.LeftClick(location.CellCenter(cell.row, cell.col));
}

void RightClick(
    const Desktop& desktop, const BoardLocation& location, Cell cell) {
  std::cout << "RightClick(" << cell.row << ", " << cell.col << ")\n";
  desktop.RightClick(location.CellCenter(cell.row, cell.col));
}

void Inner(
    const Desktop& desktop, const BoardLocation& location, Network& network,
    double eta) {
  while (true) {
    std::cout << "Taking screenshot of " << location.Board() << "\n";
    Board board = ParseBoard(desktop.Capture(location.Board()));
    if (board.IsWon()) {
      std::cout << "Victory!\n";
      return;
    }
    if (board.IsLost()) {
      std::cout << "Lost!\n";
      return;
    }

    cv::Mat input = GenerateInputLayer(board);
    cv::Mat result = ReshapeResult(board, network.FeedForward(input));
    double maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(result, nullptr, &maxVal, nullptr, &maxLoc);
    std::cout << "Expected mine at " << maxLoc << " (" << maxVal << ")\n";
    LeftClick(desktop, location, board.at(maxLoc.x, maxLoc.y));

    Board board2 = ParseBoard(desktop.Capture(location.Board()));

    cv::Mat output = GenerateOutputLayer(board2);
    network.Update(network.BackPropagate(input, output), eta);
  }
}
}  // namespace

int main() {
  Desktop desktop;
  cv::Rect display_rectangle = desktop.GetDisplayRectangle();
  std::cout << "Taking screenshot of " << display_rectangle << "\n";
  cv::Mat initial_screenshot = desktop.Capture(display_rectangle);
  BoardLocation location = BoardLocation::Find(initial_screenshot);

  Board board = ParseBoard(initial_screenshot(location.Board()));
  size_t cells = board.rows() * board.cols();
  Network network({cells, cells, cells});
  constexpr double eta = 3;
  while (true) {
    desktop.LeftClick(location.SmileyCenter());
    LeftClick(desktop, location, board.at(board.rows() / 2, board.cols() / 2));
    Inner(desktop, location, network, eta);
  }
}
