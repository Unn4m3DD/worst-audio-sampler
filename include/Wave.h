#include <stdint.h>

#include <fstream>
#include <iostream>

#include "malloc.h"
#include "string.h"
using namespace std;
#pragma pack(push, 1)
typedef struct
{
  uint32_t chunk_id;
  uint32_t chunk_size;
  uint32_t chunk_format;
  uint32_t sub_chunk_id;
  uint32_t sub_chunk_data_length;
  uint16_t type_of_format;
  uint16_t number_of_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t bits_per_sample_x_channels;
  uint16_t bits_per_sample;
  uint32_t chunk_header;
  uint32_t data_section_size;
  uint8_t* data;
} wave_t;
#pragma pack(pop)

class Wave {
 private:
  wave_t* wave;

 public:
  uint32_t time;
  uint32_t bitrate;
  uint32_t bitcount;
  Wave(wave_t* inner_wave);
  Wave(uint32_t bitrate, uint32_t bitcount, uint32_t time);
  ~Wave();
  void write_to_file(char* filename);
  void point(uint32_t time, uint8_t pitch);
  uint8_t* frequencies();
  static Wave* read_from_file(char* filename);

};