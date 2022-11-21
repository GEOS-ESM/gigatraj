# Note that directory and library name do not correspond!
esma_set_this ()

set (srcs
   ChangeVertical.cc  StreamDump.cc  StreamLoad.cc 
   StreamPrint.cc  StreamRead.cc
    )
esma_add_library (${this}
  SRCS ${srcs}
  )

include_directories (${GIGATRAJ_SOURCE_DIR}/include)

