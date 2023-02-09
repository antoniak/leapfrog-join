// #ifndef TRIEITERATOR_H
// #define TRIEITERATOR_H

// #include "Utils.h"

// using namespace types;

// class TrieNode {
//     friend class TrieIterator;
//     private:
//         std::map<int,TrieNode*> children;
//         struct TrieNode* parent;
//         int count = 0;
//         int value_;
//         bool isLeaf_ = false;
//         bool atEnd_ = false;
//         bool isRoot_ = false;
//         std::map<int,TrieNode*>::iterator cit_;

//         TrieNode() { this->isRoot_ = true; };
//         TrieNode(TrieNode *node, int value, bool isLeaf);
//         ~TrieNode();
// };

// class TrieIterator {
//     public:
//         TrieNode* node;
//         explicit TrieIterator(Table<int> table, std::vector<int> attr_order);
//         ~TrieIterator();
//         void open();
//         void up();
//         int key();
//         void next();
//         bool atEnd();
//         void seek(int key);
//         long long count();
//         bool isLeaf();
// };

// #endif // TRIEITERATOR.H
