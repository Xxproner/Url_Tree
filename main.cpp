#include <iostream>

#include "Router.hpp"

void TestTraverse()
{
	// traverse to check ptree	
};

int main(int argc, char const *argv[])
{
	Router<int> router("http", "localhost", 80);
	
	auto pair = router.InsertRoute("news/ru", 10, "realm"); // == (10, true)
	assert(pair.second == true);
	assert((*pair.first).data() == 10);


	pair = router.InsertRoute("news/ru", 100); // == (10, true)
	assert(pair.second == false); // failed. InsertRoute error.
	assert((*pair.first).data() == 10);


	{
		auto foundIter = router.FindRoute("news/non_exists");
		assert(foundIter == router.end());		
	}


	{
		auto foundIter = router.FindRoute("news/ru");
		assert(foundIter->data() == 10);
		assert(std::char_traits<char>::compare(foundIter->realm(), "realm", 
					std::char_traits<char>::length("realm")) == 0);

		foundIter = router.FindRouteOrNearestParent("news/ru/smo/last_notice");
		assert(foundIter->data() == 10);
	}
	

	{
		router.InsertRoute("news", 69); // 0 cause of prev insertion
		auto foundIter = router.FindRouteOrNearestParent("news/non_exists_lang/last_notice");
		assert(foundIter != router.end() && foundIter->data() == 69);

		foundIter = router.FindRoute("news");
		assert(foundIter != router.end() && foundIter->data() == 69);
	};


	router.clear();

	return 0;
}