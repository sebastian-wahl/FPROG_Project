#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/wordcount.hpp"
#include "../src/util.hpp"

using namespace std;

TEST_CASE("split") {
	string input = "**a b c d a a b d c e**";
	vector<string> expected = {"a", "b", "c", "d", "a", "a", "b", "d", "c", "e"};
	
	CHECK_EQ(expected, util::splitText(input));
}

TEST_CASE("wordcount") {
	auto wordList = util::splitText("a b c d a b a b d b c e");
	wordcount::KeyValueList expected = {
		{"b", 4},
		{"a", 3},
		{"c", 2},
		{"d", 2},
		{"e", 1}
	};

	wordcount::KeyValueList actual = wordcount::count(wordList);

	CHECK_EQ(expected, actual);
}