#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/wordcount.hpp"
#include "../src/util.hpp"

using namespace std;

TEST_CASE("concatenate") {
	vector<string> first = {"a", "b"};
	vector<string> second = {"c", "d"};
	vector<string> expected = {"a", "b", "c", "d"};

	CHECK_EQ(expected, util::concatenate(first, second));
}

TEST_CASE("concatenateAll") {
	vector<vector<string>> input = {
		{"a", "b"},
		{"c", "d"},
		{"e", "f"}
	};
	vector<string> expected = {"a", "b", "c", "d", "e", "f"};

	CHECK_EQ(expected, util::concatenateAll(input));
}

TEST_CASE("isFileExtensioń") {
	struct Data {
		string input;
		bool expected;
	} data;

	SUBCASE("with dot") { data.input = ".cpp", data.expected = true; }
	SUBCASE("no dot") { data.input = "cpp", data.expected = false; }
	SUBCASE("two dots") { data.input = "..cpp", data.expected = false; }
	SUBCASE("only dot") { data.input = ".", data.expected = false; }

	CAPTURE(data);
	CHECK_EQ(data.expected, util::isFileExtension(data.input));
}

TEST_CASE("split") {
	string input = "**a b c d a a b d c e**";
	vector<string> expected = {"a", "b", "c", "d", "a", "a", "b", "d", "c", "e"};
	
	CHECK_EQ(expected, util::split(input));
}