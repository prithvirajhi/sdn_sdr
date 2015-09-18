INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_STREAM_TO_MSG stream_to_msg)

FIND_PATH(
    STREAM_TO_MSG_INCLUDE_DIRS
    NAMES stream_to_msg/api.h
    HINTS $ENV{STREAM_TO_MSG_DIR}/include
        ${PC_STREAM_TO_MSG_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    STREAM_TO_MSG_LIBRARIES
    NAMES gnuradio-stream_to_msg
    HINTS $ENV{STREAM_TO_MSG_DIR}/lib
        ${PC_STREAM_TO_MSG_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(STREAM_TO_MSG DEFAULT_MSG STREAM_TO_MSG_LIBRARIES STREAM_TO_MSG_INCLUDE_DIRS)
MARK_AS_ADVANCED(STREAM_TO_MSG_LIBRARIES STREAM_TO_MSG_INCLUDE_DIRS)

