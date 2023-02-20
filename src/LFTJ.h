#ifndef LFTJ_H
#define LFTJ_H

#include <numeric>
#include "DataHandler.h"
#include "TrieIterator.h"

using namespace types;

class Level {
public:
    bool atEnd;
    int p;
    int key;
    long long count;
    std::vector<TrieIterator*> iters_;

public:
    Level(std::vector<TrieIterator*> iters);
};

class LFTJ {
public:
    explicit LFTJ(DataHandler *dH, bool count = false);
    ~LFTJ() = default;
    long long Count();

private:
    DataHandler *dH_;
    bool countTuples_;
    // Table<int> resultTable_;
    std::unordered_map<int, TrieIterator*> trieIterators_;
    std::vector<int> attrOrder_;
    std::vector<Level*> levels_;
    bool isEmpty_ = false;
    int level_ = -1;
    bool totalEnd_ = false;
    long long count_ = 0;

    void Open();
    void Init();
    void Search();
    void Next();
    void Up();
    void UpdateResult();
    bool AnyIterAtEnd();
};


#endif //LFTJ_H
