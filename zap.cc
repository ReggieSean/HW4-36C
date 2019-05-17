#include <iostream>
#include <fstream>
#include <cstdio>
#include "huffman.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: ./zap <inputfile> <zapfile>" <<std::endl;
    exit(1);
  }
  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: cannot open input file " << argv[1] <<std::endl;
    exit(1);
  }
  std::ofstream ofs(argv[2], std::ios::out |
    std::ios::trunc | std::ios::binary);
  Huffman::Compress(ifs, ofs);
  std::cout << "Compressed input file " << argv[1]
  << " into zap file " << argv[2] <<std::endl;
  ifs.close();
  ofs.close();
  return 0;
}
