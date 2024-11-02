#include <vector>
#include <string>
#include <regex>

class UrlUtils 
{
public:
	static std::vector<std::string>
		Split(std::string_view spliting_string);

	static std::vector<std::string>
		SplitStream(const std::string& spliting_stream);

	static void 
		CheckUrlCorrectness(std::nothrow_t nthw, std::string_view url) noexcept;

	static bool 
		CheckUrlCorrectness(std::string_view url);


	static bool 
		CheckUrlCorrectness(const std::string& url);

	
	static bool 
		CheckUrlCorrectness(const char* url);


	static void 
		EraseQueryParams(std::string& url) noexcept;


	const static std::regex urlPathRegex;
};

// template <typename String>
// Url<String>::Url(const String& url)
// 	: m_raw(url)
// {
// 	// nothing
// };


// namespace boost { namespace property_tree {

	// /*explicit*/ path_of<Url>::type::type(key_type urlPath)
	// 	: m_path(std::move(urlPath.m_raw))
	// 	, m_parseIdx(0ul)
	// {
	// 	// nothing
	// };


	// template <typename String>
	// path_of<overloading_type>::type::type(const String& urlRawPath)
	// 	: m_path(urlRawPath)
	// 	, m_parseIdx(0ul)
	// {
	// 	// nothing
	// };

// }; /*property_tree*/}; // boost

// template<>
// std::string
// boost::property_tree::operator/(path_of<struct Url>::type& urlPath)
// {
// 	std::string path;
// 	auto urlLen = urlPath.m_path.length();
// 	if (parseIdx != urlLen)
// 	{
// 		// check
// 		if (urlPath.m_path[parseIdx] == '/')
// 		{
// 			parseIdx++;
// 		}

// 		nextSlashIdx = urlPath.m_path.find(parseIdx, '/');
// 		if (nextSlashIdx == std::string::npos)
// 		{
// 			nextSlashIdx = urlLen;
// 		}

// 		path = urlPath.m_path.substr(parseIdx, nextSlashIdx);
// 		parseIdx = nextSalshIdx;
// 	}

// 	return path;
// };


template <typename EndpointData_T>
template <typename T, typename U>
Router<EndpointData_T>::Router(
	const T& host, const U& scheme, uint16_t port)
	: m_host(host)
	, m_scheme(scheme)
	, m_port(static_cast<uint16_t>(80))
{
	// nothing
};


static std::string TrimString(
	const std::string& str, char trimmer)
{
	using str_T = std::string;
	
	str_T::const_iterator head = str.cbegin()
						, tail = str.cend();
	while(head != tail)
	{
		if (*head != trimmer)
			break;
		++head;
	}

	while (--tail != head)
	{
		if (*tail != trimmer)
			break;
	}
	
	return str.substr(std::distance(str.cbegin(), head), std::distance(head, tail) + 1);
};



template <class EndpointData_T>
Router<EndpointData_T>::LNRIterator::operator++()
{
	if (m_node->m_ptrParentNode)
	{
		// we reached end of deep
		// go to parent
		m_node = m_node->m_ptrParentNode;
	} else 
	{
		//
	}
}



template <typename EndpointData_T> 
template <typename String1, typename String2>
// std::pair<Router<EndpointData_T>::iterator, bool>
int
Router<EndpointData_T>::InsertRoute(
	const String1& url
	, const EndpointData_T& value
	, const String2& realm)
{
	if (UrlUtils::CheckUrlCorrectness(url))
	{
		auto trimmedUrlPath =
			TrimString(url, '/');

		// construct and move
		// m_router.put(path_type{trimmedUrlPath, '/'},
		// 	Node_T{nullptr, realm, value});
	} else
	{
		throw "Invalid path";
	}

	return 0;
};


// template <typename EndpointData_T> 
// int 
// Router<EndpointData_T>::AddRoute(
// 	const typename Routers_t::key_type& url, 
// 	EndpointData_t* endpoint_data) noexcept(false)
// {
// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	m_router.put(ptree::path_type{url.substr(1ul, url.length() - back_shift), '/'},
// 		std::shared_ptr<EndpointData_t>{endpoint_data});
	
// 	return 0;
// };


// template <typename EndpointData_T>
// template <typename... Args> 
// int Router<EndpointData_T>::AddRoute(std::nothrow_t nthw,
// 	const typename Routers_t::key_type& url, Args&&... args) noexcept
// {
// 	static_assert(std::is_constructible_v<EndpointData_t, Args...>,
// 		"Data type must be constructible with args pack!");

// 	if (!UrlUtils::CheckUrlCorrectness(url))
// 	{
// 		return -1;
// 	}


// 	try
// 	{
// 		std::allocator<EndpointData_t> alloc;
// 		EndpointData_t* temp = std::allocator_traits<decltype(alloc)>::allocate(alloc, 1);
// 		std::allocator_traits<decltype(alloc)>::construct(alloc, temp, std::forward<Args>(args)...);

// 		return AddRoute(nthw, url, temp);
// 	} catch(...)
// 	{
// 		// pass:
// 	}

// 	return -1;
// };


// template <typename EndpointData_T>
// template <typename... Args> 
// int Router<EndpointData_T>::AddRoute(
// 	const typename Routers_t::key_type& url, Args&&... args) noexcept(false)
// {
// 	static_assert(std::is_constructible_v<EndpointData_t, Args...>,
// 		"Data type must be constructible with args pack!");

// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	std::allocator<EndpointData_t> alloc;
// 	EndpointData_t* temp = std::allocator_traits<decltype(alloc)>::allocate(alloc, 1);
// 	std::allocator_traits<decltype(alloc)>::construct(alloc, temp, std::forward<Args>(args)...);

// 	return AddRoute(url, temp);
// };


// template <typename EndpointData_T>	
// typename Router<EndpointData_T>::EndpointData_t*
// Router<EndpointData_T>::FindRoute(std::nothrow_t nthw,
// 	const typename Routers_t::key_type& url) noexcept
// {
// 	if (!UrlUtils::CheckUrlCorrectness(url))
// 	{
// 		return nullptr;
// 	}

// 	try 
// 	{
// 		size_t back_shift = url.back() == '/' ? 1 : 0;

// 		auto boost_optional_value = 
// 			m_router.get_child_optional(ptree::path_type{url.substr(1ul, url.length() - back_shift), '/'});

// 		return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 			nullptr;
// 	} catch(...)
// 	{
// 		// nothing
// 	}

// 	return nullptr;
// };


// template <typename EndpointData_T>	
// typename Router<EndpointData_T>::EndpointData_t*
// Router<EndpointData_T>::FindRoute(
// 	const typename Routers_t::key_type& url) noexcept(false)
// {
// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	auto boost_optional_value = 
// 		m_router.get_child_optional(ptree::path_type{url.substr(1ul, url.length() - back_shift), '/'});

// 	return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 		nullptr;
// };


// template <typename EndpointData_T>	
// const typename Router<EndpointData_T>::EndpointData_t* 
// Router<EndpointData_T>::FindRoute(std::nothrow_t nthw,
// 	const typename Routers_t::key_type& url) const noexcept
// {
// 	(void)nthw;

// 	if (UrlUtils::CheckUrlCorrectness(url))
// 	{
// 		return nullptr;
// 	}

// 	try
// 	{
// 		size_t back_shift = url.back() == '/' ? 1 : 0;

// 		auto boost_optional_value = 
// 			m_router.get_child_optional(ptree::path_type{url.substr(1ul, url.length() - back_shift), '/'});

// 		return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 			nullptr;
// 	} catch(...)
// 	{
// 		// nothing
// 	}

// 	return nullptr;
// };


// template <typename EndpointData_T>	
// const typename Router<EndpointData_T>::EndpointData_t* 
// Router<EndpointData_T>::FindRoute(
// 	const typename Routers_t::key_type& url) const noexcept(false)
// {
// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	auto boost_optional_value = 
// 		m_router.get_child_optional(ptree::path_type{url.substr(1ul, url.length() - back_shift), '/'});

// 	return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 		nullptr;

// };


// template <typename EndpointData_T>	
// std::pair<std::string, 
// 	typename Router<EndpointData_T>::EndpointData_t*>
// Router<EndpointData_T>::FindNearestRoute(
// 		const typename Routers_t::key_type& url) noexcept(false)
// {
// 	auto route_pieces = 
// 		UrlUtils::Split(url);

// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	std::string available_route; available_route.reserve(32);
// 	Routers_t* subtree = &m_router;

// 	for (size_t k = 1; k < route_pieces.size(); ++k)
// 	{
// 		available_route.push_back('/');

// 		try
// 		{
// 			subtree = 
// 				&subtree->get_child(ptree::path_type{route_pieces[k].data(), '/'});

// 			available_route.append(route_pieces[k]);			

// 		} catch (const pt::ptree_bad_path&)
// 		{
// 			break;
// 		}
// 	}

// 	available_route.pop_back(); // remove `/'

// 	return {available_route, subtree->data().get()}; // RVNO?
// };

// template <typename EndpointData_T>
// std::pair<std::string, 
// 	const typename Router<EndpointData_T>::EndpointData_t*>
// Router<EndpointData_T>::FindNearestRoute(
// 		const typename Routers_t::key_type& url) const noexcept(false)
// {
// 	auto route_pieces = 
// 		UrlUtils::Split(url);

// 	UrlUtils::CheckUrlCorrectnessThrow_V(url);

// 	std::string available_route; available_route.reserve(32);
// 	Routers_t* subtree = &m_router;

// 	for (size_t k = 1; k < route_pieces.size(); ++k)
// 	{
// 		available_route.push_back('/');

// 		try
// 		{
// 			subtree = 
// 				&subtree->get_child(ptree::path_type{route_pieces[k].data(), '/'});

// 			available_route.append(route_pieces[k]);
// 		} catch (const pt::ptree_bad_path&)
// 		{
// 			break;
// 		}
// 	}

// 	available_route.pop_back(); // remove `/'

// 	return {available_route, subtree->data().get()}; // RNVO ?
// };