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



// template <class EndpointData_T>
// Router<EndpointData_T>::LNRIterator::LNRIterator(
// 	Router<EndpointData_T>::LNRIterator::reference node)
// 	: m_nativeIter(GetIterFromThis(std::addressof(node)))
// {
//	// nothing
// };


// =================================================
// =============== struct LNRIterator ==============
// =================================================

template <class EndpointData_T>
Router<EndpointData_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T>::NativeIter_T nativeIter)
	: m_nativeIter(nativeIter)
{
	// nothing
};



template <class EndpointData_T>
typename Router<EndpointData_T>::NativeIter_T
Router<EndpointData_T>::LNRIterator::GetIterFromThis(
	Router<EndpointData_T>::Router_T* node) const
{
	Router_T* parentOfNode = node->data().Parent();
	NativeIter_T nativeIterSameDeep = parentOfNode->begin();

	while (std::addressof(nativeIterSameDeep->second) != node)
	{
		nativeIterSameDeep++;
	}

	return nativeIterSameDeep;
};



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



template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator&
Router<EndpointData_T>::LNRIterator::operator++()
{
	m_nativeIter = end();
	return *this;
};



template <class EndpointData_T>
typename Router<EndpointData_T>::LNRIterator
Router<EndpointData_T>::LNRIterator::operator++(int)
{
	NativeIter_T retIter = m_nativeIter;
	m_nativeIter = end();

	return  retIter;
};



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


// =================================================
// ============ end struct LNRIterator =============
// =================================================


template <class EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::begin()
{
	return iterator(m_router.begin());
};



template <class EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::end()
{
	return iterator(m_router.end());
};



// =================================================
// =========== struct const_LNRIterator ============
// =================================================

template <class EndpointData_T>
Router<EndpointData_T>::const_LNRIterator::const_LNRIterator(
	Router<EndpointData_T>::ConstNativeIter_T constNativeIter)
	: m_constNativeIter(constNativeIter)
{
	// nothing
};



template <class EndpointData_T>
typename Router<EndpointData_T>::ConstNativeIter_T
Router<EndpointData_T>::const_LNRIterator::GetIterFromThis(
	const Router<EndpointData_T>::Router_T* node) const
{
	const Router_T* parentOfNode = node->data().Parent();
	ConstNativeIter_T nativeIterSameDeep = parentOfNode->cbegin();

	while (std::addressof(nativeIterSameDeep->second) != node)
	{
		nativeIterSameDeep++; // sf!
	}

	return nativeIterSameDeep;
};



template <class EndpointData_T>
Router<EndpointData_T>::const_LNRIterator::const_LNRIterator(
	const Router<EndpointData_T>::Router_T& node)
	: m_constNativeIter(GetIterFromThis(std::addressof(node)))
{
	// nothing
};



template <class EndpointData_T>
Router<EndpointData_T>::const_LNRIterator::const_LNRIterator(
	const Router<EndpointData_T>::Router_T* node)
	: m_constNativeIter(GetIterFromThis(node))
{
	// nothing
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_iterator&
Router<EndpointData_T>::const_LNRIterator::operator++()
{
	m_constNativeIter = end();
	return *this;
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_iterator
Router<EndpointData_T>::const_LNRIterator::operator++(int)
{
	ConstNativeIter_T retIter = m_constNativeIter;
	m_constNativeIter = end();

	return retIter;
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_LNRIterator::reference
Router<EndpointData_T>::const_LNRIterator::Data() const
{
	return m_constNativeIter->second.data();
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_LNRIterator::reference
Router<EndpointData_T>::const_LNRIterator::operator*() const
{
	return this->Data();
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_LNRIterator::pointer
Router<EndpointData_T>::const_LNRIterator::operator->() const
{
	return std::addressof(Data());
};



template <class EndpointData_T>
bool
Router<EndpointData_T>::const_LNRIterator::operator==(const_LNRIterator otherIter) const
{
	return m_constNativeIter == otherIter.m_constNativeIter;
};



template <class EndpointData_T>
bool
Router<EndpointData_T>::const_LNRIterator::operator!=(const_LNRIterator otherIter) const
{
	return not this->operator==(otherIter);
};

// =================================================
// ========= end struct const_LNRIterator ==========
// =================================================

template <class EndpointData_T>
typename Router<EndpointData_T>::const_iterator
Router<EndpointData_T>::cbegin()
{
	return const_iterator(m_router.begin());
};



template <class EndpointData_T>
typename Router<EndpointData_T>::const_iterator
Router<EndpointData_T>::cend()
{
	return const_iterator(m_router.end());
};



template <typename EndpointData_T>
typename Router<EndpointData_T>::reference
Router<EndpointData_T>::operator[](
	const key_type& url)
{
	UTREE_ASSERT (UrlUtils::CheckUrlCorrectness(url), "Invalid url path!");

	return m_router.put(path_type{url, '/'}, EndpointData_T{}).data();
};


/**
 * @brief get direct child by binary search
 * 
 * @param key is key of direct child. 
 * */
template <typename EndpointData_T>
boost::optional<typename Router<EndpointData_T>::Router_T&>
// typename Router<EndpointData_T>::Router_T*
Router<EndpointData_T>::GetDirectChildOptional(Router_T& parent, const key_type& key)
{
	using opt = boost::optional<Router_T&>;

	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const typename NativeIter_T::value_type& node, const key_type& key){
			const auto compare_value = key.compare(node.first);
			if (compare_value == 0)
			{
				found = true;
			}

			return compare_value < 0;
		});
	return found ? opt{lower->second} : opt{};
};


/**
 * @brief put direct child. If child already
 * exists do nothing.
 * 
 * @param key is key of direct child.
 * @param child is child to put
 * */
template <typename EndpointData_T>
typename Router<EndpointData_T>::Router_T&
Router<EndpointData_T>::PutDirectChild(Router_T& parent, 
	const key_type& key, const Router_T& child)
{
	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const typename NativeIter_T::value_type& node, const key_type& key){
			const auto compare_value = key.compare(node.first);
			if (compare_value == 0)
			{
				found = true;
			}

			return compare_value < 0;
		});
	if (!found)
	{
		/* iterator insert(iterator where, const value_type & value); */
		lower = parent.insert(lower, std::make_pair(key, child));
	}

	 return lower->second;
};



/**
 * @brief add direct child. If child already
 * exists replace it.
 * 
 * @param key is key of direct child.
 * @param child is child to add
 * */
template <typename EndpointData_T>
typename Router<EndpointData_T>::Router_T&
Router<EndpointData_T>::AddDirectChild(Router_T& parent, 
	const key_type& key, const Router_T& child)
{
	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const key_type& key, const typename NativeIter_T::value_type& node){
			const auto compare_value = key.compare(node.first);
			if (compare_value == 0)
			{
				found = true;
			}

			return compare_value < 0;
		});

	if (found)
	{
		lower = parent.erase(lower);
	}

	return parent.insert(lower, std::make_pair(key, child)).second;
};




template <typename EndpointData_T>
std::pair<typename Router<EndpointData_T>::iterator, bool>
// int
Router<EndpointData_T>::InsertRoute(
	const key_type& url
	, const EndpointData_T& value
	, const char* realm)
{
	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(url), 
		"Invalid url path");

	auto urlPaths = UrlUtils::SplitString(url);
	const size_t numPaths = urlPaths.size();

	Router_T* parent = &m_router;
	boost::optional<Router_T&> child = GetDirectChildOptional(*parent, urlPaths.front());

	/* get child until possible */
	size_t numCurrentPath = 1;
	while (numCurrentPath < numPaths &&
			child)
	{
		parent = std::addressof(child.get());
		
		child = GetDirectChildOptional(*parent, urlPaths[numCurrentPath++]);
	}

	if (child) // numCurrentPath == numPaths
	{
		return std::make_pair<iterator, bool>(iterator(child.get()), false);
	} else // if (numCurrentPath == numPaths) chlid invalid -- death!
	{
		numCurrentPath--;
		while ((numPaths - numCurrentPath) > 1)
		{
			auto& newBornChild = PutDirectChild(*parent, urlPaths[numCurrentPath++],
				Router_T{Node_T{EndpointData_T{}, realm, parent}});
			
			parent = std::addressof(newBornChild);
		}

		auto& newBornChild = PutDirectChild(*parent, urlPaths.back(),
			Router_T{Node_T{value, realm, parent}});

		return std::make_pair<iterator, bool>(iterator(newBornChild), true);
	}
};


// I am not sure that is appropriate
// template <typename EndpointData_T>
// std::pair<typename Router<EndpointData_T>::iterator, bool>
// Router<EndpointData_T>::InsertRoute_hint(
// 	const std::string& url
// 	, const EndpointData_T& value
// 	, const char* realm
// 	, const_iterator hint)
// {
// 	if (!realm)
// 	{
// 		realm = def_realm;
// 	}

// 	// check deep
// 	if (CheckUrlCorrectness(url))
// 	{
// 		std::vector<> 
// 	} else 
// 	{
// 		URL_TREE_throw("Invalid path");
// 	}
// };


template <typename EndpointData_T>
void
Router<EndpointData_T>::clear() noexcept
{
	m_router.clear();
};



template <typename EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::FindRoute(const key_type& url) /*noexcept*/
{
	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(url), "Invalid url path!");

	if (auto child = m_router.get_child_optional(path_type{url, '/'});
			child)
	{
		return iterator{child.get()};
	}

	return end();
};



template <typename EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::FindRoute(const key_type& url, std::nothrow_t) noexcept
{
	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(url), "Invalid url path!");

	if (auto child = m_router.get_child_optional(path_type{url, '/'});
				child)
	{
		return iterator{child.get()};
	}

	return end();
};



template <typename EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::FindRouteOrNearestParent(const key_type& urlPath) /*noexcept*/
{
	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(urlPath), "Invalid url path!");
	
	std::vector<key_type> urlPathPieces = 
		UrlUtils::SplitString(urlPath);

	const std::size_t numPathPieces = urlPathPieces.size();

	Router_T* parent = &m_router;
	boost::optional child = parent->get_child_optional(
			path_type{urlPathPieces.front(), '/'});
	
	// get child until possible
	std::size_t numCurrPathPiece = 1;
	while (numCurrPathPiece < numPathPieces &&
			child)
	{
		parent = std::addressof(child.get());
		
		child = parent->get_child_optional(
			path_type{urlPathPieces[numCurrPathPiece++], '/'});
	}

	if (child)
	{
		return iterator{child.get()};
	}

	return iterator{parent};
};



template <typename EndpointData_T>
typename Router<EndpointData_T>::iterator
Router<EndpointData_T>::FindRouteOrNearestParent(const key_type& urlPath, std::nothrow_t) noexcept
{
	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(urlPath), "Invalid url path!")

	std::vector<key_type> urlPathPieces = 
		UrlUtils::SplitString(urlPath);

	auto& parent = m_router
		, child = parent.get_child_optional(urlPathPieces.front());

	std::size_t numCurrUrlPathPiece = 1;
	while (numCurrUrlPathPiece < urlPathPieces.size() && 
				child)
	{
		parent = child;
		child = parent.get_child_optional(urlPathPieces[numCurrUrlPathPiece++]);
	}

	return child ? child : parent;
};



// template <typename EndpointData_T>
// typename Router<EndpointData_T>::iterator
// Router<EndpointData_T>::MveInsertionPair::MveInsertionPair(
// 	const std::string& url,
// 	EndpointData_T&& data)
// 	: m_url(url)
// 	, m_data(data)
// {
// 	static_assert(std::is_move_constructible_v<EndpointData_T>, "move");
// };


// insert all-in-time
// template <typename EndpointData_T>
// template <typename... Args, typename U>
// std::array<std::pair<typename Router<EndpointData_T>::iterator, bool>, sizeof... (Args)>
// Router<EndpointData_T>::InsertAIT(Args&&... args)
// {
// 	std::array<const CpyInsertionPair&, sizeof... (Args)> allDataArr {args...};

// 	UTREE_ASSERT(std::all_of(allDataArr.cbegin(), allDataArr.cend(), [](const CpyInsertionPair& insertionPair){
// 		return UrlUtils::CheckUrlCorrectness(insertionPair.url);
// 	}), "Invalid url path!");

// 	std::array<std::pair<iterator, bool>, sizeof... (Args)> insertedIterArr;

// 	std::array<int, sizeof... (Args)> urlDeepsArr;
// 	std::size_t maxDeep = 0ul;

// 	std::transform(allDataArr.cbegin(), allDataArr.cend(), urlDeepsArr.begin(),
// 		[&maxDeep](const CpyInsertionPair& insertionPair){
// 			const std::size_t currDeep = 
// 				insertionPair.url.count('/');
// 			if (currDeep > maxDeep)
// 			{
// 				maxDeep = currDeep;
// 			}

// 			return currDeep;
// 		});

// 	Router_T* parent = &m_router;
// 	for (std::size_t deep = 0; deep < maxDeep; ++deep)
// 	{
		
// 	}

// 	return allDataArr;
// };



/**
 * @brief find common path of url path from a pack of url
 * @params each param(url) must be a correct url path (rfc 3986)
 * 
 * 
 * @return return the common path
 * */
template <typename EndpointData_T>
template <typename... Args>
typename Router<EndpointData_T>::key_type
Router<EndpointData_T>::FindCommonPath(
	const key_type& url, 
	const Args&... urls)
{
	// need expand for all that convertable for key_type
	static_assert(std::conjunction_v<std::is_same<Args, key_type>...>, "Invalid args' types");

	std::array<const key_type*, sizeof... (Args)> urlsArr {&urls...};
	std::size_t commonPathPos = url.length();

	std::size_t thatLastSlashPos = 0;
	for (std::size_t i = 0; i < urlsArr.size(); ++i)
	{
		const key_type& currUrl = *urlsArr[i];
		
		if (currUrl.length() < commonPathPos)
		{
			commonPathPos = currUrl.length();
		}

		for (std::size_t j = 0;
			j < commonPathPos; ++j)
		{
			if (currUrl[j] == '/')
			{
				thatLastSlashPos = j;
			}
			
			if (currUrl[j] != url[j])
			{
				commonPathPos = thatLastSlashPos;
				break;
			}
		}
	}

	return url.substr(0, commonPathPos);
}


/**
 * @brief Insert the node at the given path. Create any missing parents. 
 * If there already is a node at the path, nothing. */
// template <typename EndpointData_T>
// std::pair<typename Router<EndpointData_T>::Router_T&, bool>
// Router<EndpointData_T>::insert_child(
// 	Router_T& router,
// 	const path_type& path, const EndpointData_T& data) 
// {
// 	/* this router type */
// 	auto iter = InsertRoute(path, data)
// 	return {iter.first.m_nativeIter.second, iter.second};
// }


/**
 * @brief Insert the default node at the given path. Create any missing parents.
 * If there already is a node at the path, nothing. */
// template <typename EndpointData_T>
// std::pair<typename Router<EndpointData_T>::Router_T&, bool>
// Router<EndpointData_T>::insert_default_child(
// 	Router_T& router,
// 	const key_type& path)
// {
// 	/**
// 	 * REPLACE: REPLACE NODE
// 	 * NOTHING: NOTHING
// 	 * */
// 	bool isInserted = false;
// 	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(path), "Invalid url path!");

// 	std::vector<std::string> urlPathPieces =
// 		UrlUtils::SplitString(path);

// 	Router_T& parent = m_router;

// 	boost_optional child = parent.get_child_optional(urlPathPieces.front());
// 	std::size_t index = 1;
// 	while (child && index < urlPathPieces.size())
// 	{
// 		parent = child.get();
// 		child = parent.get_child_optional(urlPathPieces[index++]);
// 	}

// 	if (!child)
// 	{
// 		/* insert default */
// 		if (index < urlPathPieces.size())
// 		{
// 			while (index < urlPathPieces.size())
// 			{
// 				parent = parent.put_child(urlPathPieces[index++], 
// 					Node_T(EndpointData_T{}, nullptr, std::addressof(parent)));
// 			}
			
// 			isInserted = true;			
// 		}
// 	}

// 	return {parent, isInserted};
// }

// template <typename EndpointData_T>
// const typename Router<EndpointData_T>::Router_T&
// Router<EndpointData_T>::insert_child(const path_type& path, 
// 	const EndpointData_T& data) const
// {
// 	/* this router type */
// 	return InsertRoute(path, data).first.m_nativeIter.second;
// }

template <typename Fs, typename Sd, std::size_t SIZE, typename... U>
void PlacementMakePair(std::array<std::pair<const Fs&, const Sd&>, SIZE>& buf, 
	std::size_t& offset, 
	const Fs& first, const Fs& second,
	std::size_t value, U&&... args)
{
	buf[offset] = std::make_pair(first, second); // <const char*, std::size_t>
	++offset;

	PlacementMakePair(buf, offset, std::forward<U>(args)...);
};



template <typename Fs, typename Sd, std::size_t SIZE, typename... U>
void PlacementMakePair(std::array<std::pair<const Fs&, const Sd&>, SIZE>& buf, 
	std::size_t&& offset, 
	const Fs& first, const Fs& second, 
	std::size_t value, U&&... args)
{
	buf[offset] = std::make_pair(first, second);
	++offset;

	PlacementMakePair(buf, offset, std::forward<U>(args)...);
};



template <typename Fs, typename Sd, std::size_t SIZE>
void PlacementMakePair(std::array<std::pair<const Fs&, const Sd&>, SIZE>& buf,
	std::size_t& offset)
{
	// nothing
};





/**
 * @brief insert all sibling nodes
 * 
 * @return return array of pair of iter on insertion node and 
 * bool indicates inserted or not
 * */
// template <typename EndpointData_T>
// template <typename... Args, typename>
// std::array<std::pair<typename Router<EndpointData_T>::iterator, bool>, sizeof... (Args) / 2>
// Router<EndpointData_T>::InsertSiblings(
// 	const key_type& commonPath, const Args&... args)
// {
// 	/* Args == const key_type&, const EndpointData_t&, key_type& and etc */

// , std::enable_if_t<
// 			metaUtils::SelectionOp<
// 				InsertionFusion, Args...>::value(std::make_index_sequence<sizeof... Args>{})
// 		>
// 	UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(commonPath), "Invalid url path!");

// 	std::array<std::pair<const key_type*, const EndpointData_T*>> pack;
	
// 	/* fill array */
// 	PlacementMakePair(pack, 0, std::addressof(args)...);

// 	std::array<std::pair<iterator, bool>, sizeof... (Args) / 2> arrIterAndIsInserted;
	
// 	// check all have same parent route:
// 	// this parent route is empty or end with `/'
// 	// and last route must not have `/'
// 	for (std::size_t i = 0; i < sizeof ... (Args) / 2; i = i + 2)
// 	{
		
// 		/* optimization needed 
// 			possible to check all urls in time!
// 		*/
// 		const key_type& url = *pack[i]; // not allowed!
// 		UTREE_ASSERT(UrlUtils::CheckUrlCorrectness(url) && 
// 			(url.find('/') == std::char_traits<CharT>::npos), "Invalid url path!");
// 	}

// 	/* go to the parent */
// 	auto& [parent, isInserted] = insert_default_child(m_router, commonPath);

// 	if (isInserted)
// 	{
// 		/* we do not need to check existing. Node is a newbie */
// 		for (std::size_t i = 0; i < sizeof... (Args); i = i + 2)
// 		{
// 			arrIterAndIsInserted[i] = {parent.put_child(*pack[i], *pack[i + 1]), true};
// 		}
// 	} else 
// 	{
// 		/* this is one possible solution. Benchmark needed! */
// 		const std::size_t numberOfChildren = 
// 			std::distance(parent.begin(), parent.cend());

		
// 		constexpr std::size_t magicNumber = 15;
// 		if (numberOfChildren < magicNumber)
// 		{
// 			/* O(N^2) */
// 			std::for_each(parent.begin(), parent.cend())
// 		} else 
// 		{
// 			/* std::unordered_map */
// 		}

		
// 		for (std::size_t i = 0; i < sizeof... (Args); ++i)
// 		{

// 		}
// 	}


// 	return allDataArr;
// }


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
// 		URL_TREE_throw( "Invalid path";)	
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