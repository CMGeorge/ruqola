macro(add_ruqolaroom_test _source)
  set( _sources ${_source} ${ARGN} )
  get_filename_component( _name ${_source} NAME_WE )
  add_executable( ${_name} ${_sources} )
  add_test(NAME ${_name} COMMAND ${_name} )
  ecm_mark_as_test(${_name})
  target_link_libraries( ${_name} Qt5::Test libruqolacore libruqolawidgets)
endmacro()

add_ruqolaroom_test(roomwidgettest.cpp)
add_ruqolaroom_test(roomheaderwidgettest.cpp)
add_ruqolaroom_test(messagetextedittest.cpp)
add_ruqolaroom_test(messagelinewidgettest.cpp)
add_ruqolaroom_test(messagelistdelegatetest.cpp testdata.cpp)
add_ruqolaroom_test(messagedelegatehelperimagetest.cpp testdata.cpp)
add_ruqolaroom_test(messagedelegatehelperfiletest.cpp)
add_ruqolaroom_test(readonlylineeditwidgettest.cpp)
add_ruqolaroom_test(usersinroomflowwidgettest.cpp)
add_ruqolaroom_test(usersinroomlabeltest.cpp)
add_ruqolaroom_test(channelactionpopupmenutest.cpp)
