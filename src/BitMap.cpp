#include "BitMap.h"

#include <fstream>
#include <iostream>

#include "malloc.h"
#include "string.h"
using namespace std;
BitMap::BitMap(bitmap_t* inner_bitmap) {
  this->bitmap = inner_bitmap;
  this->width = inner_bitmap->info_header.width;
  this->height = inner_bitmap->info_header.height;
}

BitMap::BitMap(uint32_t width, uint32_t height, uint16_t default_color = 0xff) {
  this->height = height;
  this->width = width;
  bitmap = (bitmap_t*)malloc(sizeof(bitmap_t));
  bitmap->header.signature = ('M' << 8) + 'B';
  bitmap->header.file_size = sizeof(bitmap_t) - sizeof(uint16_t*) + height * width * sizeof(uint16_t);
  bitmap->header.reserved = 0;
  bitmap->header.data_offset = sizeof(bitmap_t) - sizeof(uint16_t*);
  bitmap->info_header.size = 40;
  bitmap->info_header.width = width;
  bitmap->info_header.height = height;
  bitmap->info_header.planes = 1;
  bitmap->info_header.bits_per_pixel = 16;
  bitmap->info_header.compression = 0;
  bitmap->info_header.image_size = 0;
  bitmap->info_header.x_pixels_per_m = 100;
  bitmap->info_header.y_pixels_per_m = 100;
  bitmap->info_header.colors_used = 65536;
  bitmap->info_header.important_colors = 0;
  bitmap->pixel_data = (uint16_t*)malloc(sizeof(uint16_t) * width * height);
  memset(bitmap->pixel_data, default_color, sizeof(uint16_t) * width * height);
}

BitMap::~BitMap() {
  free(bitmap->pixel_data);
  free(bitmap);
  if (frequencies_buffer != nullptr) {
    free(frequencies_buffer);
  }
}

void BitMap::write_to_file(char* filename) {
  ofstream output_file;
  output_file.open(filename, ios::binary | ios::out);
  if (output_file.is_open()) {
    output_file.write((char*)bitmap, sizeof(bitmap_t) - sizeof(uint16_t*));
    output_file.write((char*)bitmap->pixel_data, height * width * sizeof(uint16_t));
    output_file.close();
  } else {
    cout << "ERROR" << endl;
  }
}

void BitMap::set_pixel(uint32_t x, uint32_t y, uint16_t color) {
  bitmap->pixel_data[x + width * y] = color;
}

uint16_t BitMap::get_pixel(uint32_t x, uint32_t y) {
  return bitmap->pixel_data[x + width * y];
}

void BitMap::line(int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, uint16_t color) {
  int32_t distance = (from_x - to_x) * (from_x - to_x) + (from_y - to_y) * (from_y - to_y);
  double increment_x = (double)(to_x - from_x) / (double)distance;
  double increment_y = (double)(to_y - from_y) / (double)distance;
  for (int i = 0; i < distance; i++) {
    set_pixel(from_x + increment_x * i, from_y + increment_y * i, color);
  }
}

uint8_t* BitMap::frequencies() {
  if (frequencies_buffer != nullptr) return frequencies_buffer;
  frequencies_buffer = (uint8_t*)malloc(sizeof(uint8_t) * bitmap->info_header.width);
  for (uint32_t x = 0; x < bitmap->info_header.width; x++) {
    for (uint32_t y = 0; y < bitmap->info_header.height; y++) {
      if (get_pixel(x, y) != (uint16_t)-1) {
        frequencies_buffer[x] = y;
      }
    }
  }
  return frequencies_buffer;
}

BitMap* BitMap::read_from_file(char* filename) {
  bitmap_t* bitmap = (bitmap_t*)malloc(sizeof(bitmap_t));
  ifstream input_file;
  input_file.open(filename, ios::binary | ios::out);
  if (input_file.is_open()) {
    input_file.read((char*)bitmap, sizeof(bitmap_t) - sizeof(uint16_t*));
    uint32_t image_size = bitmap->info_header.height * bitmap->info_header.width;
    bitmap->pixel_data = (uint16_t*)malloc(image_size * sizeof(uint16_t));
    input_file.read((char*)bitmap->pixel_data, image_size * sizeof(uint16_t));
    input_file.close();
  } else {
    cout << "ERROR" << endl;
  }
  return new BitMap(bitmap);
}