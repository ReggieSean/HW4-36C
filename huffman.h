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
#include <utility>
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
  static std::map<char, int> CountFrequency
  (std::ifstream& ifs, std::string& all_file);
  static void BuildTree(PQueue<HuffmanNode>& HuffTree);
  static void OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos);
  static void BuildCodeTable(HuffmanNode& n, std::map<char, std::string>&
     code_table, std::string path);
  static void OutputSequence(std::string& all_char, std::map<char, std::string>&
     code_table, BinaryOutputStream& bos);
  static void CleanUp(HuffmanNode* n);
  static HuffmanNode* ReconstructTree
  (BinaryInputStream& bis);
  static void WriteIn(std::ofstream& ofs,
    HuffmanNode* root, BinaryInputStream& bis);
};

// To be completed below
void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  // read the whole file and save it to be a string
  std::string all_char = std::string((std::istreambuf_iterator<char>(ifs)),
  std::istreambuf_iterator<char>());
  // count each char's frequency
  std::map<char, int> freq = CountFrequency(ifs, all_char);
  PQueue<HuffmanNode> HuffmanTree;
  // push each node into the pqueue
  for (auto itr = freq.begin(); itr != freq.end(); itr++) {
    HuffmanNode cur_node(itr->first, itr->second);
    HuffmanTree.Push(cur_node);
  }
  // build the tree
  BuildTree(HuffmanTree);
  // intialize the bos
  BinaryOutputStream bos(ofs);
  // writing the tree into the file
  OutputHuffman(HuffmanTree.Top(), bos);
  std::map<char, std::string>code_table;
  // build code table
  BuildCodeTable(HuffmanTree.Top(), code_table, "");
  // write the number of character into the file
  bos.PutInt(HuffmanTree.Top().freq());
  // write sequence of char into the file
  OutputSequence(all_char, code_table, bos);
  // delete nodes in the memory
  CleanUp(HuffmanTree.Top().left());
  CleanUp(HuffmanTree.Top().right());
}

std::map<char, int> Huffman::CountFrequency(std::ifstream &ifs,
   std::string& all_file) {
  char temp_char;
  std::map<char, int> freq;
  // loop through every single char
  for (auto cur_char : all_file) {
    temp_char = cur_char;
    // the char does not exist in the map yet
    if (freq.find(temp_char) == freq.end()) {
      freq.insert(std::pair<char, int>(temp_char, 1));
    // find the key and increse its frequency by one
    } else {
      freq.find(temp_char)->second++;
    }
  }
  return freq;
}
void Huffman::BuildTree(PQueue<HuffmanNode>& HuffTree) {
  // loop until the there is only the root in the pqueue
  while (HuffTree.Size() != 1) {
    HuffmanNode* node1 = new HuffmanNode(HuffTree.Top().data(),
     HuffTree.Top().freq(), HuffTree.Top().left(), HuffTree.Top().right());
    HuffTree.Pop();
    HuffmanNode* node2 = new HuffmanNode(HuffTree.Top().data(),
     HuffTree.Top().freq(), HuffTree.Top().left(), HuffTree.Top().right());
    HuffTree.Pop();
    HuffmanNode newNode(0, node1->freq() + node2->freq(), node1, node2);
    HuffTree.Push(newNode);
  }
}

void Huffman::OutputHuffman(HuffmanNode& n, BinaryOutputStream& bos) {
  // use a preordered trevesal to output every node and its path
  if (n.IsLeaf()) {
    bos.PutBit(1);
    bos.PutChar(n.data());
  } else {
    bos.PutBit(0);
  }
  if (n.left()) {
    OutputHuffman(*(n.left()), bos);
  }
  if (n.right()) {
    OutputHuffman(*(n.right()), bos);
  }
}
void Huffman::BuildCodeTable(HuffmanNode& n, std::map<char, std::string>&
  code_table, std::string path) {
  // recursively loop through all nodes and add information to the code table
  if (n.IsLeaf()) {
    code_table.insert(std::pair<char, std::string> (n.data(), path));
  } else {
    if (n.left()) {
      BuildCodeTable(*(n.left()), code_table, path.append("0"));
      path.pop_back();
    }
    if (n.right()) {
      BuildCodeTable(*(n.right()), code_table, path.append("1"));
      path.pop_back();
    }
  }
}

void Huffman::OutputSequence(std::string& all_char, std::map<char, std::string>&
  code_table, BinaryOutputStream& bos) {
  // loop through every single char
  for (auto ch : all_char) {
    // for each of them find the coresponding sequence
    // and then convert it to the form where putbit can read
    for (auto bit : code_table.find(ch)->second) {
      std::string str(1, bit);
      bos.PutBit(std::stoi(str));
    }
  }
}

void Huffman::CleanUp(HuffmanNode* n) {
  // use a preordered trevesal to free all nodes
  if (!n) {
    return;
  }
  CleanUp(n->left());
  CleanUp(n->right());
  free(n);
}

void Huffman::Decompress(std::ifstream& ifs, std::ofstream& ofs) {
  BinaryInputStream bis(ifs);
  // recursively rebuild the huffmantree
  HuffmanNode* root = ReconstructTree(bis);
  // write into the output file
  WriteIn(ofs, root, bis);
  // free created nodes
  CleanUp(root);
}

HuffmanNode* Huffman::ReconstructTree(BinaryInputStream& bis) {
  HuffmanNode* cur_node;
  // the path is 0 means it is a internal node
  if (!bis.GetBit()) {
    cur_node = new HuffmanNode(0, 0, ReconstructTree(bis),
    ReconstructTree(bis));
    return cur_node;
  // else it is a leaf node
  } else {
    cur_node = new HuffmanNode(bis.GetChar(), 0, nullptr, nullptr);
    return cur_node;
  }
}

void Huffman::WriteIn(std::ofstream& ofs, HuffmanNode* root,
  BinaryInputStream& bis) {
  BinaryOutputStream bos(ofs);
  // read the 32-bit integer from the string
  int size = bis.GetInt();
  HuffmanNode* cur_node;
  // loop exactly the number of char times
  // since it tells the exact number of nodes
  for (int i = 0; i < size; i++) {
    // start from root in each loop
    cur_node = root;
    // keep looping if the cur_node is not a leaf
    // if path is 1, go right, otherwise go left
    while (!cur_node->IsLeaf()) {
      if (bis.GetBit()) {
        cur_node = cur_node->right();
      } else {
        cur_node = cur_node->left();
      }
    }
    // write the leaf node's char into the file
    bos.PutChar(cur_node->data());
  }
}

#endif  // HUFFMAN_H_
