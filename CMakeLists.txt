cmake_minimum_required (VERSION 3.8)
project ("Reversio")

set(REVERSIO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${REVERSIO_DIR}/exe)

set(BASIC_COMPILE_OPTIONS
		/MP  # Multiprocessor compilation
		/W4  # Warnings level
		/WX) # Treat warnings as errors

set(RELEASE_COMPILE_OPTIONS
		/O2		# Max optimization level
		/Ob3	# Aggressive inlining
		/Oi     # Enable intrinsics
		/Ot     # Prefer fast code
		/Oy)	# Omit frame pointers

set(LTO_COMPILE_OPTIONS
		/Gy		# Using COMDAT folding for functions
		/Gw		# Using COMDAT folding for variables
		/GL)	# Enable link time code generation

set(RELEASE_LINK_OPTIONS
		/OPT:REF)	# Eliminate unused code

set(LTO_LINK_OPTIONS
		/OPT:ICF			# Enable COMDAT folding
		/LTCG:INCREMENTAL)	# Force fast link timecode generation

add_subdirectory(src)
