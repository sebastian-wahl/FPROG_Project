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

auto duration = [](const auto& stop, const auto& start) {
    return chrono::duration_cast<chrono::milliseconds>(stop - start).count();
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

    //if (!util::isFileExtension(fileextension)) {
    //    return EINVAL;
    //}

    // TODO: iterate directory, read file

    ifstream file(filepath + "/" + fileextension);
    ofstream outFile;

    ostringstream sout;
    copy(istreambuf_iterator<char>(file),
        istreambuf_iterator<char>(),
        ostreambuf_iterator<char>(sout));

    outFile.open("./out.txt");

    auto time1 = chrono::high_resolution_clock::now();
    auto wordList = util::splitText(sout.str());
    auto time2 = chrono::high_resolution_clock::now();

    auto wordCount = wordcount::count(wordList);
    auto time3 = chrono::high_resolution_clock::now();

    printList(wordCount, outFile);
    auto time4 = chrono::high_resolution_clock::now();

    file.close();
    outFile.close();

    cout << "split text: " << duration(time2, time1) << endl;
    cout << "word count: " << duration(time3, time2) << endl;
    cout << "print to file: " << duration(time4, time3) << endl;
    return 0;
}