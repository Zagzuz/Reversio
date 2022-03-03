set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.78 COMPONENTS unit_test_framework REQUIRED)

set(SOURCE_FILES "test_main.cpp" "test_hex.cpp")

link_directories(${PROJECT_SOURCE_DIR}/Boost/lib
)
include_directories(${Boost_INCLUDE_DIRS}
					${PROJECT_SOURCE_DIR}/src
					${PROJECT_SOURCE_DIR}/SFML/include)
add_executable(ReversioTest ${SOURCE_FILES})
target_link_libraries(ReversioTest ${Boost_unit_test_framework}
)

add_test(HexTest hex_test)