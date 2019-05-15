#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <cstddef>
#include <fstream>
#include <iostream>
#include <bitset>

class BinaryInputStream {
 public:
  explicit BinaryInputStream(std::ifstream &ifs);

  bool GetBit();
  char GetChar();
  int GetInt();

 private:
  std::ifstream &ifs;
  char buffer = 0;
  size_t avail = 0;

  // Helpers
  void RefillBuffer();
};

BinaryInputStream::BinaryInputStream(std::ifstream &ifs) : ifs(ifs) { }

void BinaryInputStream::RefillBuffer() {
  // Read the next byte from the input stream
  ifs.get(buffer);
  if (ifs.gcount() != 1)
    throw std::underflow_error("No more characters to read");
  avail = 8;
}

bool BinaryInputStream::GetBit() {
  bool bit;

  if (!avail)
    RefillBuffer();

  avail--;
  bit = ((buffer >> avail) & 1) == 1;

#if 0  // Switch to 1 for debug purposes
  if (bit)
    std::cout << '1' << std::endl;
  else
    std::cout << '0' << std::endl;
#endif

  return bit;
}

char BinaryInputStream::GetChar() {
  char byte = 0;
  for (int i = 0; i < 8; i++) {
    //byte = byte << 1;
    byte |= GetBit() << (7-i);
  }
  return byte;
}

int BinaryInputStream::GetInt() {
  // To be completed
  int four_bytes = 0;


  for (int i = 0; i < 4; i++) {
    four_bytes |= GetChar() << ((3-i) * 8);
  }
  return four_bytes;
}

class BinaryOutputStream {
 public:
  explicit BinaryOutputStream(std::ofstream &ofs);
  ~BinaryOutputStream();

  void Close();

  void PutBit(bool bit);
  void PutChar(char byte);
  void PutInt(int word);

 private:
  std::ofstream &ofs;
  char buffer = 0;
  size_t count = 0;

  // Helpers
  void FlushBuffer();
};

BinaryOutputStream::BinaryOutputStream(std::ofstream &ofs) : ofs(ofs) { }

BinaryOutputStream::~BinaryOutputStream() {
	Close();
}

void BinaryOutputStream::Close() {
  FlushBuffer();
}

void BinaryOutputStream::FlushBuffer() {
  // Nothing to flush
  if (!count)
    return;

  // If buffer isn't complete, pad with 0s before writing
  if (count > 0)
    buffer <<= (8 - count);

  // Write to output stream
  ofs.put(buffer);

  // Reset buffer
  buffer = 0;
  count = 0;
}

void BinaryOutputStream::PutBit(bool bit) { //not sure how to test a bit
  // Make some space and add bit to buffer
  buffer <<= 1;
  if (bit)
    buffer |= 1;

  // If buffer is full, write it
  if (++count == 8)
    FlushBuffer();
}

void BinaryOutputStream::PutChar(char byte) {
  // putchar is working fine
  buffer = 0;
  bool bit;
  for (int i = 0; i < 8; i++){
    bit = (byte >> i) & 1U;
    if (bit) {
      buffer |= 1 << i;
    } else {
      buffer |= 0 << i;
    }
    count++;
  }
  FlushBuffer();
}

void BinaryOutputStream::PutInt(int word) {
  /*two potential problems here:
  even the function is outputing binary data into the file
  i can not test if they are in the right order,
  somehow google test always shows the value is 0, I think another
  way of extracting int from file is needed*/
  char byte;
  for (size_t i = 0; i < sizeof(int); i++){
    byte = (word >> (((sizeof(int)-1)-i) * 8));
    // std::bitset<8> x(byte);
    // std::cout << byte << " test" << std::endl;
    // std::cout << x << " test2" << std::endl;
    PutChar(byte);
  }
  // To be completed
  /*bool bit;
  for (int i = 0; i < 32; i++) {
    bit = word >> (31 - i) & 1;
    PutBit(bit);
  }*/
}

#endif  // BSTREAM_H_
