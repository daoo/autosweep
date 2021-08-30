project(
  'autosweep',
  'cpp',
  default_options: ['cpp_std=c++20'])

opencv = dependency('opencv4')
x11 = dependency('X11')

executable(
    'main',
    'main.cc',
    'board.cc',
    'desktop.cc',
    'detector.cc',
    'stream.cc',
    dependencies: [opencv, x11])

tests = executable(
    'tests',
    'tests.cc',
    'board.cc',
    dependencies: [opencv])

test('automatic tests', tests)
