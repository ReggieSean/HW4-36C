#include <iostream>
#include <fstream>
#include <cstdio>
#include "huffman.h"

int main(int argc, char* argv[]) {
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::ofstream ofs(argv[2], std::ios::out | std::ios::trunc | std::ios::binary);
  Huffman huff;
  huff.Compress(ifs, ofs);
  return 0;
}
