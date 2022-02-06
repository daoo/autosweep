#ifndef AUTOSWEEP_DESKTOP_H_
#define AUTOSWEEP_DESKTOP_H_

#include <opencv4/opencv2/core/mat.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Desktop {
 private:
  Display* _display;
  Window _window;

  void Move(int x, int y) const;
  void Click(unsigned int button) const;

 public:
  Desktop() {
    _display = XOpenDisplay(nullptr);
    _window = DefaultRootWindow(_display);
  }

  cv::Rect GetDisplayRectangle() const;

  cv::Mat Capture(const cv::Rect& rectangle) const;

  void LeftClick(int x, int y) const;
  void RightClick(int x, int y) const;

  ~Desktop() { XCloseDisplay(_display); }
};

#endif  // AUTOSWEEP_DESKTOP_H_
