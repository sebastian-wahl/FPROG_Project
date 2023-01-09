#include <string>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/wordcount.hpp"
#include "../src/util.hpp"

using namespace std;

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
	vector<string> wordList = {"a", "b", "c", "d", "a", "b"};
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
	vector<string> toGroup = {"denny", "alfred", "berta", "denny", "klaus", "alfred"};

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