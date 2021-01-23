
#include <math.h>
#include <stdio.h>

#include <iostream>

#include "BitMap.h"
#include "Wave.h"
#include "alloca.h"
#include "string.h"
using namespace std;

void plot_frequencies(uint8_t* frequencies, uint32_t size, BitMap* image) {
  for (uint32_t i = 0; i < size - 2; i++) {
    image->line(i, frequencies[i], i + 1, (frequencies[i + 1] + frequencies[i + 2]) / 2, 0);
  }
}
void wav_frequencies(uint8_t* frequencies, uint32_t size, Wave* audio) {
  for (uint32_t i = 0; i < size; i++) {
    audio->point(i, frequencies[i]);
  }
}

int main(int argc, char** argv) {
  if (argc < 4) {
    cout << "Usage image-to-audio src_image.bmp out_file.wav audio_length" << endl
         << "audio_length must be an integer in seconds" << endl;
    return -1;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];
  uint32_t time = atoi(argv[3]);
  BitMap* image = BitMap::read_from_file(input_file);
  Wave wave(image->width / time, log(image->height) / log(2), time);
  wav_frequencies(image->frequencies(), image->width, &wave);
  wave.write_to_file((char*)output_file);
}