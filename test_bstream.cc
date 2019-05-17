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
  char temp_char;
  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);

  temp_char = bis.GetChar();
  //std::cout << temp_char << std::endl;

  EXPECT_EQ(temp_char, 0x62); // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42); // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276); // 01011000010000000000001001110110
// 1110110 00000010 01000000 01011000 : 1979859032
// 1011000 01000000 00000010 01110110 : 1480589942

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
  bos.PutInt(440);
  // equals to 00000000 00000000 00000001 10111000 in binary form
  bos.PutBit(1);
  ofs.close();

  std::ifstream ifs(filename, std::ifstream::binary);
  char test_char;
  ifs.get(test_char);
  EXPECT_EQ(test_char, 'X');
  ifs.get(test_char);
  EXPECT_EQ(test_char, 'A');

  ifs.get(test_char);
  std::bitset<8> z(test_char);
  EXPECT_EQ(z, 0);
  // z is 00000000 which equals to 0 in integer;

  ifs.get(test_char);
  std::bitset<8> z1(test_char);
  EXPECT_EQ(z1, 0);
  // z1 is 00000000 which equals to 0 in integer;
  ifs.get(test_char);
  std::bitset<8> z2(test_char);
  EXPECT_EQ(z2, 1);
  // z2 is 00000001 which equals to 1 in integer;

  ifs.get(test_char);
  //std::cout << test_char << " test7" << std::endl;
  std::bitset<8> z3(test_char);
  //std::cout << z3 << " test7" << std::endl;
  EXPECT_EQ(z3, 0xB8);
  // z3 is 11011100 which equals to 184(10-base) 0xB8(16-base)
  // the previous bits are added to be
  // 00000000 0000000 00000001 11011100 == 440
  // 0x1B8 (16-base)

  std::remove(filename.c_str());
}


TEST(BStream, UsingBisToTestBos) {
  std::string filename{"test_bstream_output"};
  std::ofstream ofs(filename);
  BinaryOutputStream bos(ofs);
  bos.PutChar('X');
  bos.PutChar('A');
  bos.PutInt(-89818203);
  // equals to 11111010 10100101 01111011 10100101 in binary form

  ofs.close();

  std::ifstream ifs(filename, std::ios::in |
      std::ios::binary);
  BinaryInputStream bis(ifs);
  EXPECT_EQ(bis.GetChar(), 0x58);
  // the hex value for 'X' in ASCII table
  EXPECT_EQ(bis.GetChar(), 0x41);
  // the hex value for 'X' in ASCII table
  EXPECT_EQ(bis.GetInt(), -89818203);

  ifs.close();
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
