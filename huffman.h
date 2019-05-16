#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>


#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...
  std::map<char, int> CountFrequency(std::ifstream& ifs);
  void BuildTree(PQueue<HuffmanNode>& HuffTree);
  void OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos);
  void BuildCodeTable(HuffmanNode& n, std::map<char, std::string>
     code_table, std::string path);
  void BuildSequence(std::ifstream& ifs, std::map<char, std::string>
     code_table, BinaryOutputStream& bos);




};

// To be completed below
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs){

  std::map<char, int> freq = CountFrequency(ifs);
  PQueue<HuffmanNode> HuffmanTree;
  for (auto itr = freq.begin(); itr != freq.end(); itr++) {
    HuffmanTree.push(HuffmanNode cur_node(itr->first, itr->second));
  }
  BuildTree(PQueue<HuffmanNode>& HuffmanTree);
  BinaryOutputStream bos(ofs);
  OutputHuffman(Huffmantree.Top(), bos);
  std::map<char, std::string>code_table;
  BuildCodeTable(HuffmanTree.Top(), code_table, "");
  bos.PutInt(HuffmanTree.Top().freq());
  BuildSequence(ifs, code_table, bos);
}

std::map<char, int> Huffman::CountFrequency(std::ifstream &ifs){
  char temp_char;
  std::std::map<char, int> freq;
  BinaryInputStream bis(ifs);
  while (true) {
    if (ifs.eof()) {
      break;
    }
    temp_char = bis.GetChar();
    if (freq.find(temp_char) == freq.end()) {
      freq.insert(temp_char, 1);
    } else {
      freq.find(temp_char)->second++; // might need to change
    }
  }
  return freq;
}
void Huffman::BuildTree(PQueue<HuffmanNode>& HuffTree){
  while (HuffTree.Size() != 1) {
    HuffmanNode* node1 = new HuffmanNode(HuffTree.Top().data(), HuffTree.Top().freq());
    HuffTree.Pop();
    HuffmanNode* node2 = new HuffmanNode(HuffTree.Top().data(), HuffTree.Top().freq());
    HuffTree.Pop();
    HuffmanNode newNode(0, node1->freq_() + node2->freq_(), node1, node2);
    HuffTree.Push(newNode);
  }
}

void Huffman::OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos){
  if (n.IsLeaf()) {
    bos.PutBit(1);
    bos.PutChar(n.data());
  } else {
    bos.PutBit(0);
  }
  OutputHuffman(*(n.left()));
  OutputHuffman(*(n.right()));
}
void HuffMan::BuildCodeTable(HuffmanNode& n, std:map<char, std::string> code_table, std::string path){
  if (n.IsLeaf()){
    map.insert(n.data(), path);
  } else {
    BuildCodeTable(*(n.left()), code_table, path.append("0"));
    BuildCodeTable(*(n.right()), code_table, path.append("1"));
  }
}

void Huffman::BuildSequence(std::ifstream& ifs, std::string> code_table, BinaryOutputStream& bos){
  char ch;
  while(true) {
    if (ifs.eof()) {
      break;
    }
    ifs.get(ch);
    for(bit : code_table[ch]) {
      bos.PutBit(strtoi(bit));
    }
  }
}


#endif  // HUFFMAN_H_
