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
	template <typename T>
	static void TestFindCommonPathMethod(Router<T>& router)
	{
		assert(router.FindCommonPath(std::string("a")		, std::string("b")		, std::string("c")) 
			== "");
		assert(router.FindCommonPath(std::string("a/b")	, std::string("b/b")		, std::string("a"))
			== "");
		assert(router.FindCommonPath(std::string("a/b")	, std::string("a/b")		, std::string("a/с"))
			== "a");
		assert(router.FindCommonPath(std::string("a/b/c")	, std::string("a/b/d")			, std::string("a/b/d"))
			== "a/b");
		assert(router.FindCommonPath(std::string("a/b/c/a/a/a")		, std::string("a")	, std::string("a/b/c/a/a/a")) 
			== "a");
		assert(router.FindCommonPath(std::string("a/a/a/a")		, std::string("b/a/a/a")	, std::string("b/a/a/a")) 
			== "");
		assert(router.FindCommonPath(std::string("a/b/c/d/e/f")		, std::string("a/b/c/d/e/f/g/h")	, std::string("a/b/c/d/e/f/g")) 
			== "a/b/c/d/e/f");
		assert(router.FindCommonPath(std::string("a/b/c/d/f")		, std::string("a/b/c")	, std::string("a/b/c/d")) 
			== "a/b/c");
	};
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
	

	// {
	// 	router.InsertRoute("news", 69); // 0 cause of prev insertion
	// 	auto foundIter = router.FindRouteOrNearestParent("news/non_exists_lang/last_notice");
	// 	assert(foundIter != router.end() && foundIter->data() == 69);

	// 	foundIter = router.FindRoute("news");
	// 	assert(foundIter != router.end() && foundIter->data() == 69);
	// };


	// NativeTestClass::TestFindCommonPathMethod(router);

	router.clear();

	return 0;
}