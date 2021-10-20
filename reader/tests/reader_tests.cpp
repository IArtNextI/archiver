#include "../reader.h"
#include <gtest/gtest.h>

#include <vector>

TEST(Reader, ReadBinaryFile) {
    const std::vector<bool> expected_data = {0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1};
    Reader reader("mock/test_1.bin");

    for (size_t i = 0; i < expected_data.size(); ++i) {
        ASSERT_TRUE(reader.HasBitsLeft());
        auto current_bit = static_cast<int>(reader.ReadBit());
        ASSERT_EQ(current_bit, expected_data[i]);
    }
    ASSERT_EQ(static_cast<int>(reader.ReadBit()), -1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
