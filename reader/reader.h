//
// Created by Artem on 02.10.2021.
//

#pragma once

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Reader {
private:
    std::shared_ptr<std::istream> file_;
    size_t file_size_;
    size_t file_pos_;
    std::vector<bool> buffer_;
    size_t buffer_pos_;
    void ReadByte();

public:
    explicit Reader(const std::string& file_path);
    explicit Reader(std::shared_ptr<std::istream> file);
    int ReadBit();
    bool HasBytesLeft() const;
    bool HasBitsLeft() const;
    void Reset();
    ~Reader() = default;
};
