set(REVERSIO_TESTING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SFML_STATIC_LIBRARIES TRUE)
set(Boost_USE_STATIC_LIBS ON)

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(TESTING_SOURCES "test_hex.cpp")

foreach (src ${TESTING_SOURCES})

	get_filename_component(src_name ${src} NAME_WE)

	add_executable(${src_name} ${src})

	target_compile_features(${src_name} PRIVATE cxx_std_20)

	target_link_options(${src_name} PRIVATE
			$<$<CONFIG:Release>:${RELEASE_LINK_OPTIONS}>
			$<$<CONFIG:Release>:${LTO_LINK_OPTIONS}>)

	target_include_directories(${src_name} 
			PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
			PRIVATE ${REVERSIO_SOURCE_DIR})
	
	target_link_libraries(${src_name} PUBLIC
			sfml-audio
			sfml-graphics
			sfml-network
			sfml-system
			Boost::unit_test_framework)

	add_test(NAME ${src_name}
			WORKING_DIRECTORY ${REVERSIO_TESTING_DIR}
			COMMAND ${REVERSIO_TEST_DIR}/${src_name})

endforeach(src)
