cmake_minimum_required(VERSION 3.16)
project(Learn2Slither)

# set the c++ to 17
set(CMAKE_CXX_STANDARD 20)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(Learn2Slither main.cpp Board.cpp Snake.cpp utils.cpp visuals.cpp)

# Link to the actual SDL3 library.
target_link_libraries(Learn2Slither PRIVATE SDL3::SDL3)
