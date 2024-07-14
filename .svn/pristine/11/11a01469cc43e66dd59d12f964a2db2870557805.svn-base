/*
 * countFreq.cpp
 *
 *  Created on: 2022. 7. 22.
 *      File made by: g
 *
Original code
https://www.geeksforgeeks.org/find-frequency-number-array/
by Sangita Dey
 */

// Returns the largest value that occurs in the start tail with the frequency
// Frequency N scatter reflectance -M
// https://www.gisagmaps.com/l8-atmospheric-correction
#ifdef ONE_U
#include "countFreq.h"

using namespace std;

// unordered_map<int, int> hmr, hmg, hmb;
uint16_t hmr[256] = {0}, hmg[256] = {0}, hmb[256] = {0};

void countFreq(uint32_t *a, uint32_t n) {
  // Insert elements and their
  // frequencies in hash map.
  for (uint32_t i = 0; i < n; i++) {
    // b = a[2 * i]; // & 0xF8;
    uint16_t *c1 = (uint16_t *)a + i;
    uint8_t b1 = (uint8_t)(*c1 >> 8);
    uint8_t b2 = (uint8_t)(*c1 & 0x00FF);
    uint8_t r = (b1) & 0xf8;
    uint8_t g = ((b1) << 5) | (((b2) & 0xe0) >> 3);
    uint8_t b = (b2) << 3;
    if (hmr[r] < 65535)
      hmr[r]++; // no need to go up higher
    if (hmg[g] < 65535)
      hmg[g]++; // no need to go up higher
    if (hmb[b] < 65535)
      hmb[b]++; // no need to go up higher
  }
}

// for RGB565 images
// check freqN on R/G/B
// startAddr: Memory address
// size: size of image (not total bytes)
// N: Frequency N
// dataOut: Output
int freqN(uint32_t *startAddr, uint32_t size, uint32_t N, uint8_t *dataOut) {

  uint32_t n = 0;

  // freqN on R
  countFreq(startAddr, size);

  // RGB565 interpolation
  for (uint8_t i = 0; i < 32; i++) {
    uint16_t div8r = uint16_t(float(hmr[8 * i]) / 8.0);
    uint16_t div8b = uint16_t(float(hmb[8 * i]) / 8.0);
    for (uint8_t j = 1; j < 8; j++) {
      hmr[8 * i + j] = div8r;
      hmb[8 * i + j] = div8b;
    }
    hmr[8 * i] = hmr[8 * i] - 7 * div8r;
    hmb[8 * i] = hmb[8 * i] - 7 * div8b;
  }
  for (uint8_t i = 0; i < 64; i++) {
    uint16_t div4g = uint16_t(float(hmg[4 * i]) / 4.0);
    for (uint8_t j = 1; j < 4; j++) {
      hmg[4 * i + j] = div4g;
    }
    hmg[4 * i] = hmg[4 * i] - 3 * div4g;
  }

  /*
  uint32_t _nn = 0;
  for (uint16_t i = 0; i < 256; i++) {
    printf("%u, %u\n", i, hmg[i]);
    // printf("%u\n", _nn); //should add up total pixels
    //_nn += hmr[i];
  }
  // printf("%u\n", _nn); //should add up total pixels
  */

  uint32_t _n;
  _n = 0;
  for (uint16_t i = 0; i < 256; i++) {
    _n += (uint32_t)hmr[i];
    // printf("%d, %u\n", i, _n);
    if (_n > N) {
      if (i != 0)
        // n = i - 1;
        n = i;
      else
        n = 0;
      break;
    }
  }
  // printf("%u\n", n);
  dataOut[0] = (uint8_t)n;

  _n = 0;
  // freqN on G
  for (uint16_t i = 0; i < 256; i++) {
    _n += (uint32_t)hmg[i];
    if (_n > N) {
      if (i != 0)
        // n = i - 1;
        n = i;
      else
        n = 0;
      break;
    }
  }
  dataOut[1] = (uint8_t)n;

  _n = 0;
  // freqN on B
  for (uint16_t i = 0; i < 256; i++) {
    // uint32_t _n = hmb[i];
    _n += (uint32_t)hmb[i];
    if (_n > N) {
      if (i != 0)
        // n = i - 1;
        n = i;
      else
        n = 0;
      break;
    }
  }
  dataOut[2] = (uint8_t)n;

  // printf("%d, %d, %d\n", dataOut[0], dataOut[1], dataOut[2]);

  return 0;
}
#endif/* for 1U*/
