# CMakeList.txt : CMake project for Reversio, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Reversio")

# Add source to this project's executable.

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "B:/SFML/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system graphics audio network REQUIRED)
add_executable (Reversio "main.cpp"  "Hex.h" "Hex.cpp")
target_link_libraries(Reversio PRIVATE sfml-audio sfml-graphics sfml-network sfml-system)

# TODO: Add tests and install targets if needed.
