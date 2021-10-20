//
// Created by Artem on 02.10.2021.
//
#include "writer.h"
#include <fstream>

Writer::Writer(const std::string& file_path)
    : file_(std::dynamic_pointer_cast<std::ostream>(std::make_shared<std::ofstream>(file_path, std::ios::binary))),
    buffer_(8, false) {};

Writer::Writer(std::shared_ptr<std::ostream> const file)
    : file_(file), buffer_(8, false) {};

void Writer::WriteByte() {
    char byte = 0;
    for (size_t i = 0; i < buffer_.size(); ++i) {
        if (buffer_[i]) {
            byte |= (1 << (7 - i));
        }
    }
    file_->write(&byte, sizeof(char));
    buffer_pos_ = 0;
}

void Writer::WriteBit(bool bit) {
    buffer_[buffer_pos_++] = bit;
    if (buffer_pos_ == 8) {
        WriteByte();
    }
}

void Writer::Flush() {
    if (buffer_pos_ != 0) {
        for (; buffer_pos_ < 8; ++buffer_pos_) {
            buffer_[buffer_pos_] = 0;
        }
        WriteByte();
    }
}
