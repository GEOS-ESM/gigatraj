add_executable (
  gtmodel_s01.x
  gtmodel_s01.cc
  )
target_link_libraries(gtmodel_s01.x gigatraj filters metsources)
add_executable (
  gtmodel_s02.x
  gtmodel_s02.cc
  )
target_link_libraries(gtmodel_s02.x gigatraj filters metsources)

install (
   PROGRAMS ${programs}
   DESTINATION bin
   )

