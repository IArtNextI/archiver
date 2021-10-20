//
// Created by Artem on 05.10.2021.
//
#include "archiver.h"
#include "../priority_queue/priority_queue.h"
#include "../priority_queue/priority_queue.cpp"
#include "../trie/trie.cpp"

#include <algorithm>
#include <bitset>
#include <memory>

const int64_t FILENAME_END = 256;
const int64_t ONE_MORE_FILE = 257;
const int64_t ARCHIVE_END = 258;
const size_t MAX_CODE_LENGTH = 270;

std::map<int64_t, std::vector<bool>> Archiver::BuildTable(const std::map<int64_t, int64_t>& depths) {
    std::vector<std::pair<int64_t, int64_t>> lengths_sorted(depths.begin(), depths.end());
    std::sort(lengths_sorted.begin(), lengths_sorted.end(),
         [](std::pair<int64_t, int64_t> a, std::pair<int64_t, int64_t> b) {
             return std::tie(a.second, a.first) < std::tie(b.second, b.first);
         });
    std::bitset<MAX_CODE_LENGTH> code;
    std::map<int64_t, std::vector<bool>> result;
    for (size_t i = 0; i < lengths_sorted.size(); ++i) {
        for (int64_t j = 0; j < lengths_sorted[i].second; ++j) {
            result[lengths_sorted[i].first].push_back((code >> (lengths_sorted[i].second - 1 - j))[0]);
        }
        if (i < lengths_sorted.size() - 1) {
            size_t j = 0;
            size_t carry = 1;
            while (j < 270) {
                int cur = (code[j] + carry);
                carry = cur / 2;
                code[j] = cur % 2;
                ++j;
            }
            code <<= (lengths_sorted[i + 1].second - lengths_sorted[i].second);
        }
    }
    return result;
}

void Archiver::AddCode(std::vector<std::vector<int64_t>>& trie, std::vector<int64_t>& terminal, std::vector<bool>& code,
                       int64_t symbol) {
    size_t v = 0;
    for (auto bit : code) {
        if (trie[v][bit] == -1) {
            trie.emplace_back(2, -1);
            terminal.push_back(-1);
            trie[v][bit] = trie.size() - 1;
            v = trie[v][bit];
        } else {
            v = trie[v][bit];
        }
    }
    terminal[v] = symbol;
}

std::map<int64_t, int64_t> Archiver::SymbolFrequencies(const std::string& path) {
    std::map<int64_t, int64_t> result;
    for (auto symbol : path) {
        ++result[static_cast<unsigned char>(symbol)];
    }
    Reader current_reader(path);
    int64_t byte = 0;
    int bit;
    int64_t bits_seen = 0;
    while ((bit = current_reader.ReadBit()) != -1) {
        byte |= (bit << (7 - bits_seen % 8));
        ++bits_seen;
        if (bits_seen % 8 == 0) {
            ++result[byte];
            byte = 0;
        }
    }
    result[FILENAME_END] = 1;
    result[ONE_MORE_FILE] = 1;
    result[ARCHIVE_END] = 1;
    return result;
}

std::shared_ptr<TrieNode> Archiver::BuildTrie(const std::map<int64_t, int64_t>& cnt) {
    PriorityQueue<std::shared_ptr<TrieNode>> vertices;
    for (auto [k, v] : cnt) {
        vertices.push(std::make_shared<TrieNode>(k, -v, nullptr, nullptr, true));
    }
    while (vertices.size() > 1) {
        auto a = vertices.top();
        vertices.pop();
        auto b = vertices.top();
        vertices.pop();
        vertices.push(std::make_shared<TrieNode>(a, b));
    }
    return vertices.top();
}

void Archiver::EncodeFile(Writer& writer, const std::string& file_path, const std::map<int64_t, std::vector<bool>>& table,
                          bool last_file) {
    for (auto symbol : file_path) {
        auto unsigned_symbol = static_cast<unsigned char>(symbol);
        for (auto current_bit : table.at(static_cast<int64_t>(unsigned_symbol))) {
            writer.WriteBit(current_bit);
        }
    }
    for (auto current_bit : table.at(FILENAME_END)) {
        writer.WriteBit(current_bit);
    }

    Reader current_reader(file_path);

    int64_t byte = 0;
    int64_t bits_seen = 0;
    int bit;

    while ((bit = static_cast<int>(current_reader.ReadBit())) != -1) {
        byte |= (bit << (7 - bits_seen % 8));
        ++bits_seen;
        if (bits_seen % 8 == 0) {
            for (auto current_bit : table.at(byte)) {
                writer.WriteBit(current_bit);
            }
            byte = 0;
        }
    }
    if (last_file) {
        for (auto current_bit : table.at(ARCHIVE_END)) {
            writer.WriteBit(current_bit);
        }
    } else {
        for (auto current_bit : table.at(ONE_MORE_FILE)) {
            writer.WriteBit(current_bit);
        }
    }
}

void Archiver::Encode(std::vector<std::string>& files, std::shared_ptr<std::ostream> out) {
    Writer writer(out);
    for (size_t i = 0; i < files.size(); ++i) {
        std::string &file_path = files[i];
        auto cnt = SymbolFrequencies(file_path);
        std::shared_ptr<TrieNode> root = BuildTrie(cnt);
        std::map<int64_t, int64_t> depths;
        root->TrieWalk(depths);
        int64_t total_codes = depths.size();
        for (int j = 0; j < 9; ++j) {
            writer.WriteBit((total_codes >> (8 - j)) & 1);
        }
        for (auto &[code, length] : depths) {
            for (int j = 0; j < 9; ++j) {
                writer.WriteBit((code >> (8 - j)) & 1);
            }
            for (int j = 0; j < 9; ++j) {
                writer.WriteBit((length >> (8 - j)) & 1);
            }
        }
        auto table = BuildTable(depths);
        EncodeFile(writer, file_path, table, static_cast<int64_t>(i + 1) == static_cast<int64_t>(files.size()));
    }
    writer.Flush();
}

std::pair<ArchiverStatus, std::map<int64_t, int64_t>> Archiver::ReadLengths(Reader& reader) {
    int64_t symbols_count = 0;
    std::map<int64_t, int64_t> result;
    for (size_t i = 0; i < 9; ++i) {
        int64_t current_bit = reader.ReadBit();
        if (current_bit == -1) {
            return make_pair(ArchiverStatus::ErrorArchiveCorrupted, result);
        }
        symbols_count |= (current_bit << (8 - i));
    }
    for (size_t i = 0; i < static_cast<size_t>(symbols_count); ++i) {
        int64_t current_code = 0;
        for (size_t j = 0; j < 9; ++j) {
            int64_t current_bit = reader.ReadBit();
            if (current_bit == -1) {
                return make_pair(ArchiverStatus::ErrorArchiveCorrupted, result);
            }
            current_code |= (current_bit << (8 - j));
        }
        int64_t length = 0;
        for (size_t j = 0; j < 9; ++j) {
            int64_t current_bit = reader.ReadBit();
            if (current_bit == -1) {
                return make_pair(ArchiverStatus::ErrorArchiveCorrupted, result);
            }
            length |= (current_bit << (8 - j));
        }
        result[current_code] = length;
    }
    return make_pair(ArchiverStatus::OK, result);
}

ArchiverStatus Archiver::ProcessOneFile(Reader& reader, std::vector<int64_t>& terminal,
                                        std::vector<std::vector<int64_t>>& trie) {
    std::string file_name;
    size_t v = 0;
    while (true) {
        int64_t current_bit = reader.ReadBit();
        if (current_bit == -1) {
            return ArchiverStatus::ErrorArchiveCorrupted;
        }
        v = trie[v][current_bit];
        if (terminal[v] != -1) {
            if (terminal[v] == FILENAME_END) {
                break;
            }
            file_name += static_cast<char>(terminal[v]);
            v = 0;
        }
    }
    Writer writer(file_name);
    v = 0;
    while (true) {
        int64_t current_bit = reader.ReadBit();
        if (current_bit == -1) {
            return ArchiverStatus::ErrorArchiveCorrupted;
        }
        v = trie[v][current_bit];
        if (terminal[v] != -1) {
            if (terminal[v] == ONE_MORE_FILE) {
                return ArchiverStatus::OneMoreFile;
            } else if (terminal[v] == ARCHIVE_END) {
                return ArchiverStatus::OK;
            } else {
                for (int i = 0; i < 8; ++i) {
                    writer.WriteBit((terminal[v] >> (7 - i)) & 1);
                }
                v = 0;
            }
        }
    }
}

void Archiver::BuildArrayTrie(std::map<int64_t, std::vector<bool>>& table, std::vector<std::vector<int64_t>>& trie,
                    std::vector<int64_t>& terminal) {
    terminal.push_back(-1);
    trie.emplace_back(2, -1);
    for (auto &[symbol, code] : table) {
        AddCode(trie, terminal, code, symbol);
    }
}

ArchiverStatus Archiver::Decode(std::shared_ptr<std::istream> in) {
    Reader reader(in);
    while (true) {
        auto lengths_ = ReadLengths(reader);
        if (lengths_.first == ArchiverStatus::ErrorArchiveCorrupted) {
            return ArchiverStatus::ErrorArchiveCorrupted;
        }
        auto table = BuildTable(lengths_.second);

        std::vector<std::vector<int64_t>> trie;
        std::vector<int64_t> terminal;
        BuildArrayTrie(table, trie, terminal);

        auto result = ProcessOneFile(reader, terminal, trie);
        if (result == ArchiverStatus::OneMoreFile) {
            continue;
        } else {
            return result;
        }
    }
}