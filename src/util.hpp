#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <regex>

using namespace std;

namespace util {		

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

	auto splitText = [](const auto& text){
		return split(text.begin(), text.end());
	};	
}

#endif