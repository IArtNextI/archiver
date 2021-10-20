#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "archiver/archiver.h"

bool FileExists(std::string path) {
    std::ifstream file(path);
    return file.good();
}

std::vector<std::string> ParseArguments(int argc, char* argv[]) {
    std::vector<std::string> result(argc);
    for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
        result[i] = argv[i];
    }
    return result;
}

void ShowHelp() {
    std::cout << "Usage:\n";
    std::cout << "\tarchiver -c archive_name file1 [file2 ...]\t\tcreates an archive of file1, file2, ... \n";
    std::cout << "\tarchiver -d archive_name\t\t\t\textracts the files from archive and puts them into current";
    std::cout << " directory\n\tarchiver -h\t\t\t\t\t\tdisplays this message" << std::endl;
}

int main(int argc, char* argv[]) {
    auto arguments = ParseArguments(argc, argv);
    if (argc == 3) {
        if (arguments[1] != "-d") {
            ShowHelp();
            return 0;
        }
        std::string archive_path = arguments[2];
        if (!FileExists(archive_path)) {
            std::cout << "Could not find archive" << std::endl;
            return 0;
        }
        auto in_stream = std::dynamic_pointer_cast<std::istream>(std::make_shared<std::ifstream>(archive_path,
                                                                                                 std::ios::binary));
        Archiver archiver;
        auto result = archiver.Decode(in_stream);
        if (result == ArchiverStatus::ErrorArchiveCorrupted) {
            std::cout << "Archive is corrupted" << std::endl;
        }
    } else if (argc > 3) {
        std::vector<std::string> files;
        for (size_t i = 3; i < arguments.size(); ++i) {
            if (!FileExists(arguments[i])) {
                std::cout << "Could not find file " << arguments[i] << std::endl;
                return 0;
            }
            files.push_back(arguments[i]);
        }
        auto out_stream = std::dynamic_pointer_cast<std::ostream>(std::make_shared<std::ofstream>(arguments[2],
                                                                                                  std::ios::binary));
        Archiver archiver;
        archiver.Encode(files, out_stream);
    } else {
        ShowHelp();
    }
    return 0;
}
