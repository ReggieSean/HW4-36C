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
#include <streambuf>
#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it does n't have any children
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
  static std::map<char, int> CountFrequency(std::ifstream& ifs, std::string& all_file);
  static void BuildTree(PQueue<HuffmanNode>& HuffTree);
  static void OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos);
  static void BuildCodeTable(HuffmanNode& n, std::map<char, std::string>&
     code_table, std::string path);
  static void BuildSequence(std::string& all_char, std::map<char, std::string>&
     code_table, BinaryOutputStream& bos);
  static void CleanUp(HuffmanNode* n);
  static HuffmanNode* ReconstructTree(BinaryInputStream& bis);
  // static void RebuildCodeTable(HuffmanNode* n, std::map<char, std::string>&
     // code_table, BinaryInputStream, bos);
  static void WriteIn(std::ofstream& ofs, HuffmanNode* root, BinaryInputStream& bis);
};

// To be completed below
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs){
  std::string all_char = std::string((std::istreambuf_iterator<char>(ifs)),
  std::istreambuf_iterator<char>());
  std::map<char, int> freq = CountFrequency(ifs, all_char);
  // std::cout << "im here" <<std::endl;
  PQueue<HuffmanNode> HuffmanTree;
  for (auto itr = freq.begin(); itr != freq.end(); itr++) {
    HuffmanNode cur_node(itr->first, itr->second);
    // std::cout << itr->first << ' ' << itr->second << std::endl;
    // std::cout << cur_node.data() << ' ' << cur_node.freq() << std::endl;
    HuffmanTree.Push(cur_node);
  }
  // std::cout << "im here" <<std::endl;
  BuildTree(HuffmanTree);
  // std::cout << "im here" <<std::endl;
  BinaryOutputStream bos(ofs);
  // std::cout << "im here" <<std::endl;
  OutputHuffman(HuffmanTree.Top(), bos);
  // std::cout << "im here" <<std::endl;
  std::map<char, std::string>code_table;
  BuildCodeTable(HuffmanTree.Top(), code_table, "");
  bos.PutInt(HuffmanTree.Top().freq());
  BuildSequence(all_char, code_table, bos);
  CleanUp(HuffmanTree.Top().left());
  CleanUp(HuffmanTree.Top().right());
}

std::map<char, int> Huffman::CountFrequency(std::ifstream &ifs, std::string& all_file){
  char temp_char;
  std::map<char, int> freq;
  // BinaryInputStream bis(ifs);
  // std::string all_file = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  // while (true) {
  //   if (ifs.eof()) {
  //     break;
  //   }
  //   // std::cout << "im here" <<std::endl;
    // temp_char = bis.GetChar();
    // if (freq.find(temp_char) == freq.end()) {
    //   freq.insert(std::pavoid CleanUp(HuffmanNode& root);ir<char, int>(temp_char, 1));
    // } else {
    //   freq.find(temp_char)->second++; // might need to change
    // }

  // }
  for (auto cur_char : all_file) {
    temp_char = cur_char;
    if (freq.find(temp_char) == freq.end()) {
      freq.insert(std::pair<char, int>(temp_char, 1));
    } else {
      freq.find(temp_char)->second++; // might need to change
    }
  }
  return freq;
}
void Huffman::BuildTree(PQueue<HuffmanNode>& HuffTree){
  while (HuffTree.Size() != 1) {
    // std::cout << HuffTree.Size() <<std::endl;
    HuffmanNode* node1 = new HuffmanNode(HuffTree.Top().data(), HuffTree.Top().freq(), HuffTree.Top().left(), HuffTree.Top().right());
    // std::cout << node1->data() <<std::endl;
    // std::cout << node1->freq() <<std::endl;
    HuffTree.Pop();
    HuffmanNode* node2 = new HuffmanNode(HuffTree.Top().data(), HuffTree.Top().freq(), HuffTree.Top().left(), HuffTree.Top().right());
    // std::cout << node2->data() <<std::endl;
    // std::cout << node2->freq() <<std::endl;
    HuffTree.Pop();
    HuffmanNode newNode(0, node1->freq() + node2->freq(), node1, node2);
    // std::cout << "new " << newNode.data() <<std::endl;
    // std::cout << "new " << newNode.freq() <<std::endl;
    HuffTree.Push(newNode);
  }
}

void Huffman::OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos){
  // std::cout << "new node" <<std::endl;
  // std::cout << n.data() << ' ' << n.freq() <<std::endl;
  if (n.IsLeaf()) {
    // std::cout << "is leaf" <<std::endl;
     bos.PutBit(1);
    // std::cout << "put 1" <<std::endl;

    bos.PutChar(n.data());
    // std::bitset<8> z(n.data());
    // std::cout << "put char " << z <<std::endl;
    // std::cout << "im here1" <<std::endl;
    // bos.PutChar(n.data());
    // std::cout << "im here1" <<std::endl;
  } else {
    // std::cout << "is not leaf" <<std::endl;
     bos.PutBit(0);
    // std::cout << "put 0" <<std::endl;
  }
  if (n.left()) {
    // std::cout << "checking left" <<std::endl;
    OutputHuffman(*(n.left()), bos);
  }
  if (n.right()) {
    // std::cout << "checking right" <<std::endl;
    OutputHuffman(*(n.right()), bos);
  }


}
void Huffman::BuildCodeTable(HuffmanNode& n, std::map<char, std::string>&
   code_table, std::string path){
  if (n.IsLeaf()){
    code_table.insert(std::pair<char, std::string> (n.data(), path));
    // std::cout << code_table.find(n.data())->first << ':' << code_table.find(n.data())->second <<std::endl;
  } else {
    if (n.left()) {
      BuildCodeTable(*(n.left()), code_table, path.append("0"));
      path.pop_back();
      // std::cout << path << std::endl;
    }
    if (n.right()) {
      BuildCodeTable(*(n.right()), code_table, path.append("1"));
      path.pop_back();
      // std::cout << path << std::endl;
    }
  }
}

void Huffman::BuildSequence(std::string& all_char, std::map<char, std::string>& code_table, BinaryOutputStream& bos){
  // char ch;
  // while(true) {
  //   if (ifs.eof()) {
  //     break;
  //   }
  //   ifs.get(ch);
  //   std::cout << ch <<std::endl;
   for (auto ch : all_char) {
     // std::cout << code_table.find(ch)->first << " hello" << std::endl;
     for (auto bit : code_table.find(ch)->second) {
        // std::cout << bit << std::endl;
        std::string str(1, bit);
        bos.PutBit(std::stoi(str));
     }
      // std::cout << code_table[ch];
      // std::string str(1, bit);
      // std::cout << str <<std::endl;
      // bos.PutBit(std::stoi(str));
  // }

   }
}

void Huffman::CleanUp(HuffmanNode* n){
  if (!n) {
    return;
  }
  CleanUp(n->left());
  CleanUp(n->right());
  free(n);
}

void Huffman::Decompress(std::ifstream& ifs, std::ofstream& ofs) {
  BinaryInputStream bis(ifs);
  HuffmanNode* root = ReconstructTree(bis);
  WriteIn(ofs, root, bis);
  CleanUp(root);
}

HuffmanNode* Huffman::ReconstructTree(BinaryInputStream& bis){
  HuffmanNode* cur_node;
  if (!bis.GetBit()) {
    cur_node = new HuffmanNode(0, 0, ReconstructTree(bis), ReconstructTree(bis));
    return cur_node;
  } else {
    cur_node = new HuffmanNode(bis.GetChar(), 0, nullptr, nullptr);
    return cur_node;
  }
}

void Huffman::WriteIn(std::ofstream& ofs, HuffmanNode* root, BinaryInputStream& bis) {
  BinaryOutputStream bos(ofs);
  int size = bis.GetInt();
  std::bitset<32> z(size);
  std::cout << z << " size" << std::endl;
  std::cout << size << std::endl;
  HuffmanNode* cur_node;
  for (int i = 0; i < size; i++) {
    cur_node = root;
    while (!cur_node->IsLeaf()){
      if (bis.GetBit()){
        cur_node = cur_node->right();
      } else {
        cur_node = cur_node->left();
      }
    }
    char ch = cur_node->data();
    std::cout << ch << " size" << std::endl;
    bos.PutChar(ch);
  }
}







#endif  // HUFFMAN_H_
