#ifndef AUTOSWEEP_SCREEN_H_
#define AUTOSWEEP_SCREEN_H_

#include <opencv4/opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Desktop {
private:
  Display *_display;
  Window _window;
  int _x, _y, _width, _height;

public:
  Desktop(int x, int y, int width, int height)
      : _x(x), _y(y), _width(width), _height(height) {
    _display = XOpenDisplay(nullptr);
    _window = DefaultRootWindow(_display);
  }

  void Capture(cv::Mat &mat) {
    XImage *image = XGetImage(_display, _window, _x, _y, _width, _height,
                              AllPlanes, ZPixmap);
    // Could hold the XImage in a separate capture object with clean up
    // destructor to avoid the clone().
    mat = cv::Mat(_height, _width, CV_8UC4, image->data).clone();
    XDestroyImage(image);
  }

  void Move(int x, int y) {
    XEvent event;
    XQueryPointer(_display, _window, &event.xbutton.root, &event.xbutton.window,
                  &event.xbutton.x_root, &event.xbutton.y_root,
                  &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    int dx = -event.xbutton.x + x;
    int dy = -event.xbutton.y + y;
    XWarpPointer(_display, None, None, 0, 0, 0, 0, dx, dy);
    usleep(100);
  }

  void Click(int button) {
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xbutton.button = button;
    event.xbutton.same_screen = true;
    event.xbutton.subwindow = _window;
    while (event.xbutton.subwindow) {
      event.xbutton.window = event.xbutton.subwindow;
      XQueryPointer(_display, event.xbutton.window, &event.xbutton.root,
                    &event.xbutton.subwindow, &event.xbutton.x_root,
                    &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                    &event.xbutton.state);
    }
    // Press
    event.type = ButtonPress;
    if (XSendEvent(_display, PointerWindow, True, ButtonPressMask, &event) == 0)
      fprintf(stderr, "Error to send the event!\n");
    XFlush(_display);
    usleep(100);
    // Release
    event.type = ButtonRelease;
    if (XSendEvent(_display, PointerWindow, True, ButtonReleaseMask, &event) ==
        0)
      fprintf(stderr, "Error to send the event!\n");
    XFlush(_display);
    usleep(100);
  }

  void RightClick(int x, int y) {
    printf("RightClick(%d, %d)\n", x, y);
    Move(x, y);
    Click(Button3);
  }

  void LeftClick(int x, int y) {
    printf("LeftClick(%d, %d)\n", x, y);
    Move(x, y);
    Click(Button1);
  }

  ~Desktop() { XCloseDisplay(_display); }
};

#endif // AUTOSWEEP_SCREEN_H_
