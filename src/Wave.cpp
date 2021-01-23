#include "Wave.h"

#include <iostream>
using namespace std;
Wave::Wave(wave_t* inner_wave) {
  wave = inner_wave;
  this->time = ((double)inner_wave->data_section_size * 8) / (inner_wave->sample_rate * inner_wave->bits_per_sample);
  this->bitrate = inner_wave->sample_rate;
  this->bitcount = inner_wave->bits_per_sample;
}
Wave::Wave(uint32_t bitrate, uint32_t bitcount, double time) {
  this->time = time;
  this->bitrate = bitrate;
  this->bitcount = bitcount;
  wave = (wave_t*)malloc(sizeof(wave_t));
  wave->chunk_id = 'F' << 24 | 'F' << 16 | 'I' << 8 | 'R' << 0;
  wave->chunk_format = 'E' << 24 | 'V' << 16 | 'A' << 8 | 'W';
  wave->sub_chunk_id = ' ' << 24 | 't' << 16 | 'm' << 8 | 'f';
  wave->sub_chunk_data_length = 16;
  wave->type_of_format = 1;
  wave->number_of_channels = 1;
  wave->sample_rate = bitrate;
  wave->byte_rate = (bitrate * bitcount * wave->number_of_channels + 7) / 8;
  wave->bits_per_sample_x_channels = (bitrate * wave->number_of_channels + 7) / 8;
  wave->bits_per_sample = bitcount;
  wave->chunk_header = 'a' << 24 | 't' << 16 | 'a' << 8 | 'd';
  wave->data_section_size = (time * bitrate * wave->number_of_channels * bitcount + 7) / 8;
  wave->data = (uint8_t*)malloc(wave->data_section_size);
  wave->chunk_size = 36 + wave->data_section_size;
  memset(wave->data, 0, wave->data_section_size);
}

Wave::~Wave() {
  free(wave->data);
  free(wave);
}

void Wave::write_to_file(char* filename) {
  ofstream output_file;
  output_file.open(filename, ios::binary | ios::out);
  if (output_file.is_open()) {
    output_file.write((char*)wave, sizeof(wave_t) - sizeof(uint8_t*));
    output_file.write((char*)wave->data, wave->data_section_size);
    output_file.close();
  } else {
    cout << "ERROR" << endl;
  }
}

void Wave::point(uint32_t time, uint8_t pitch) {
  wave->data[time] = pitch;
}

uint8_t* Wave::frequencies() {
  return this->wave->data;
}

Wave* Wave::read_from_file(char* filename) {
  wave_t* inner_wave = (wave_t*)malloc(sizeof(wave_t));
  ifstream input_file;
  input_file.open(filename, ios::binary | ios::in);
  input_file.read((char*)inner_wave, sizeof(wave_t) - sizeof(uint8_t*));
  inner_wave->data = (uint8_t*)malloc(inner_wave->chunk_size - 36);
  input_file.read((char*)inner_wave->data, inner_wave->chunk_size - 36);
  Wave* result = new Wave(inner_wave);
  return result;
}
