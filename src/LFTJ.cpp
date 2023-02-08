#include "LFTJ.h"

#include <stdlib.h>

using namespace types;

Level::Level(std::vector<TrieIterator*> iters){
    for (auto iter: iters) iters_.push_back(iter);
}

LFTJ::LFTJ(DataHandler<int> *dH, bool count) : dH_(dH), countTuples_(count) {
    attrOrder_.reserve(dH_->attrs);
    for (int i = 0; i < dH_->attrs; ++i) attrOrder_.push_back(i);

    for (int table = 0; table < dH_->tables; ++table) {
        if(dH_->database[table].size() == 0) {
            isEmpty_ = true;
            return;
        }
    }

    for (int table = 0; table < dH_->tables; ++table) {
        std::vector<int> order;
        order.reserve(dH_->IDs[table].size());
        for (auto attr: attrOrder_) {
            auto it = find(dH_->IDs[table].begin(), dH_->IDs[table].end(), attr);
            if (it != dH_->IDs[table].end()){
                int index = it - dH_->IDs[table].begin();
                order.push_back(index);
            }
        }
        trieIterators_[table] = new TrieIterator(dH_->database[table], order);
    }

    levels_.reserve(dH_->attrs);
    for (auto attr: attrOrder_){
        std::vector<TrieIterator*> iters;
        iters.reserve(dH_->ta[attr].size());
        for(auto table: dH_->ta[attr]) iters.push_back(trieIterators_[table.tableID]);
        levels_.push_back(new Level(iters));
    }
}

long long LFTJ::Count() {
    if (isEmpty_) return count_;
    LFTJ::Open();
    while(true) {
        if(levels_[level_]->atEnd) {
            if (level_ == 0) break;
            else {
                LFTJ::Up();
                LFTJ::Next();
            }
        } else {
            if (level_ == dH_->attrs - 1) LFTJ::Next();
            else LFTJ::Open();
        }
    }
    return count_;
}

void LFTJ::Init() {
    if(AnyIterAtEnd()) {
        levels_[level_]->atEnd = true;
    } else {
        levels_[level_]->atEnd = false;
        std::sort(  levels_[level_]->iters_.begin(),
            levels_[level_]->iters_.end(), [this](TrieIterator* it_a, TrieIterator* it_b) {
                if (it_a->key() < it_b->key()) return true;
                return false;
        });
        levels_[level_]->p = 0;
        LFTJ::Search();
    }
}

void LFTJ::Open() {
    level_++;
    for(auto iter: levels_[level_]->iters_) iter->open();
    LFTJ::Init();
}

void LFTJ::Up() {
    for(auto iter: levels_[level_]->iters_) iter->up();
    level_--;
}

void LFTJ::Search() {
    int k = levels_[level_]->iters_.size();
    int x_ = levels_[level_]->iters_[utils::mod(levels_[level_]->p-1, k)]->key();
    while(true) {
        int x = levels_[level_]->iters_[levels_[level_]->p]->key();
        if (x == x_) {
            UpdateResult();
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

void LFTJ::UpdateResult() {
    if(level_ == 0) levels_[level_]->count = 1;
    if(level_ > 0) levels_[level_]->count = levels_[level_-1]->count;
    for (auto iter: levels_[level_]->iters_) {
        if(iter->isLeaf()) levels_[level_]->count *= iter->count();
    }
    if(level_ == dH_->attrs - 1) {
        count_ += levels_[level_]->count;
        resultTable.push_back();
    }
}

bool LFTJ::AnyIterAtEnd() {
    for(auto iter: levels_[level_]->iters_) {
        if (iter->atEnd()) return true;
    }
    return false;
}
