set(REVERSIO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SFML_STATIC_LIBRARIES TRUE)
set(Boost_USE_STATIC_LIBS ON)

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

add_subdirectory(base)
add_subdirectory(testing)

add_executable(reversio "main.cpp")

target_compile_features(reversio PUBLIC cxx_std_20)

target_compile_options(reversio PRIVATE 
		${BASIC_COMPILE_OPTIONS}
		$<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
		$<$<CONFIG:Release>:${LTO_COMPILE_OPTIONS}>)

target_link_options(reversio PRIVATE
		$<$<CONFIG:Release>:${RELEASE_LINK_OPTIONS}>
		$<$<CONFIG:Release>:${LTO_LINK_OPTIONS}>)

target_include_directories(reversio PRIVATE 
		${Boost_INCLUDE_DIRS} 
		${REVERSIO_SOURCE_DIR})

target_link_libraries(reversio
		sfml-audio
		sfml-graphics
		sfml-network
		sfml-system
		${Boost_log}
		base)
