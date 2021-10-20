#include "../writer.h"
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

void WriteOut(std::string path, const std::vector<bool>& data) {
    Writer writer(path);
    for (size_t i = 0; i < data.size(); ++i) {
        writer.WriteBit(data[i]);
    }
}

TEST(Writer, WriteBinaryFile) {
    const std::vector<bool> data = {0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1};
    std::string path = "test_writer_1.bin";

    WriteOut(path, data);
    std::ifstream file(path, std::ios::in);
    std::string s;
    getline(file, s);
    ASSERT_EQ("abc", s);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
