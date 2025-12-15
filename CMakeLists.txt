# CMakeList.txt : CMake project for piper_cpp_http_server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)
option(SIMULATE_PIPER "Stub out piper calls" OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include_directories(piper1-gpl/libpiper/include)
if ("${BOOST_ROOT}" STREQUAL "")
else()
	include_directories(${BOOST_ROOT}) 
endif()

#the submodule piper1-gpl must already have been build and in release mode
link_directories(piper1-gpl/libpiper/install piper1-gpl/libpiper/install/lib)

project ("piper_cpp_http_server")

if (SIMULATE_PIPER)
	add_compile_definitions(-DSIMULATE_PIPER)
endif()

if (WIN32)
	message( "WIN32 is set to " ${WIN32})
	add_compile_definitions(_WIN32_WINNT=0x0601)
endif()

# Add source to this project's executable.
add_executable (piper_cpp_http_server 
		piper_http_server.cpp 
		piper_synth.cpp
		piper_manager.cpp
		server.cpp
		request_parser.cpp
		request_handler.cpp
		reply.cpp
		connection.cpp
)

if (SIMULATE_PIPER)
else()
	add_executable (demo demo.cpp)
	target_link_libraries(piper_cpp_http_server piper onnxruntime)
	target_link_libraries(demo piper onnxruntime)
endif()

if (WIN32)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(piper_cpp_http_server ws2_32 wsock32)
endif()
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET piper_cpp_http_server PROPERTY CXX_STANDARD 20)
endif()

install (TARGETS piper_cpp_http_server)
if (WIN32)
install (FILES  piper1-gpl/libpiper/install/lib/onnxruntime.dll DESTINATION bin)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
install (FILES piper1-gpl/libpiper/install/piper.dll   DESTINATION bin)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
install (FILES piper1-gpl/libpiper/install/libpiper.dll  DESTINATION bin)
endif()
endif()
install (DIRECTORY piper1-gpl/libpiper/install/espeak-ng-data DESTINATION espeak-ng-data)
if (WIN32)
install (DIRECTORY windows_deployment/model DESTINATION .)
install (FILES windows_deployment/run_piper_server.bat DESTINATION .)
install (FILES windows_deployment/ReadMe.txt DESTINATION .)
install (FILES JsonSaveAs.png DESTINATION .)
endif()
