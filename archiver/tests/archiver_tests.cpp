#include "../archiver.h"
#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

void TriggerEncode(std::vector<std::string>& files, const std::string& out) {
    Archiver archiver;
    auto out_stream = std::dynamic_pointer_cast<std::ostream>(std::make_shared<std::ofstream>(out, std::ios::binary));
    archiver.Encode(files, out_stream);
}

void TriggerDecode(std::shared_ptr<std::istream> in) {
    Archiver archiver;
    archiver.Decode(in);
}

std::vector<char> ReadBinary(const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    size_t size = in.tellg();
    in.seekg(0);
    std::vector<char> buffer(size);
    for (size_t i = 0; i < size; ++i) {
        char current_symbol;
        in.read(&current_symbol, sizeof(char));
        buffer[i] = current_symbol;
    }
    return buffer;
}

TEST(ARCHIVER, EcodingDecoding) {
    std::vector<std::string> files;
    files.emplace_back("mock/a");
    auto a_binary = ReadBinary("mock/a");
    files.emplace_back("mock/b");
    auto b_binary = ReadBinary("mock/b");
    TriggerEncode(files, "mock/archive");
    auto in_stream = std::dynamic_pointer_cast<std::istream>(std::make_shared<std::ifstream>("mock/archive",
                                                                                             std::ios::binary));
    TriggerDecode(in_stream);
    auto new_a_binary = ReadBinary("mock/a");
    auto new_b_binary = ReadBinary("mock/b");
    ASSERT_EQ(a_binary, new_a_binary);
    ASSERT_EQ(b_binary, new_b_binary);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
