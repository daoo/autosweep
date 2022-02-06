project(
  'autosweep',
  'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

catch2 = dependency('catch2')
opencv = dependency('opencv4')
x11 = dependency('X11')

libautosweep = static_library(
    'autosweep',
    'autosweep/board.cc',
    'autosweep/detector.cc',
    'autosweep/stream.cc',
    dependencies: [opencv])

executable(
    'autosweep',
    'autosweep/main.cc',
    'autosweep/desktop.cc',
    dependencies: [opencv, x11],
    link_with: libautosweep)

tests = executable(
    'tests',
    'autosweep/tests.cc',
    'autosweep/board_unittests.cc',
    dependencies: [catch2, opencv],
    link_with: libautosweep)

test('autosweep unit tests', tests)
