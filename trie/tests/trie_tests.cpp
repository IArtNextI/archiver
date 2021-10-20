#include "../trie.h"
#include <gtest/gtest.h>

#include <memory>
#include <vector>

TEST(TRIE, Construction) {
    auto a = std::make_shared<TrieNode>(1, 2, nullptr, nullptr, true);
    auto b = std::make_shared<TrieNode>(2, 3, nullptr, nullptr, true);
    auto c = std::make_shared<TrieNode>(3, 1, nullptr, nullptr, true);
    auto d = std::make_shared<TrieNode>(a, b);
    auto e = std::make_shared<TrieNode>(c, d);

    std::map<int64_t, int64_t> result;
    e->TrieWalk(result);

    ASSERT_EQ(result[1], 2);
    ASSERT_EQ(result[2], 2);
    ASSERT_EQ(result[3], 1);
    ASSERT_EQ(result.find(4), result.end());
    ASSERT_EQ(result.find(5), result.end());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
