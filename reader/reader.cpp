//
// Created by Artem on 02.10.2021.
//
#include "reader.h"

#include <istream>

Reader::Reader(const std::string& file_path) : file_pos_(0), buffer_(8, false), buffer_pos_(8) {
    file_ = std::dynamic_pointer_cast<std::istream>(std::make_shared<std::ifstream>(file_path,
                                                                                    std::ios::binary | std::ios::ate));
    file_size_ = file_->tellg();
    file_->seekg(0);
}

Reader::Reader(std::shared_ptr<std::istream> file) : file_pos_(0), buffer_(8, false), buffer_pos_(8) {
    file_ = file;
    file_->seekg(0, std::ios::end);
    file_size_ = file_->tellg();
    file_->seekg(0);
}

void Reader::Reset() {
    file_->seekg(0);
    file_pos_ = 0;
    buffer_.assign(8, false);
    buffer_pos_ = 8;
}

void Reader::ReadByte() {
    char result;
    file_->read(&result, 1);
    ++file_pos_;
    for (size_t i = 0; i < 8; ++i) {
        buffer_[i] = static_cast<bool>((result >> (7 - i)) & 1);
    }
    buffer_pos_ = 0;
}

bool Reader::HasBytesLeft() const {
    return file_pos_ < file_size_;
}

int Reader::ReadBit() {
    if (buffer_pos_ == 8 && HasBytesLeft()) {
        ReadByte();
    }
    if (buffer_pos_ != 8) {
        return static_cast<int>(buffer_[buffer_pos_++]);
    } else {
        return -1;
    }
}

bool Reader::HasBitsLeft() const {
    return HasBytesLeft() || buffer_pos_ < 8;
}
