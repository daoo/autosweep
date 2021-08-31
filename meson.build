project(
  'autosweep',
  'cpp',
  default_options: ['cpp_std=c++20'])

opencv = dependency('opencv4')
x11 = dependency('X11')

executable(
    'autosweep',
    'autosweep/main.cc',
    'autosweep/board.cc',
    'autosweep/desktop.cc',
    'autosweep/detector.cc',
    'autosweep/stream.cc',
    dependencies: [opencv, x11])

tests = executable(
    'tests',
    'autosweep/tests.cc',
    'autosweep/board.cc',
    dependencies: [opencv])

test('automatic tests', tests)
