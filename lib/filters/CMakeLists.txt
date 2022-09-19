# Note that directory and library name do not correspond!
set (this filters)

set (srcs
   ChangeVertical.cc  StreamDump.cc  StreamLoad.cc 
   StreamPrint.cc  StreamRead.cc
    )

include_directories (${this} ${CMAKE_SOURCE_DIR}/include)
add_library (${this} ${srcs})
target_link_libraries(${this} gigatraj)

