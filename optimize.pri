win32-g++: {
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
}

!win32-g++: {
DEFINES += _USE_MATH_DEFINES
}

