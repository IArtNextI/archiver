//
// Created by Artem on 02.10.2021.
//

#pragma once

#include <memory>
#include <ostream>
#include <vector>

class Writer {
private:
    std::shared_ptr<std::ostream> file_;
    std::vector<bool> buffer_;
    size_t buffer_pos_ = 0;
    void WriteByte();

public:
    explicit Writer(const std::string& file_path);
    explicit Writer(std::shared_ptr<std::ostream> file);
    void WriteBit(bool bit);
    void Flush();
    ~Writer() = default;
};
