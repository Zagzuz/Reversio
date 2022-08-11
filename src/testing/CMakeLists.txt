find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(test_unit 
		"test_hex.cpp" 
		"test_main.cpp" 
		"test_nof.cpp")

target_compile_features(test_unit PRIVATE cxx_std_20)

target_link_options(test_unit PRIVATE
		$<$<CONFIG:Release>:${RELEASE_LINK_OPTIONS}>
		$<$<CONFIG:Release>:${LTO_LINK_OPTIONS}>)

target_include_directories(test_unit 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
		PRIVATE ${REVERSIO_SOURCE_DIR})

target_link_libraries(test_unit PUBLIC
		sfml-audio
		sfml-graphics
		sfml-network
		sfml-system
		Boost::unit_test_framework)
