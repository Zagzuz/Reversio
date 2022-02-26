set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS
			 system
			 graphics
			 audio
			 network REQUIRED
)
add_executable(Reversio "main.cpp")
add_library(Hex "hex.cpp" "hex.h")
target_link_libraries(Reversio PRIVATE
					  Hex
					  sfml-audio
					  sfml-graphics
					  sfml-network
					  sfml-system
)