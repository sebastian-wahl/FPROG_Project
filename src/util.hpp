#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <regex>
#include <filesystem>
#include <execution>

using namespace std;
using namespace std::filesystem;
using namespace std::placeholders;

namespace util {

	template<typename DestinationType>
	auto transformAll = [](const auto& source, auto fn){
		DestinationType result;
		result.resize(source.size());
		transform(std::execution::par, source.begin(), source.end(), result.begin(), fn);
		return result;
	};		

	template<typename DestinationType>
	auto filter = [](const auto& source, auto fn){
		DestinationType filteredResult;
		copy_if(source.begin(), source.end(), std::back_inserter(filteredResult), fn);
		return filteredResult;
	};

	auto accumulateAll = [](const auto source, auto fn){
		return accumulate(source.begin(), source.end(), typename decltype(source)::value_type(), fn);
	};

	auto concatenate = [](const auto& first, const auto& second){
		auto result(first);
		result.insert(result.end(), make_move_iterator(second.begin()), make_move_iterator(second.end()));
		return result;
	};

	auto concatenateAll = [](const auto& collection){
		return accumulateAll(collection, concatenate);
	};

	auto findFirst = [](const auto& collection){
		return collection.size() > 0 ? make_optional(collection[0]) : nullopt;
	};

	auto isFileExtension = [](const string& fileextension){
		auto firstChar = findFirst(fileextension);
		return firstChar.has_value() && firstChar.value() == '.';
	};

	auto split = [](const string& input){
		const regex rgx("[a-zA-Z0-9ßöäüÖÄÜ]+");
		vector<string> out;
		copy(sregex_token_iterator(input.begin(), input.end(), rgx, 0),
				sregex_token_iterator(),
				back_inserter(out));
		return out;
	};

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
}

#endif