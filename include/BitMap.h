#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
  struct {
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
  } header;
  struct
  {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t important_colors;
  } info_header;
  uint16_t* pixel_data;
} bitmap_t;
#pragma pack(pop)
class BitMap {
 private:
  bitmap_t* bitmap;
  uint8_t* frequencies_buffer = nullptr;

 public:
  uint32_t height;
  uint32_t width;
  BitMap(bitmap_t* inner_bitmap);
  BitMap(uint32_t width, uint32_t height);
  BitMap(uint32_t width, uint32_t height, uint16_t default_color);
  ~BitMap();
  void write_to_file(char* filename);
  void set_pixel(uint32_t x, uint32_t y, uint16_t color);
  uint16_t get_pixel(uint32_t x, uint32_t y);
  void line(int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, uint16_t color);
  uint8_t* frequencies();
  static BitMap* read_from_file(char* filename);
};