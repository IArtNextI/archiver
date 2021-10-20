#include "../priority_queue.h"
#include <gtest/gtest.h>

#include <vector>

TEST(PRIORITY_QUEUE, Construction) {
    PriorityQueue<int> pq;

    const std::vector<int> expected_data = {1, 4, 6, 6, 6, 6, 6, 6};
    const std::vector<int> add_data = {1, 4, 6, 3, 2, -10, 3, 4};
    const std::vector<int> expected_data_del = {4, 4, 3, 3, 2, 1, -10};

    for (size_t i = 0; i < add_data.size(); ++i) {
        pq.push(add_data[i]);
        ASSERT_EQ(pq.top(), expected_data[i]);
        ASSERT_TRUE(!pq.empty());
    }
    for (size_t i = 0; i < expected_data_del.size(); ++i) {
        pq.pop();
        ASSERT_EQ(pq.top(), expected_data_del[i]);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
