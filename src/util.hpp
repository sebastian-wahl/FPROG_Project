#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <regex>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace std::placeholders;

namespace util {

	template<typename DestinationType>
	auto transformAllIterator = [](const auto& begin, const auto& end, auto fn){
		DestinationType result;
		std::transform(begin, end, std::back_inserter(result), fn);
		return result;
	};	

	template<typename DestinationType>
	auto transformAll = [](const auto& source, auto fn){
		return transformAllIterator<DestinationType>(source.begin(), source.end(), fn);
	};		

	template<typename DestinationType>
	auto filterIterator = [](const auto& begin, const auto& end, auto fn){
		DestinationType filteredResult;
		copy_if(begin, end, std::back_inserter(filteredResult), fn);
		return filteredResult;
	};

	template<typename DestinationType>
	auto filter = [](const auto& source, auto fn){
		return filterIterator<DestinationType>(source.begin(), source.end(), fn);
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

	auto split = [](const auto& begin, const auto& end){
		const regex ws_re("[a-zA-Z0-9ßöäüÖÄÜ]+");
		vector<string> out;
		copy(sregex_token_iterator(begin, end, ws_re, 0),
				sregex_token_iterator(),
				back_inserter(out));
		return out;
	};

	auto splitText = [](const string& text){
		return split(text.begin(), text.end());
	};	

	auto getPath = [](const auto& dirEntry) {
		return dirEntry.path();
	};

	auto isFileWithCorrectExtension = [](const auto& path, const auto& fileExtension) {
		return !filesystem::is_directory(path) && is_regular_file(path) && path.extension() == fileExtension;
	};

	auto fileList = [](const auto& filepath, const auto& fileExtension) {
		return filter<vector<string>>(transformAllIterator<vector<path>>(recursive_directory_iterator(filepath), recursive_directory_iterator(), getPath), bind(isFileWithCorrectExtension, _1, fileExtension));
	};

	auto readFile = [](const auto& filePath) {
		ifstream file(filePath);

		ostringstream sout;
		copy(istreambuf_iterator<char>(file),
			istreambuf_iterator<char>(),
			ostreambuf_iterator<char>(sout));
		file.close();
		return sout.str();
	};
}

#endif