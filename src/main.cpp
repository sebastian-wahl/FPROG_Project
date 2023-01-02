#include <filesystem>
#include <iostream>
#include <optional>

#include "wordcount.hpp"
#include "util.hpp"

using namespace std;

void printList(const wordcount::KeyValueList& wordList) {
	for (auto& pair : wordList) {
		cout << pair.first << " -> " << pair.second << endl;
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

    // TODO: iterate directory, read file

    return 0;
}