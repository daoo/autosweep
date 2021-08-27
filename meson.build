project(
  'autosweep',
  'cpp',
  default_options: ['cpp_std=c++20'])

executable(
    'main',
    'main.cc',
    'board.cc',
    dependencies: [
        dependency('opencv4'),
        dependency('X11'),
    ]
)
