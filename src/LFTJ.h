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
    * LFTJ contructor.
    */
    explicit LFTJ(DataHandler *dH);
    
    /**
    * LFTJ destructor.
    */
    ~LFTJ() = default;

    /**
    * Count the number of tuples in the result.
    */
    long long Count();

private:
    DataHandler *dH_;
    // Tuple<int> tuple_;
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
