#include "LFTJ.h"

using namespace types;

Level::Level(std::vector<TrieIterator*> iters){
    for (auto iter: iters) iters_.push_back(iter);
}

LFTJ::LFTJ(DataHandler *dH, bool out): dH_(dH), out_(out) {
    
    tuple_ =  new int(dH_->variableOrder.size()); 

    // If at least one relation is empty then return. 
    for (int table = 0; table < dH_->relations.size(); ++table) {
        if(dH_->database[table].size() == 0) {
            isEmpty_ = true;
            return;
        }
    }

    for (int table = 0; table < dH_->relations.size(); ++table) {
        // for each relation set the order of the variables in accordance with the variable order given by the user
        std::vector<int> order;
        order.reserve(dH_->relationsToVariables[table].size());
        for (auto attr: dH_->variableOrder) {
            auto it = find(dH_->relationsToVariables[table].begin(), dH_->relationsToVariables[table].end(), attr);
            if (it != dH_->relationsToVariables[table].end()){
                int index = it - dH_->relationsToVariables[table].begin();
                order.push_back(index);
            }
        }
        // construct the trie iterators for that relation
        trieIterators_[table] = new TrieIterator(dH_->database[table], order);
    }

    // construct a set of leapfrog joins; one for each variable in the query
    levels_.reserve(dH_->variables.size());
    for (auto attr: dH_->variableOrder){
        std::vector<TrieIterator*> iters;
        iters.reserve(dH_->variablesToRelations[attr].size());
        for(auto table: dH_->variablesToRelations[attr]) {
            iters.push_back(trieIterators_[table]);
        }
        levels_.push_back(new Level(iters));
    }
}

/* Count the tuples in the result. */
Table<int> LFTJ::Compute() {
    if (isEmpty_) {
        return resultTable_;
    }
    LFTJ::Open(); // initialize and search for the first match
    while(true) {
        if(levels_[level_]->atEnd) { // if the end of the level is reached
            if (level_ == 0) {
                // return 
                break; 
            } else {
                // move one level up and search for the next match 
                LFTJ::Up(); 
                LFTJ::Next(); 
            }
        } else { 
            if (level_ == dH_->variables.size() - 1) { // if this is the last level
                LFTJ::Next(); // find the next match
            } else { 
                LFTJ::Open(); // open the next level
            }
        }
    }
    std::cout << "The output consists of " << count_ << " tuples." << std::endl;
    return resultTable_;
}

/* Initialise the iterators and call search to find the first match. */
void LFTJ::Init() {
    if(AnyIterAtEnd()) { 
        levels_[level_]->atEnd = true;
    } else {
        levels_[level_]->atEnd = false;
        std::sort(  levels_[level_]->iters_.begin(),
            levels_[level_]->iters_.end(), [this](TrieIterator* it_a, TrieIterator* it_b) {
                if (it_a->key() < it_b->key()) {
                    return true;
                }
                return false;
        }); // sort by key 
        levels_[level_]->p = 0;
        LFTJ::Search();
    }
}

/* Place all iterators at the first key of the next level.*/
void LFTJ::Open() {
    level_++;
    for(auto iter: levels_[level_]->iters_) {
        iter->open();
    }
    LFTJ::Init(); 
}

/* Move the iterators one level up. */
void LFTJ::Up() {
    for(auto iter: levels_[level_]->iters_) {
        iter->up();
    }
    level_--;
}

/* Search for the first match */
void LFTJ::Search() {
    int k = levels_[level_]->iters_.size();
    int x_ = levels_[level_]->iters_[utils::mod(levels_[level_]->p-1, k)]->key();
    while(true) {
        int x = levels_[level_]->iters_[levels_[level_]->p]->key();
        if (x == x_) { // all iterators have the same key
            UpdateResult(x);
            return;
        } else {
            levels_[level_]->iters_[levels_[level_]->p]->seek(x_);
            if (levels_[level_]->iters_[levels_[level_]->p]->atEnd()) {
                levels_[level_]->atEnd = true;
                return;
            } else {
                x_ = levels_[level_]->iters_[levels_[level_]->p]->key();
                levels_[level_]->p = utils::mod(levels_[level_]->p + 1, k);
            }
        }
    }
}

/* Find the next result. */
void LFTJ::Next() {
    levels_[level_]->iters_[levels_[level_]->p]->next();
    if(levels_[level_]->iters_[levels_[level_]->p]->atEnd()) {
        levels_[level_]->atEnd = true;
    } else {
        int k = levels_[level_]->iters_.size();
        levels_[level_]->p = utils::mod(levels_[level_]->p + 1, k);
        LFTJ::Search();
    }
}

/* Update the counts and the result table. */
void LFTJ::UpdateResult(int value) {
    tuple_[level_] =  value;
    if(level_ == 0) {
        levels_[level_]->level_count = 1;
    }
    if(level_ > 0) {
        levels_[level_]->level_count = levels_[level_-1]->level_count;
    }
    for (auto iter: levels_[level_]->iters_) {
        if(iter->isLeaf()) {
            levels_[level_]->level_count *= iter->count();
        }
    }
    if(level_ == dH_->variables.size() - 1) {
        count_ += levels_[level_]->level_count;
        if(out_) {
            for (int i = 0; i < levels_[level_]->level_count; i++) {
                resultTable_.push_back(tuple_);
            }
        }
    }
}

/* Check if any of the iterators has reached its end. */
bool LFTJ::AnyIterAtEnd() {
    for(auto iter: levels_[level_]->iters_) {
        if (iter->atEnd()) {
            return true;
        }
    }
    return false;
}
