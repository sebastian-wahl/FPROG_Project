#include <functional>
#include <numeric>
#include <algorithm>
#include <optional>
#include <ranges>

using namespace std;

template<typename DestinationType>
auto transformAll = [](const auto& source, auto fn){
    DestinationType result;
    transform(source.begin(), source.end(), back_inserter(result), fn);
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

template<typename DestinationType>
auto functionalSort = [](const auto& collection){
	DestinationType collectionCopy;
	copy(collection.begin(), collection.end(), back_inserter(collectionCopy));
	ranges::sort(collectionCopy);
	return collectionCopy;
};


int main(/*int argc, char* const argv[]*/) {
	
}