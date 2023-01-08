#include <filesystem>
#include <iostream>
#include <fstream>
#include <optional>
#include <chrono>

#include "wordcount.hpp"
#include "util.hpp"

using namespace std;

void printList(const wordcount::KeyValueList& wordList, ofstream& outFile) {
	for (auto& pair : wordList) {
		outFile << pair.first << " -> " << pair.second << endl;
	}
}

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
    
    wordcount::KeyValueList wordCount = wordcount::mergeLists(
        util::transformAll<vector<wordcount::KeyValueList>>(
            util::transformAll<vector<vector<string>>>(
                util::transformAll<vector<string>>(util::fileList(filepath, fileextension), util::readFile),
            util::split),
        wordcount::count)
    );


    ofstream outFile("./out.txt");

    printList(wordCount, outFile);

    outFile.close();

    return 0;
}