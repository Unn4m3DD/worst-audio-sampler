cmake_minimum_required (VERSION 2.8.12)
project (worst-audio-sampler)
add_executable (audio-to-image ./src/audio_to_image.cpp ./src/BitMap.cpp ./src/Wave.cpp)
add_executable (image-to-audio ./src/image_to_audio.cpp ./src/BitMap.cpp ./src/Wave.cpp)
target_include_directories (audio-to-image PUBLIC ./include)
target_include_directories (image-to-audio PUBLIC ./include)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../x86_64-bin )