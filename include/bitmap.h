#include <stdint.h>
typedef struct
{
  struct {
    uint16_t Signature;
    uint32_t FileSize;
    uint32_t reserved;
    uint32_t DataOffset;
  } header;
  struct
  {
    uint32_t Size;
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t Bits_Per_Pixel;
    uint32_t Compression;
    uint32_t ImageSize;
    uint32_t XpixelsPerM;
    uint32_t YpixelsPerM;
    uint32_t Colors_Used;
    uint32_t Important_Colors;
  };
  uint8_t pixel_data[];
} bitmap;
