#include <filesystem>
#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>

#include "wordcount.hpp"
#include "util.hpp"

using namespace std;
using namespace std::filesystem;
using namespace std::placeholders;

void printList(const wordcount::KeyValueList& wordList, ofstream& outFile) {
	for (auto& pair : wordList) {
		outFile << pair.first << " -> " << pair.second << endl;
	}
}

auto getPath = [](const auto& dirEntry) {
    return dirEntry.path();
};

auto isFileWithCorrectExtension = [](const auto& path, const auto& fileExtension) {
    return !filesystem::is_directory(path) && is_regular_file(path) && path.extension() == fileExtension;
};

auto listDirectoryEntries = [](const auto& dirpath){
    vector<directory_entry> entries;
    copy(recursive_directory_iterator(dirpath), recursive_directory_iterator(), back_inserter(entries)); 

    return entries;
};

auto fileList = [](const auto& dirpath, const auto& fileExtension) {
    return filter<vector<string>>(transformAll<vector<path>>(listDirectoryEntries(dirpath), getPath), bind(isFileWithCorrectExtension, _1, fileExtension));
};

auto readFile = [](const auto& filePath) {
    ifstream file(filePath);
    string str(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));
    file.close();			

    return str;
};

int main(const int argc, char* const argv[]) {
	
    if (argc != 3) {;
        return EXIT_FAILURE;
    }

    const string filepath = argv[1];
    const string fileextension = argv[2];

    if (!filesystem::is_directory(filepath)) {
        return ENOTDIR;
    }

    if (!util::isFileExtension(fileextension)) {
       return EINVAL;
    }
    
    // accumulate equal words and their count
    wordcount::KeyValueList wordCount = wordcount::mergeLists(
        // count words
        util::transformAll<vector<wordcount::KeyValueList>>(
            // split words
            util::transformAll<vector<vector<string>>>(
                // open file and read all chars
                util::transformAll<vector<string>>(fileList(filepath, fileextension), readFile),
            util::split),
        wordcount::count)
    );

    ofstream outFile("./out.txt");

    printList(wordCount, outFile);

    outFile.close();

    return 0;
}