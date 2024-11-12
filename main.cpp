#include <iostream>

#include "Router.hpp"

void TestTraverse()
{
	// traverse to check ptree	
};

int main(int argc, char const *argv[])
{
	Router<int> router("http", "localhost", 80);
	
	auto pair = router.InsertRoute("news/ru/", 10, "realm"); // == (10, true)
	assert(pair.second == true);
	assert((*pair.first).data() == 10);


	pair = router.InsertRoute("news/ru/", 100); // == (10, true)
	assert(pair.second == false); // failed. InsertRoute error.
	assert((*pair.first).data() == 10);


	auto foundIter = router.FindRoute("news/non_exists/");
	assert(foundIter == router.end());


	foundIter = router.FindRoute("news/ru/");
	assert(foundIter->data() == 10);
	assert(foundIter->realm() == "realm");

	return 0;
}