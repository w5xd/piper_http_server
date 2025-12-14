# CMakeList.txt : CMake project for piper_cpp_http_server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)
option(SIMULATE_PIPER "Stub out piper calls" ON)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set (wbuild True)

include_directories(piper1-gpl/libpiper/include)
if (wbuild)
include_directories(c:/dev/boost/boost_1_82_0/)
else()
endif()

link_directories(piper1-gpl/libpiper/install piper1-gpl/libpiper/install/lib)

project ("piper_cpp_http_server")

if (SIMULATE_PIPER)
	add_compile_definitions(-DSIMULATE_PIPER)
endif()

if (WIN32)
	add_compile_definitions(-D_WIN32_WINNT=0x0601)
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

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET piper_cpp_http_server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
install (TARGETS piper_cpp_http_server demo)
if (WIN32)
install (FILES piper1-gpl/libpiper/install/piper.dll  piper1-gpl/libpiper/install/lib/onnxruntime.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
install (DIRECTORY piper1-gpl/libpiper/install/espeak-ng-data DESTINATION espeak-ng-data)