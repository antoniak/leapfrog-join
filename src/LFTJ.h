#ifndef LFTJ_H
#define LFTJ_H

#include <numeric>
#include <stdlib.h>
#include "DataHandler.h"
#include "TrieIterator.h"

using namespace types;

class Level {
public:
    bool atEnd; 
    int p;
    int key;
    long long level_count; 
    std::vector<TrieIterator*> iters_; 

public:
    /**
    * Level constructor.
    */
    Level(std::vector<TrieIterator*> iters);
};

class LFTJ {
public:

    /**
    * LFTJ constructor.
    */
    explicit LFTJ(DataHandler *dH, bool out = false);
    
    /**
    * LFTJ destructor.
    */
    ~LFTJ() = default;

    /**
    * Count the number of tuples in the result.
    */
    Table<int> Compute();

private:
    DataHandler *dH_;
    bool out_;
    Tuple<int> tuple_;
    Table<int> resultTable_;
    std::unordered_map<int, TrieIterator*> trieIterators_;
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
    void UpdateResult(int value);
    bool AnyIterAtEnd();
};


#endif //LFTJ_H
