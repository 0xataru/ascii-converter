#define STB_IMAGE_IMPLEMENTATION
#include "./tools/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./tools/stb_image_write.h"

#include <stdio.h>

const char ASCII_CHARS[] = "@%#*+=-:. ";

void convertToASCII(const unsigned char *image, int width, int height,
                    int channels, const char *output_file) {
  FILE *file = fopen(output_file, "w");
  if (!file) {
    printf("Could not open file for writing");
    return;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char r = image[(y * width + x) * channels + 0];
      unsigned char g = image[(y * width + x) * channels + 1];
      unsigned char b = image[(y * width + x) * channels + 2];
      unsigned char brightness = (r + g + b) / 3;

      int index = (brightness * (sizeof(ASCII_CHARS) - 1)) / 255;
      fputc(ASCII_CHARS[index], file);
    }
    fputs("\n", file);
  }

  fclose(file);
  printf("Conversion successful\n");
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <input_file> <output_file>\n", argv[0]);
    return 1;
  }

  const char *input_file = argv[1];
  const char *output_file = argv[2];

  int width, height, channels;
  unsigned char *image = stbi_load(input_file, &width, &height, &channels, 0);
  if (!image) {
    printf("Could not load image\n");
    return 1;
  }

  convertToASCII(image, width, height, channels, output_file);

  stbi_image_free(image);
  return 0;
}
