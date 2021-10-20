//
// Created by Artem on 05.10.2021.
//
#include "trie.h"

TrieNode::TrieNode(int64_t key, int64_t value, std::shared_ptr<TrieNode> l, std::shared_ptr<TrieNode> r,
                   bool terminal)
                   : key_(key), value_(value), l_(l), r_(r), terminal_(terminal) {
}

void TrieNode::TrieWalk(std::map<int64_t, int64_t> &depths, int64_t depth) {
    if (terminal_) {
        depths[key_] = depth;
        return;
    }
    l_->TrieWalk(depths, depth + 1);
    r_->TrieWalk(depths, depth + 1);
}

TrieNode::TrieNode(std::shared_ptr<TrieNode> l, std::shared_ptr<TrieNode> r)
: l_(l), r_(r), key_(l->key_), value_(l->value_ + r->value_), terminal_(false) {
}

bool TrieNode::operator<(const TrieNode& a) const {
    return std::tie(value_, key_) < std::tie(a.value_, a.key_);
}

bool operator< (std::shared_ptr<TrieNode>& a, std::shared_ptr<TrieNode>& b) {
    return (*a) < (*b);
}