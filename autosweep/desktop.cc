#include <iostream>
#include <opencv4/opencv2/imgproc.hpp>

#include "autosweep/desktop.h"

const int DELAY_AFTER_CLICK = 70000;
const int DELAY_AFTER_MOVE = 1;
const int DELAY_BETWEEN_PRESS_AND_RELEASE = 1;

void Desktop::Move(int x, int y) const {
  XEvent event;
  XQueryPointer(
      _display, _window, &event.xbutton.root, &event.xbutton.window,
      &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x,
      &event.xbutton.y, &event.xbutton.state);
  int dx = -event.xbutton.x + x;
  int dy = -event.xbutton.y + y;
  XWarpPointer(_display, None, None, 0, 0, 0, 0, dx, dy);
  usleep(DELAY_AFTER_MOVE);
}

void Desktop::Click(unsigned int button) const {
  XEvent event;
  memset(&event, 0, sizeof(event));
  event.xbutton.button = button;
  event.xbutton.same_screen = true;
  event.xbutton.subwindow = _window;
  while (event.xbutton.subwindow) {
    event.xbutton.window = event.xbutton.subwindow;
    XQueryPointer(
        _display, event.xbutton.window, &event.xbutton.root,
        &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root,
        &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
  }
  // Press
  event.type = ButtonPress;
  if (XSendEvent(_display, PointerWindow, true, ButtonPressMask, &event) == 0)
    std::cerr << "Error to send the event!\n";
  XFlush(_display);
  usleep(DELAY_BETWEEN_PRESS_AND_RELEASE);
  // Release
  event.type = ButtonRelease;
  if (XSendEvent(_display, PointerWindow, true, ButtonReleaseMask, &event) == 0)
    std::cerr << "Error to send the event!\n";
  XFlush(_display);
  usleep(DELAY_AFTER_CLICK);
}

cv::Rect Desktop::GetDisplayRectangle() const {
  int screen = DefaultScreen(_display);
  int width = DisplayWidth(_display, screen);
  int height = DisplayHeight(_display, screen);
  return {0, 0, width, height};
}

cv::Mat Desktop::Capture(const cv::Rect& rectangle) const {
  XImage* image = XGetImage(
      _display, _window, rectangle.x, rectangle.y, rectangle.width,
      rectangle.height, AllPlanes, ZPixmap);
  cv::Mat bgra =
      cv::Mat(rectangle.height, rectangle.width, CV_8UC4, image->data);
  cv::Mat bgr;
  cv::cvtColor(bgra, bgr, cv::COLOR_BGRA2BGR);
  XDestroyImage(image);
  return bgr;
}

void Desktop::LeftClick(int x, int y) const {
  Move(x, y);
  Click(Button1);
}

void Desktop::RightClick(int x, int y) const {
  Move(x, y);
  Click(Button3);
}
