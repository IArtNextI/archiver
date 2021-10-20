//
// Created by Artem on 05.10.2021.
//

#pragma once

#include <map>
#include <memory>
#include <tuple>

class TrieNode {
private:
    int64_t key_;
    int64_t value_;
    std::shared_ptr<TrieNode> l_;
    std::shared_ptr<TrieNode> r_;
    bool terminal_;

public:
    explicit TrieNode(int64_t key = 0, int64_t value = 0, std::shared_ptr<TrieNode> l = nullptr,
                      std::shared_ptr<TrieNode> r = nullptr, bool terminal = false);
    TrieNode(std::shared_ptr<TrieNode> l, std::shared_ptr<TrieNode> r);
    void TrieWalk(std::map<int64_t, int64_t>& depths, int64_t depth = 0);
    bool operator<(const TrieNode& a) const;
};
