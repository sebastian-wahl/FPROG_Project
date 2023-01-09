#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <range/v3/all.hpp>
#include "util.hpp"

using namespace util;

namespace wordcount {

	using KeyValueList = std::vector<std::pair<std::string, int>>;

	auto map = [](const auto& wordList){
		return transformAll<KeyValueList>(wordList, [](const auto& word){
			return std::make_pair(word, 1);
		});
	};	

	auto sortBy = [](const auto& collection, const auto fn){
		auto collectionCopy(collection);
		ranges::sort(collectionCopy, fn);
		return collectionCopy;
	};

	auto ascendingWord = [](const auto& a, const auto& b){return a.first < b.first;};
	auto sortByWord = bind(sortBy, std::placeholders::_1, ascendingWord);

	auto descendingCount = [](const auto& a, const auto& b){return a.second > b.second;};
	auto sortByCount = bind(sortBy, std::placeholders::_1, descendingCount);

	auto group = [](const auto& mappedResult){
        auto equalWord = [](const auto& a, const auto& b) {
            return a.first == b.first;
        };

        return mappedResult 
            | ranges::views::chunk_by(equalWord) 
            | ranges::views::transform([](const auto& subrange){return ranges::to<std::vector>(subrange);})
            | ranges::to<std::vector>();
    };
	
	auto accumulateCount = [](const auto& source){
        auto sumCount = [](const auto& a, const auto& b){
            return std::make_pair(b.first, a.second + b.second);
        };

        return accumulateAll(source, sumCount);
    };

	auto reduce = [](const auto& groups){
		return transformAll<KeyValueList>(groups, accumulateCount);
	};

	auto count = [](const auto& wordList){
		return reduce(group(sortByWord(map(wordList))));
	};

	auto mergeLists = [](const auto& wordLists) {
		return sortByCount(reduce(group(sortByWord(concatenateAll(wordLists)))));
	};
}

#endif