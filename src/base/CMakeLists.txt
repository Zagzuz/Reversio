add_library(base
		"hex.h"
		"layout.h"
		"nof.h"
		"orientation.h"
		"point.h")

target_compile_features(base PRIVATE cxx_std_20)

target_link_options(base PRIVATE
		$<$<CONFIG:Release>:${RELEASE_LINK_OPTIONS}>
		$<$<CONFIG:Release>:${LTO_LINK_OPTIONS}>)

target_include_directories(base 
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
		PRIVATE ${REVERSIO_SOURCE_DIR})

target_link_libraries(base PUBLIC
		sfml-audio
		sfml-graphics
		sfml-network
		sfml-system
		Boost::log)
