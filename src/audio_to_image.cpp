#include <math.h>
#include <stdio.h>

#include <iostream>

#include "BitMap.h"
#include "Wave.h"
#include "alloca.h"
#include "string.h"
using namespace std;

void plot_frequencies(uint8_t* frequencies, uint32_t size, BitMap* image) {
  for (uint32_t i = 0; i < size - 1;) {
    uint32_t j = i + 1;
    while (120 < frequencies[j] && frequencies[j] < 134 && j < size) j++;
    image->line(i, frequencies[i], j, frequencies[j], 0);
    i = j;
  }
}
void wav_frequencies(uint32_t* frequencies, uint32_t size, Wave* audio) {
  for (uint32_t i = 0; i < size; i++) {
    audio->point(i, frequencies[i]);
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "Usage audio-to-image src_audio.wav out_filename.bmp" << endl
         << "src_audio.wav must be (Microsoft PCM, 8 bit, unsigned)" << endl;
    return -1;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];
  Wave* wave = Wave::read_from_file((char*)input_file);
  BitMap image(wave->bitrate * wave->time, pow(2, wave->bitcount), -1);
  plot_frequencies(wave->frequencies(), wave->bitrate * wave->time, &image);
  image.write_to_file((char*)output_file);
}