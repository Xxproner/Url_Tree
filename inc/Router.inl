#include <vector>
#include <string>
#include <regex>
#include <utility>

class UrlUtils 
{
public:
	static std::vector<std::string>
		Split(std::string_view spliting_string);

	static std::vector<std::string>
		SplitString(const std::string& spliting_stream);


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
	, m_port(static_cast<uint16_t>(port))
	, m_router(Node_T(EndpointData_T{}, def_realm, nullptr)) // valgrind track : this is uninited value
{
	// if container empty for begin(), end()
	// m_router.put(path_type{"#", '/'}, Node_T(EndpointData_T{}, def_realm, &m_router));
};


template <typename EndpointData_T>
Router<EndpointData_T>::Node_T::Node_T()
	: m_endpointData(EndpointData_T{})
	, m_realm(def_realm)
	, m_ptrParent(nullptr)
{
	// nothing

	// why this contructed?
}



template <typename EndpointData_T>
Router<EndpointData_T>::Node_T::Node_T(
	EndpointData_T data, const char* realm, Router_T* const ptrParent)
	: m_endpointData(data)
	, m_realm(realm)
	, m_ptrParent(ptrParent)
{
	// nothing
}


template <typename EndpointData_T>
Router<EndpointData_T>::Node_T::Node_T(
	const Node_T& otherNode)
	: m_endpointData(otherNode.m_endpointData)
	, m_realm(otherNode.m_realm)
	, m_ptrParent(otherNode.m_ptrParent)
{
	// nothing
};


// template <typename EndpointData_T>
// Router<EndpointData_T>::Node_T::Node_T(
// 	Node_T&& otherNode)
// 	: m_endpointData(std::move(otherNode.m_endpointData))
// 	, m_realm(otherNode.m_realm)
// 	, m_ptrParent(otherNode.m_ptrParent)
// {
// 	// nothing
// 	// m_realm = nullptr;
// 	// m_ptrParent = nullptr
// };



template <typename EndpointData_T>
typename Router<EndpointData_T>::Node_T&
Router<EndpointData_T>::Node_T::operator=(
	const Node_T& otherNode)
{
	if (this != &otherNode)
	{
		m_endpointData = otherNode.m_endpointData;
		m_realm = otherNode.m_realm;
		m_ptrParent = otherNode.m_ptrParent;
	}

	return *this;
};




// template <typename EndpointData_T>
// typename Router<EndpointData_T>::Node_T&
// Router<EndpointData_T>::Node_T::operator=(
// 	Node_T&& otherNode)
// {
// 	m_endpointData = std::move(m_endpointData);
// 	m_realm = otherNode.m_realm;
// 	m_ptrParent = otherNode.m_ptrParent;

// 	return *this;
// };


template <typename EndpointData_T>
typename Router<EndpointData_T>::Router_T*
Router<EndpointData_T>::Node_T::Parent() const
{
	return m_ptrParent;	
};


template <typename EndpointData_T>
typename Router<EndpointData_T>::Router_T*
Router<EndpointData_T>::Node_T::Grandparent() const
{
	// data() return not valid ref
	Router_T* parent = Parent(); // for debug

	return parent->data().Parent();
};



// template <typename EndpointData_T>
// Router<EndpointData_T>::Node_T::Node_T(
// 	EndpointData_T data
// 	, std::string realm
// 	, typename Router<EndpointData_T>::Router_T* const ptrParent)
	
// 	: m_endpointData(std::move(data))
// 	, m_realm(std::move(realm))
// 	, m_ptrParent(ptrParent)
// {
// 	// nothing
// };


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
Router<EndpointData_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T>::native_iter_T nativeIter)
	: m_nativeIter(nativeIter)
{
	// nothing
};


template <class EndpointData_T>
typename Router<EndpointData_T>::native_iter_T
Router<EndpointData_T>::LNRIterator::GetIterFromThis(
	Router<EndpointData_T>::Router_T* node) const
{
	auto nativeIterSameDeep = node->data().Parent()->begin();

	while (std::addressof(nativeIterSameDeep->second) != node)
	{
		++nativeIterSameDeep;
	}

	return nativeIterSameDeep;
};


// template <class EndpointData_T>
// Router<EndpointData_T>::LNRIterator::LNRIterator(
// 	Router<EndpointData_T>::LNRIterator::reference node)
// 	: m_nativeIter(GetIterFromThis(std::addressof(node)))
// {
// 	// nothing
// };



template <class EndpointData_T>
Router<EndpointData_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T>::Router_T& node)
	: m_nativeIter(GetIterFromThis(std::addressof(node)))
{
	// nothing
};



template <class EndpointData_T>
Router<EndpointData_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T>::Router_T* node)
	: m_nativeIter(GetIterFromThis(node))
{
	// nothing
};


// template <class EndpointData_T>
// typename Router<EndpointData_T>::LNRIterator&
// Router<EndpointData_T>::LNRIterator::operator++()
// {
// 	if (bool hasChild = false; hasChild) // c++17
// 	{
// 		// go deep
// 	} else
// 	{
// 		const auto nativeDeepEndIter = Data().Parent()->end();
// 		m_nativeIter++;

// 		if (m_nativeIter == nativeDeepEndIter) // go deep up
// 		{
// 			// parent always exists on any level
// 			// m_router does not have parent!

// 			/**
// 			 * gcc error: there are no arguments to ‘Grandparent’ that depend on a template parameter, 
// 			 * so a declaration of ‘Grandparent’ must be available*/
// 			if (Data().Grandparent())
// 			{
// 				// deep up _|
// 				m_nativeIter = GetIterFromThis(Data().Parent());
// 			} else
// 			{
// 				// end of iterating
// 				// next increment follows ub!
// 			}
// 		}
// 	}

// 	return *this;
// };



// template <class EndpointData_T>
// typename Router<EndpointData_T>::LNRIterator
// Router<EndpointData_T>::LNRIterator::operator++(int)
// {
// 	const auto retNativeIter = m_nativeIter;
	
// 	if (bool hasChild = false; hasChild) // c++17
// 	{
// 		// go deep
// 	} else
// 	{
// 		const auto nativeDeepEndIter = Data().Parent()->end();
// 		m_nativeIter++;

// 		if (m_nativeIter == nativeDeepEndIter) // go deep up
// 		{
// 			// parent always exists on any level
// 			// m_router does not have parent!

// 			/**
// 			 * gcc error: there are no arguments to ‘Grandparent’ that depend on a template parameter, 
// 			 * so a declaration of ‘Grandparent’ must be available*/
// 			if (Data().Grandparent())
// 			{
// 				// deep up _|
// 				m_nativeIter = GetIterFromThis(Data().Parent());
// 			} else
// 			{
// 				// end of iterating
// 				// next increment follows ub!
// 			}
// 		}
// 	}
	
// 	return LNRIterator{retNativeIter};
// };



template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator::reference
Router<EndpointData_T>::LNRIterator::Data() const
{
	return m_nativeIter->second.data();
};


template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator::reference
Router<EndpointData_T>::LNRIterator::operator*() const
{
	return this->Data();
};



template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator::pointer
Router<EndpointData_T>::LNRIterator::operator->() const
{
	return std::addressof(Data());
};


template <class EndpointData_T>
bool
Router<EndpointData_T>::LNRIterator::operator==(LNRIterator otherIter) const
{
	return m_nativeIter == otherIter.m_nativeIter;
};


template <class EndpointData_T>
bool
Router<EndpointData_T>::LNRIterator::operator!=(LNRIterator otherIter) const
{
	return not this->operator==(otherIter);
};



template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator
Router<EndpointData_T>::begin()
{
	return LNRIterator(m_router.begin());
};


template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator
Router<EndpointData_T>::end()
{
	return LNRIterator(m_router.end());
};



// template <class EndpointData_T>
// typename Router<EndpointData_T>::LNRIterator
// Router<EndpointData_T>::cbegin()
// {
// 	return LNRIterator(m_router.begin());
// };


// template <class EndpointData_T>
// typename Router<EndpointData_T>::LNRIterator
// Router<EndpointData_T>::cend()
// {
// 	return LNRIterator(m_router.end());
// };



template <typename EndpointData_T>
std::pair<typename Router<EndpointData_T>::iterator, bool>
// int
Router<EndpointData_T>::InsertRoute(
	const std::string& url
	, const EndpointData_T& value
	, const char* realm)
{
	if (UrlUtils::CheckUrlCorrectness(url))
	{
		auto urlPaths = UrlUtils::SplitString(
			TrimString(url, '/'));

		const size_t numPaths = urlPaths.size();

		Router_T* parent = &m_router;
		boost::optional child = parent->get_child_optional(
				path_type{urlPaths.front(), '/'});
		
		// get child until possible
		size_t numCurrentPath = 1;
		while (numCurrentPath < numPaths &&
				child)
		{
			parent = std::addressof(child.get());
			
			child = parent->get_child_optional(
				path_type{urlPaths[numCurrentPath++], '/'});
		}
		
		if (child) // numCurrentPath == numPaths
		{
			return std::make_pair<LNRIterator, bool>(LNRIterator(child.get()), false);
		} else // if (numCurrentPath == numPaths) chlid invalid -- death!
		{
			numCurrentPath--;
			while ((numPaths - numCurrentPath) > 1)
			{
				auto& newBornChild = parent->add(path_type{urlPaths[numCurrentPath++], '/'},
					Node_T(EndpointData_T{}, realm, parent));
				
				parent = std::addressof(newBornChild);
			}

			auto& newBornChild = parent->add(path_type{urlPaths.back(), '/'},
				Node_T(value, realm, parent));

			return std::make_pair<LNRIterator, bool>(LNRIterator(newBornChild), true);
		}
	} else
	{
		throw "Invalid path";
	}

	// never reached
};



template <typename EndpointData_T>
void
Router<EndpointData_T>::clear() noexcept
{
	m_router.clear();
};


template <typename EndpointData_T>
typename Router<EndpointData_T>::LNRIterator
Router<EndpointData_T>::FindRoute(const std::string& url) /*noexcept*/
{
	if (UrlUtils::CheckUrlCorrectness(url))
	{
		// what happens if url ends with `/'
		if (auto child = m_router.get_child_optional(path_type{url, '/'});
				child)
		{
			return LNRIterator{child.get()};
		}
	} else 
	{
		throw "zaebis";	
	}

	return end();
};




// template <typename EndpointData_T>
// typename Router<EndpointData_T::LNRIterator>
// Router<EndpointData_T>::FindRoute(const std::string& url) /*noexcept*/
// {
// 	if (UrlUtils::CheckUrlCorrectness(url))
// 	{
// 		// what happens if url ends with `/'
// 		if (auto child = m_router.get_child_optional(path_type{url, '/'});
// 				child)
// 		{
// 			return LNRIterator{child.get()};
// 		}
// 	} else 
// 	{
// 		throw "zaebis";	
// 	}

// 	return end();
// };


// template <typename EndpointData_T>
// template <typename... Args>
// std::pair<iterator, bool>
// EmplaceRoute(
// 	const std::string& url,
// 	const char* realm,
// 	Args&&... args)
// {
// 	return InsertRoute(url, EndpointData_T{std::forward<Args>(args)...},
// 		def_realm);
// };


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