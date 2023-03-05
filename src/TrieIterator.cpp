#include "TrieIterator.h"

using namespace types;

TrieNode::TrieNode (TrieNode* node, int value, bool isLeaf) { 
    this->parent = node;
    this->value_ = value;
    this->isLeaf_ = isLeaf;
};

TrieNode::~TrieNode() {
    if (this->isRoot_) {
        return;
    }
    std::map<int, TrieNode*>::iterator it = this->children.begin();
    while(it != this->children.end()) {
        it->second->~TrieNode();
        it++;
    }
    this->children.clear();
    free(this);
}

TrieIterator::TrieIterator(Table<int> table, std::vector<int> attr_order) {
    this->node = new TrieNode();
    for (auto tuple: table){
        TrieNode* cur = this->node;
        TrieNode* prev = nullptr;
        for (int i = 0; i < attr_order.size(); i++){
            bool leaf_ = false;
            if (i == (attr_order.size() - 1)) {
                leaf_ = true;
            } 
            if (cur->children.find(tuple[attr_order[i]]) == cur->children.end()){
                TrieNode* new_node = new TrieNode(cur, tuple[attr_order[i]], leaf_);
                cur->children[tuple[attr_order[i]]] = new_node;
            } 
            if (leaf_) {
                cur->children[tuple[attr_order[i]]]->count++;
            }
            prev = cur;
            cur = cur->children.find(tuple[attr_order[i]])->second;                
        }
    } 
}  

TrieIterator::~TrieIterator() {
    while (this->node->parent != nullptr) {
        this->up();
    }
    this->node->~TrieNode(); 
}  

void TrieIterator::open(){
        this->node->cit_ = this->node->children.begin();
        this->node = this->node->cit_->second;
        this->node->atEnd_ = false;
}

void TrieIterator::up() { 
        this->node->atEnd_ = false;
        this->node->cit_ = this->node->children.begin();
        this->node = this->node->parent;
}

int TrieIterator::key() { 
    return this->node->value_; 
}

bool TrieIterator::atEnd() {
    if(this->node->atEnd_) {
        return true;
    }
    return false;
}

void TrieIterator::next() {
    if(std::next(this->node->parent->cit_) != this->node->parent->children.end()){
        this->node->parent->cit_++;
        this->node = this->node->parent->cit_->second;
        this->node->atEnd_ = false;
    } else {
        this->node->parent->cit_->second->atEnd_ = true;
    }
}

void TrieIterator::seek(int key) {
    if (node->parent->children.find(key) == node->parent->children.end()) {
        if(node->parent->children.upper_bound(key) == node->parent->children.end()) {
            this->node->parent->cit_->second->atEnd_ = true;
        } else {
            this->node->parent->cit_ =  node->parent->children.upper_bound(key);
            this->node = this->node->parent->cit_->second;
            this->node->atEnd_ = false;
        }
    } else {
        this->node->parent->cit_ = node->parent->children.find(key);
        this->node = this->node->parent->cit_->second;
        this->node->atEnd_ = false;
    }
}

 long long TrieIterator::count() { 
    return this->node->count; 
}

 bool TrieIterator::isLeaf() { 
    return this->node->isLeaf_; 
}