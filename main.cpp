#include <iostream>

// for testing private methods
#define UNIT_NATIVE_TEST 1

#include "Router.hpp"

void TestTraverse()
{
	// traverse to check ptree	
};

class NativeTestClass
{
public:

	static void TestNodeOrder()
	{
		Router<int> router("http", "localhost", 80);
	
		{
			router.InsertRoute("13", 10, "realm");
			router.InsertRoute("2", 10, "realm");
			router.InsertRoute("22", 10, "realm");
			router.InsertRoute("77", 10, "realm");
			router.InsertRoute("99", 10, "realm");
			router.InsertRoute("17", 10, "realm");

			assert(std::is_sorted(router.m_router.begin(), router.m_router.end(),
				[](const Router<int>::NativeIter_T::value_type& node1, 
				   const Router<int>::NativeIter_T::value_type& node2){ 
					return node1.first > node2. first; 
			}));
		}		
	};

};

int main()
{
	/* check order of insertion values */
	// NativeTestClass::TestNodeOrder();

	Router<int> router("http", "localhost", 80);
	assert(router.empty());
	assert(router.begin() == router.end());
	assert(router.cbegin() == router.cend());

	auto pair = router.InsertRoute("news/ru", 10, "realm"); // == (10, true)
	assert(pair.second == true);
	assert((*pair.first).data() == 10);

	assert(!router.empty());


	pair =	router.InsertRoute("news/ru", 12, "none"); // == (10, false)
	assert(pair.second == false);
	assert(pair.first->data() == 10);


	pair = router.InsertRoute("news/en", 12, "realm");
	assert(pair.second);
	assert(pair.first->data() == 12);

	{
		auto foundIter = router.FindRoute("news/non_exists_lang");
		assert(foundIter == router.end());	
	}


	{
		/* FindRoute and FindRouteOrNearestParent */
		auto foundIter = router.FindRoute("news/ru");
		assert(foundIter->data() == 10);
		assert(std::char_traits<char>::compare(foundIter->realm(), "realm", 
					std::char_traits<char>::length("realm")) == 0);

		foundIter = router.FindRoute("news/en");
		assert(foundIter->data() == 12);


		foundIter = router.FindRouteOrNearestParent("news/ru/today/last_hour");
		assert(foundIter->data() == 10);
		assert(std::char_traits<char>::compare(foundIter->realm(), "realm", 
					std::char_traits<char>::length("realm")) == 0);
	}
	

	{
		/* operator[] */
		router["news"].data() = 100;
		auto foundIter = router.FindRoute("news");
		assert(foundIter != router.end() && foundIter->data() == 100);

		router["news/ru"].data() = 50;
		foundIter = router.FindRoute("news/ru");
		assert(foundIter != router.end() && foundIter->data() == 50);
	};


	{
		/* iterating */
		for (auto iter = router.begin(); iter != router.end(); iter++)
		{
			std::cout << (*iter).data() << std::endl;
		}
	}

	// router.InsertSiblings("news/lang_ru", "today", 10, "yesterday", 9);

	router.clear();

	return 0;
}