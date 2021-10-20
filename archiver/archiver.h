//
// Created by Artem on 05.10.2021.
//

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../trie/trie.h"
#include "../writer/writer.h"
#include "../reader/reader.h"

enum class ArchiverStatus { OK, ErrorArchiveCorrupted, OneMoreFile };

class Archiver {
private:
    std::map<int64_t, std::vector<bool>> BuildTable(const std::map<int64_t, int64_t>& depths);
    void AddCode(std::vector<std::vector<int64_t>>& trie, std::vector<int64_t>& terminal, std::vector<bool>& code,
                 int64_t symbol);
    std::map<int64_t, int64_t> SymbolFrequencies(const std::string& path);
    static std::shared_ptr<TrieNode> BuildTrie(const std::map<int64_t, int64_t>& cnt);
    static void EncodeFile(Writer& writer, const std::string& file_path, const std::map<int64_t, std::vector<bool>>& table,
                    bool last_file = false);
    static std::pair<ArchiverStatus, std::map<int64_t, int64_t>> ReadLengths(Reader& reader);
    ArchiverStatus ProcessOneFile(Reader& reader, std::vector<int64_t>& terminal,
                                            std::vector<std::vector<int64_t>>& trie);
    void BuildArrayTrie(std::map<int64_t, std::vector<bool>>& table, std::vector<std::vector<int64_t>>& trie,
                        std::vector<int64_t>& terminal);

public:
    void Encode(std::vector<std::string>& files, std::shared_ptr<std::ostream> out);
    ArchiverStatus Decode(std::shared_ptr<std::istream> in);
};

