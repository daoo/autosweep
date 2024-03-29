#include <opencv4/opencv2/imgproc.hpp>

#include "autosweep/board.h"
#include "autosweep/detector.h"

namespace {

static const struct {
  int width;
  int height;
  int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  uchar pixel_data[26 * 26 * 3 + 1];
} smiley_face = {
    26,
    26,
    3,
    "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{"
    "{{{\275\275\275{{{\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\275\275\275{{{{{{\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\275"
    "\275\275{{{{{{{{{\377\377\377\377\377\377\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275{{{{{{{{"
    "{{{{\377\377\377\377\377\377\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275{{{{{{{{{{{{\377\377"
    "\377\377\377\377\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\000\000\000\000\000\000\000\000"
    "\000\000\000\000\000\000\000\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275{{{{{{{{{{{{\377\377\377\377\377\377\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\000\000\000\000\000\000\377"
    "\377\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\000\000\000\000\000\000\275\275"
    "\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275{{{{{{{{{{{{\377\377\377\377\377\377\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\000\000\000\377"
    "\377\000"
    "\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\377"
    "\377\000\000\000\000\275\275\275\275\275\275\275\275\275\275\275\275{{{{{{"
    "{{{{{{"
    "\377\377\377\377\377\377\275\275\275\275\275\275\275\275\275\275\275\275"
    "\000\000\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\377\377\000\000\000\000\275\275"
    "\275\275\275\275"
    "\275\275\275{{{{{{{{{{{{\377\377\377\377\377\377\275\275\275\275\275\275"
    "\275\275\275\000\000\000\377\377\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\377"
    "\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377"
    "\377\000\377\377"
    "\000\000\000\000\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275"
    "\275\275\275\275\275\275\275\275\000\000\000\377\377\000\377\377\000\377"
    "\377\000\000\000"
    "\000\000\000\000\377\377\000\377\377\000\377\377\000\000\000\000\000\000"
    "\000\377\377\000\377\377\000\377"
    "\377\000\000\000\000\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377"
    "\275\275\275\275\275\275\000\000\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\000"
    "\000\000\000\000\000\377\377\000\377\377\000\377\377\000\000\000\000\000"
    "\000\000\377\377\000\377\377\000"
    "\377\377\000\377\377\000\000\000\000\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377"
    "\275\275\275\275\275\275\000\000\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\377"
    "\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377"
    "\377\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\000\000\000\275\275\275{{{{{{{{{{"
    "{{\377\377\377"
    "\377\377\377\275\275\275\275\275\275\000\000\000\377\377\000\377\377\000"
    "\377\377\000"
    "\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000"
    "\377\377\000\377"
    "\377\000\377\377\000\377\377\000\377\377\000\377\377\000\000\000\000\275"
    "\275\275{{{{{{{{"
    "{{{{"
    "\377\377\377\377\377\377\275\275\275\275\275\275\000\000\000\377\377\000"
    "\377"
    "\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377"
    "\377\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377"
    "\000\000\000\000\275"
    "\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275\275\275\275\275\275\000\000"
    "\000\377\377\000\377\377\000\377\377\000\000\000\000\377\377\000\377\377"
    "\000\377\377\000\377"
    "\377\000\377\377\000\377\377\000\377\377\000\000\000\000\377\377\000\377"
    "\377\000\377\377\000"
    "\000\000\000\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275\275\275\275\275"
    "\275\275\275\275\000\000\000\377\377\000\377\377\000\377\377\000\000\000"
    "\000\377\377\000\377"
    "\377\000\377\377\000\377\377\000\377\377\000\000\000\000\377\377\000\377"
    "\377\000\377\377\000"
    "\000\000\000\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275\275"
    "\275\275\275\275\275\275\275\000\000\000\377\377\000\377\377\000\377\377"
    "\000\377\377"
    "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000"
    "\000\000\000\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275\275"
    "\275\275\275\275\275\275\275\275\275\275\000\000\000\377\377\000\377\377"
    "\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377"
    "\000\377\377\000"
    "\377\377\000\000\000\000\275\275\275\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377"
    "\377\377\377\377\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\000\000\000\377\377\000\377\377\000\377\377\000\377\377\000\377\377"
    "\000\377\377\000\377"
    "\377\000\377\377\000\377\377\000\000\000\000\275\275\275\275\275\275\275"
    "\275\275\275"
    "\275\275{{{{{{{{{{{{\377\377\377\377\377\377\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\000\000\000\000\000\000\377"
    "\377\000\377\377"
    "\000\377\377\000\377\377\000\377\377\000\000\000\000\000\000\000\275\275"
    "\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275{{{{{{{{{{{{\377\377\377\377\377\377\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\000\000\000\000\000\000\000\000\000\000\000\000\000"
    "\000\000\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275{{{{{{{{{{{{"
    "\377\377\377\377\377\377\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275\275"
    "\275\275\275\275\275\275\275\275\275\275\275\275{{{{{{{{{{{{\377\377\377"
    "\275\275\275{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{"
    "{{{{{{{{{{{{\275\275\275{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{"
    "{{{{{{{{{{{{{{{{{{{{{{{{\275\275\275{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{"
    "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{",
};

const int BOARD_PIXEL_X_PADDING = 8;
const int BOARD_PIXEL_Y_TOP_PADDING = 50;
const int BOARD_PIXEL_Y_BOTTOM_PADDING = 10;
const int BOARD_PIXEL_CELL_SIDE = 16;
const int BOARD_PIXEL_Y_PADDING = 11;

cv::Rect FindSmileyLocation(const cv::Mat& screenshot) {
  cv::Mat smiley_face_rgb(
      smiley_face.width, smiley_face.height, CV_8UC3,
      static_cast<void*>(const_cast<uchar*>(smiley_face.pixel_data)));
  cv::Mat smiley_face_bgr;
  cv::cvtColor(smiley_face_rgb, smiley_face_bgr, cv::COLOR_RGB2BGR);

  cv::Mat match_result;
  cv::matchTemplate(
      screenshot, smiley_face_bgr, match_result, cv::TM_SQDIFF_NORMED);
  cv::Point smiley_location, maxLoc;
  double minVal, maxVal;
  cv::minMaxLoc(match_result, &minVal, &maxVal, &smiley_location, &maxLoc);
  return cv::Rect{
      smiley_location.x, smiley_location.y, smiley_face.width,
      smiley_face.height};
}

}  // namespace

BoardLocation::BoardLocation(cv::Rect board, cv::Rect smiely)
    : board_(std::move(board)), smiley_(std::move(smiely)) {
  auto minimumWidth = BOARD_PIXEL_X_PADDING * 2;
  if (board_.width < minimumWidth) {
    throw std::runtime_error(
        "Board width " + std::to_string(board_.width) + " less than minimum " +
        std::to_string(minimumWidth));
  }
  auto minimumHeight = BOARD_PIXEL_Y_TOP_PADDING + BOARD_PIXEL_Y_BOTTOM_PADDING;
  if (board_.height < minimumHeight) {
    throw std::runtime_error(
        "Board width " + std::to_string(board_.height) + " less than minimum " +
        std::to_string(minimumHeight));
  }
}

BoardLocation BoardLocation::Find(const cv::Mat& screenshot) {
  cv::Rect smiley_location = FindSmileyLocation(screenshot);
  assert(smiley_location.y >= BOARD_PIXEL_Y_PADDING);
  int top = smiley_location.y - BOARD_PIXEL_Y_PADDING;

  const cv::Vec3b THAT_GREY_COLOR(189, 189, 189);

  int left, right, bottom;
  for (left = smiley_location.x; left >= 0; --left) {
    cv::Vec3b pixel = screenshot.at<cv::Vec3b>(top, left);
    if (pixel != THAT_GREY_COLOR) break;
  }
  left++;
  for (right = smiley_location.x; right < screenshot.cols; ++right) {
    cv::Vec3b pixel = screenshot.at<cv::Vec3b>(top, right);
    if (pixel != THAT_GREY_COLOR) break;
  }
  right--;
  for (bottom = top; bottom < screenshot.rows; ++bottom) {
    cv::Vec3b pixel = screenshot.at<cv::Vec3b>(bottom, left);
    if (pixel != THAT_GREY_COLOR) break;
  }
  int width = right - left;
  int height = bottom - top;
  cv::Rect board_location{left, top, width, height};

  return BoardLocation(board_location, smiley_location);
}

cv::Point2i BoardLocation::CellCenter(int row, int col) const {
  int offset = BOARD_PIXEL_CELL_SIDE / 2;
  int x =
      offset + board_.x + BOARD_PIXEL_X_PADDING + col * BOARD_PIXEL_CELL_SIDE;
  int y = offset + board_.y + BOARD_PIXEL_Y_TOP_PADDING +
      row * BOARD_PIXEL_CELL_SIDE;

  bool badX = x <= board_.tl().x || x >= board_.br().x;
  bool badY = y <= board_.tl().y || y >= board_.br().y;
  if (badX || badY) {
    auto cell_index_str = std::to_string(row) + " x " + std::to_string(col);
    auto pixel_coordinate_str =
        "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    auto board_top_left_str = "(" + std::to_string(board_.tl().x) + ", " +
        std::to_string(board_.tl().y) + ")";
    auto board_bottom_right_str = "(" + std::to_string(board_.br().x) + ", " +
        std::to_string(board_.br().y) + ")";
    auto board_str =
        "[" + board_top_left_str + ", " + board_bottom_right_str + "]";
    throw std::runtime_error(
        "Cell " + cell_index_str + " at pixel " + pixel_coordinate_str +
        " is outside of board region " + board_str + ".");
  }
  return {x, y};
}

cv::Point2i BoardLocation::SmileyCenter() const {
  return {smiley_.x + smiley_.width / 2, smiley_.y + smiley_.height / 2};
}

namespace {

uchar ParseCell(const cv::Mat& cell) {
  cv::Scalar color = cv::mean(cell);

  cv::Scalar unknown_color(189, 189, 189);
  cv::Scalar flag_color(160.207031, 160.207031, 177.140625);
  cv::Scalar zero_color(181.007812, 181.007812, 181.007812);
  cv::Scalar one_color(191.320312, 151.476562, 151.476562);
  cv::Scalar two_color(133.019531, 164.250000, 133.019531);
  cv::Scalar three_color(135.234375, 135.234375, 196.992188);
  cv::Scalar four_color(166.570312, 139.664062, 139.664062);
  cv::Scalar five_color(129.328125, 129.328125, 162.960938);
  cv::Scalar six_color(162.445312, 162.445312, 127.851562);
  cv::Scalar seven_color(148.523438, 148.523438, 148.523438);
  cv::Scalar eight_color(161.414062, 161.414062, 161.414062);
  cv::Scalar mine_color(18.878906, 18.878906, 162.316406);
  cv::Scalar clicked_mine_color(125.191, 125.191, 125.191);
  cv::Scalar misplaced_flag_mine_color(108.914, 108.914, 154.734);

  if (cv::norm(color, unknown_color) < 0.1) return CELL_UNKNOWN;
  if (cv::norm(color, flag_color) < 0.1) return CELL_FLAG;
  if (cv::norm(color, mine_color) < 0.1) return CELL_MINE;
  if (cv::norm(color, clicked_mine_color) < 0.1) return CELL_MINE;
  if (cv::norm(color, misplaced_flag_mine_color) < 0.1) return CELL_MINE;
  if (cv::norm(color, zero_color) < 0.1) return 0;
  if (cv::norm(color, one_color) < 0.1) return 1;
  if (cv::norm(color, two_color) < 0.1) return 2;
  if (cv::norm(color, three_color) < 0.1) return 3;
  if (cv::norm(color, four_color) < 0.1) return 4;
  if (cv::norm(color, five_color) < 0.1) return 5;
  if (cv::norm(color, six_color) < 0.1) return 6;
  if (cv::norm(color, seven_color) < 0.1) return 7;
  if (cv::norm(color, eight_color) < 0.1) return 8;

  std::stringstream error;
  error << "Found cell with unknown mean color (" << color[0] << ", "
        << color[1] << ", " << color[2] << ").";
  throw std::runtime_error(error.str());
}

}  // namespace

Board ParseBoard(const cv::Mat& screenshot) {
  int cell_grid_width = screenshot.cols - BOARD_PIXEL_X_PADDING * 2;
  int cell_grid_height = screenshot.rows - BOARD_PIXEL_Y_TOP_PADDING -
      BOARD_PIXEL_Y_BOTTOM_PADDING;
  int cols = 1 + cell_grid_width / BOARD_PIXEL_CELL_SIDE;
  int rows = 1 + cell_grid_height / BOARD_PIXEL_CELL_SIDE;

  cv::Mat cells(rows, cols, CV_8UC1);
  for (int i = 0; i < cols; ++i) {
    int x1 = BOARD_PIXEL_X_PADDING + i * BOARD_PIXEL_CELL_SIDE;
    int x2 = x1 + BOARD_PIXEL_CELL_SIDE;
    for (int j = 0; j < rows; ++j) {
      int y1 = BOARD_PIXEL_Y_TOP_PADDING + j * BOARD_PIXEL_CELL_SIDE;
      int y2 = y1 + BOARD_PIXEL_CELL_SIDE;
      cv::Mat cell = screenshot(cv::Range(y1, y2), cv::Range(x1, x2));
      cells.at<uchar>(j, i) = ParseCell(cell);
    }
  }

  return Board(cells);
}
