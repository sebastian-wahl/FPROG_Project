#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/wordcount.hpp"
#include "../src/util.hpp"

using namespace std;
using namespace ranges;

using stringList = vector<string>;

// generators
auto genRandomString = [](int len) {
    vector<char> chars = views::concat(views::ints(48, 58), views::ints(65, 91), views::ints(97, 123), views::ints(45, 46)) | views::sample(len) | views::transform([](int asciiCode){ return char(asciiCode); }) | to<vector>();
	string aString(chars.begin(), chars.end());
	return aString;
};

auto genRandomStringVector = [](int min, int wordCount) {		
	return views::ints(min, min+wordCount) | views::transform(genRandomString) | to<vector>();
};

auto nWordStringGenerator = [](const int wordCount) {
	return views::ints(0, 10) | views::transform(bind(genRandomStringVector, std::placeholders::_1, wordCount)) | to<vector>();
};
auto oneWordStringGenerator = bind(nWordStringGenerator, 1);

auto doubleStringSingleListGenerator = []() {
	auto generated = oneWordStringGenerator();
	return util::transformAll<vector<stringList>>(generated, [](auto const& list) {
		return util::concatenate(list, list);
	});
};
auto doubleStringDoubleListGenerator = []() {
	auto generated = oneWordStringGenerator();
	return util::transformAll<vector<vector<stringList>>>(generated, [](auto const& list) {
		return vector<stringList>({list, list});
	});
};

// properties
auto prop_count_list_with_one_word = [](const auto& wordList) {
	auto counted = wordcount::count(wordList);
	CHECK_EQ(wordList.size(), 1);
	CHECK_EQ(counted.size(), 1);
	auto first = counted[0];
	return first.second == 1;
};
auto prop_count_list_with_two_words = [](const auto& wordList) {
	auto counted = wordcount::count(wordList);
	CHECK_EQ(wordList.size(), 2);
	CHECK_EQ(counted.size(), 1);
	auto first = counted[0];
	return first.second == 2;
};
auto prop_mergeLists_with_two_words = [](const auto& wordList) {
	auto counted = wordcount::mergeLists(util::transformAll<vector<wordcount::KeyValueList>>(wordList, wordcount::count));
	CHECK_EQ(wordList.size(), 2);
	CHECK_EQ(counted.size(), 1);
	auto first = counted[0];
	return first.second == 2;
};

auto all_of_collection = [](const auto& collection, auto lambda){
	return std::all_of(collection.begin(), collection.end(), lambda);
};


auto check_property = [](const auto& generator, const auto& property){
	auto values = generator();
	CHECK(all_of_collection(values, property));
};

TEST_CASE("wordcountOneWordString") {
	check_property(oneWordStringGenerator, prop_count_list_with_one_word);
}

TEST_CASE("wordcountTwoWordsString") {
	check_property(doubleStringSingleListGenerator, prop_count_list_with_two_words);
}

TEST_CASE("wordcountTwoWordsString") {
	check_property(doubleStringDoubleListGenerator, prop_mergeLists_with_two_words);
}

TEST_CASE("wordcount") {
	auto wordList = util::split("a b c d a b a b d b c e");
	wordcount::KeyValueList expected = {
		{"b", 4},
		{"a", 3},
		{"c", 2},
		{"d", 2},
		{"e", 1}
	};

	wordcount::KeyValueList actual = wordcount::sortByCount(wordcount::count(wordList));

	CHECK_EQ(expected, actual);
}

TEST_CASE("map") {
	stringList wordList = {"a", "b", "c", "d", "a", "b"};
	wordcount::KeyValueList expected = {
		{"a", 1},
		{"b", 1},
		{"c", 1},
		{"d", 1},
		{"a", 1},
		{"b", 1}
	};

	wordcount::KeyValueList actual = wordcount::map(wordList);

	CHECK_EQ(expected, actual);
}

TEST_CASE("sortByWord") {
	wordcount::KeyValueList toSort = {
		{"alfred", 1},
		{"berta", 2},
		{"klaus", 1},
		{"denny", 6},
		{"anna", 1}
	};

	wordcount::KeyValueList expected = {
		{"alfred", 1},
		{"anna", 1},
		{"berta", 2},
		{"denny", 6},
		{"klaus", 1}
	};

	wordcount::KeyValueList actual = wordcount::sortByWord(toSort);

	CHECK_EQ(expected, actual);
}

TEST_CASE("sortByCount") {
	wordcount::KeyValueList toSort = {
		{"alfred", 1},
		{"berta", 2},
		{"klaus", 1},
		{"denny", 6},
		{"anna", 1}
	};

	wordcount::KeyValueList expected = {
		{"denny", 6},
		{"berta", 2},
		{"alfred", 1},
		{"klaus", 1},
		{"anna", 1}
	};

	wordcount::KeyValueList actual = wordcount::sortByCount(toSort);

	CHECK_EQ(expected, actual);
}

TEST_CASE("group") {
	wordcount::KeyValueList toGroup = {
		{"alfred", 1},
		{"alfred", 1},
		{"berta", 1},
		{"denny", 1},
		{"denny", 1},
		{"klaus", 1}
	};

	vector<wordcount::KeyValueList> expected = {
		{
			{"alfred", 1},
			{"alfred", 1}
		},
		{
			{"berta", 1},
		},
		{
			{"denny", 1},
			{"denny", 1}
		},
		{
			{"klaus", 1}
		}
	};

	vector<wordcount::KeyValueList> actual = wordcount::group(toGroup);

	CHECK_EQ(expected, actual);
}

TEST_CASE("accumulateCount") {
	wordcount::KeyValueList toAccumulate = {
		{"alfred", 1},
		{"alfred", 1},
	};

	std::pair<std::string, int> expected = {"alfred", 2};

	std::pair<std::string, int> actual = wordcount::accumulateCount(toAccumulate);

	CHECK_EQ(expected, actual);
}

TEST_CASE("reduce") {
	vector<wordcount::KeyValueList> toGroup = {
		{
			{"alfred", 1},
			{"alfred", 1}
		},
		{
			{"berta", 1},
		},
		{
			{"denny", 1},
			{"denny", 1}
		},
		{
			{"klaus", 1}
		}
	};

	wordcount::KeyValueList expected = {
		{"alfred", 2},
		{"berta", 1},
		{"denny", 2},
		{"klaus", 1}
	};

	wordcount::KeyValueList actual = wordcount::reduce(toGroup);

	CHECK_EQ(expected, actual);
}

TEST_CASE("count") {
	stringList toGroup = {"denny", "alfred", "berta", "denny", "klaus", "alfred"};

	wordcount::KeyValueList expected = {
		{"alfred", 2},
		{"berta", 1},
		{"denny", 2},
		{"klaus", 1}
	};

	wordcount::KeyValueList actual = wordcount::count(toGroup);

	CHECK_EQ(expected, actual);
}

TEST_CASE("mergeLists") {
	vector<wordcount::KeyValueList> wordLists = {
		{
			{"alfred", 2},
			{"berta", 1},
			{"denny", 2},
			{"klaus", 1}
		},
		{
			{"berta", 3},
			{"denny", 1},
			{"klaus", 4}
		}
	};

	wordcount::KeyValueList expected = {
		{"klaus", 5},
		{"berta", 4},
		{"denny", 3},
		{"alfred", 2}
	};

	wordcount::KeyValueList actual = wordcount::mergeLists(wordLists);

	CHECK_EQ(expected, actual);
}