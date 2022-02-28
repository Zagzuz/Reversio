set(SFML_STATIC_LIBRARIES TRUE)
set(Boost_USE_STATIC_LIBS ON)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS
			 system
			 graphics
			 audio
			 network REQUIRED
)
find_package(Boost 1.78 COMPONENTS log REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/Boost/lib)
include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

add_executable(Reversio "main.cpp")
add_library(Hex "hex.cpp" "hex.h")
target_link_libraries(Reversio PRIVATE
					  Hex
					  sfml-audio
					  sfml-graphics
					  sfml-network
					  sfml-system
)
target_link_libraries(Reversio ${Boost_log})