#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include <stdio.h>
#include <stdlib.h>

const char *ASCII_CHARS = "@%#*+=-:. ";

void image_to_ascii(const char *input, const char *output, int scale) {
  int width, height, channels;
  unsigned char *img = stbi_load(input, &width, &height, &channels, 1);
  if (!img) {
    fprintf(stderr, "Error loading image\n");
    return;
  }

  FILE *file = fopen(output, "w");
  if (!file) {
    fprintf(stderr, "Error opening file for writing\n");
    stbi_image_free(img);
    return;
  }

  size_t len = strlen(ASCII_CHARS) - 1;
  float aspect_ratio = 2.0;
  int new_width = width / scale;
  int new_height = height / (scale * aspect_ratio);

  for (int y = 0; y < new_height; y++) {
    for (int x = 0; x < new_width; x++) {
      int sum = 0;
      for (int dy = 0; dy < scale * aspect_ratio; dy++) {
        for (int dx = 0; dx < scale; dx++) {
          int px = (y * scale * aspect_ratio + dy) * width + (x * scale + dx);
          if (px < width * height)
            sum += img[px];
        }
      }
      int avg_pixel = sum / (scale * scale * aspect_ratio);
      fputc(ASCII_CHARS[(avg_pixel * len) / 256], file);
    }
    fputc('\n', file);
  }

  fclose(file);
  stbi_image_free(img);
  printf("Conversion complete! Result saved to %s\n", output);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <input_image> <output_text> <scale>\n", argv[0]);
    return 1;
  }

  int scale = atoi(argv[3]);
  if (scale < 1)
    scale = 1;

  image_to_ascii(argv[1], argv[2], scale);
  return 0;
}
