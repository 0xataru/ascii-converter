
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./tools/stb_image.h"

const char ASCII_CHARS[] =
    "@$B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`\'. ";

#define OUTPUT_WIDTH 50
#define OUTPUT_HEIGHT 50

void resizeImage(const unsigned char *input, int inW, int inH, int channels,
                 unsigned char *output, int outW, int outH) {
  float scaleX = (float)inW / outW;
  float scaleY = (float)inH / outH;

  for (int y = 0; y < outH; y++) {
    for (int x = 0; x < outW; x++) {
      int srcX = (int)(x * scaleX);
      int srcY = (int)(y * scaleY);
      int idx = (srcY * inW + srcX) * channels;

      int outIdx = (y * outW + x) * 3;
      output[outIdx] = input[idx];
      output[outIdx + 1] = (channels > 1) ? input[idx + 1] : input[idx];
      output[outIdx + 2] = (channels > 1) ? input[idx + 2] : input[idx];
    }
  }
}

void convertToASCII(const unsigned char *image, int width, int height,
                    int channels) {
  int asciiLen = sizeof(ASCII_CHARS) - 1;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int idx = (y * width + x) * channels;
      unsigned char r = image[idx];
      unsigned char g = (channels > 1) ? image[idx + 1] : image[idx];
      unsigned char b = (channels > 1) ? image[idx + 2] : image[idx];

      unsigned char brightness = (r + g + b) / 3;
      int asciiIndex = (brightness * (asciiLen - 1)) / 255;
      putchar(ASCII_CHARS[asciiIndex]);
    }
    putchar('\n');
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <input_image>\n", argv[0]);
    return 1;
  }

  const char *input_file = argv[1];

  int width, height, channels;
  unsigned char *image = stbi_load(input_file, &width, &height, &channels, 0);
  if (!image) {
    printf("Could not load image\n");
    return 1;
  }

  int newW = OUTPUT_WIDTH;
  int newH = OUTPUT_HEIGHT;

  unsigned char *resizedImage = (unsigned char *)malloc(newW * newH * 3);
  if (!resizedImage) {
    printf("Memory allocation failed\n");
    stbi_image_free(image);
    return 1;
  }

  resizeImage(image, width, height, channels, resizedImage, newW, newH);
  convertToASCII(resizedImage, newW, newH, 3);

  free(resizedImage);
  stbi_image_free(image);
  return 0;
}
