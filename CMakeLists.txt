cmake_minimum_required (VERSION 2.8.11)
project (worst-audio-sampler)
add_executable (audio-to-image ./src/audio_to_image.cpp)
target_include_directories (audio-to-image PUBLIC ./include)
