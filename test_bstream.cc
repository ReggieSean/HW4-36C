#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "bstream.h"

TEST(BStream, input) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62); // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42); // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276); // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}

TEST(BStream, output) {
  /*Since PutChar takes an char as parameter and
  Put Int takes an int as parameter, we can simply
  call them with dedicated characters and integers
  then open the file that was written and check
  the values that are outputed by the class*/
  std::string filename{"test_bstream_output"};
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);
  bos.PutChar('X');
  bos.PutChar('A');
  bos.PutInt(440); // equals to 00000000000000000000000110111000 in binary form

  ofs.close();

  std::ifstream ifs(filename, std::ifstream::binary);
  char test_char;
  //int test_int;
  // std::bitset<32> y(test_int);
  // std::cout << y << " test3" << std::endl;
  ifs.get(test_char);
  EXPECT_EQ(test_char, 'X');
  ifs.get(test_char);
  EXPECT_EQ(test_char, 'A');

  //ifs.read(reinterpret_cast<char*>(&test_int), sizeof(int));
//  EXPECT_EQ(test_int, 440);
  //std::bitset<32> z(test_int);
  //std::cout << z << " test4" << std::endl;
  // EXPECT_EQ(test_int, 440);

  ifs.get(test_char);
  std::bitset<8> z(test_char);
  //std::cout << z0 << " test4" << std::endl;
  EXPECT_EQ(z, 0);
  ifs.get(test_char);
  std::bitset<8> z1(test_char);
  // std::cout << z2 << " test5" << std::endl;
  EXPECT_EQ(z1, 0);
  ifs.get(test_char);
  std::bitset<8> z2(test_char);
  EXPECT_EQ(z2, 1);
  // std::cout << z << " test6" << std::endl;
  ifs.get(test_char);
  std::cout << test_char << " test7" << std::endl;
  std::bitset<8> z3(test_char);
  std::cout << z3 << " test7" << std::endl;
   EXPECT_EQ(z3, 0xB8);

  std::remove(filename.c_str());
}


TEST(BStream, outandin) {
  std::string filename{"test_bstream_output"};
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);
  bos.PutChar('X');
  bos.PutChar('A');
  bos.PutInt(440); // equals to 00000000000000000000000110111000 in binary form

  ofs.close();

  std::ifstream ifs(filename, std::ios::in |
      std::ios::binary);
  BinaryInputStream bis(ifs);

  EXPECT_EQ(bis.GetChar(),0x58 );
  EXPECT_EQ(bis.GetChar(), 0x41);
  EXPECT_EQ(bis.GetInt(), 0x000001B8);
//  EXPECT_EQ(bis.GetChar(), 0x1B8);
//  EXPECT_EQ(bis.GetChar(), 0x1B8);
//  EXPECT_EQ(bis.GetChar(), 0x1B8);
  //char test_char;
  //int test_int;
  // std::bitset<32> y(test_int);
  // std::cout << y << " test3" << std::endl;
  //ifs.get(test_char);
  //EXPECT_EQ(test_char, 'X');
  //ifs.get(test_char);
  //EXPECT_EQ(test_char, 'A');
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
