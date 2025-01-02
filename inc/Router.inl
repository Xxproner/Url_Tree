#include <utility>

#include "UrlUtils.hpp"
#include "Node_impl.inl"
#include "Iterator_impl.inl"

template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::Router(
	const Key_T& host, const Key_T& scheme, uint16_t port)
	: m_host(host)
	, m_scheme(scheme)
	, m_port(static_cast<uint16_t>(port))
	, m_router(Node_T(EndpointData_T{}, def_realm, nullptr)) // valgrind track : this is uninited value
{
	// if container empty for begin(), end()
	// m_router.put(Path_T{"#", '/'}, Node_T(EndpointData_T{}, def_realm, &m_router));
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




template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::begin()
{
	return iterator(m_router.begin());
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::end()
{
	return iterator(m_router.end());
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::cbegin()
{
	return const_iterator(m_router.begin());
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::cend()
{
	return const_iterator(m_router.end());
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::reference
Router<EndpointData_T, URLChar_T>::operator[](
	const Key_T& url)
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;
	UTREE_ASSERT (thisUrlUtils::
		CheckUrlPathCorrectness(url), "Invalid url path!");

	return InsertLazyDefaultChild(m_router, url).first.data();
};


/**
 * @brief get direct child by binary search
 * Function do not check param correctness, 
 * this is responsibility of the calling func.
 * 
 * @param key is key of direct child. 
 * */
template <typename EndpointData_T, typename URLChar_T>
boost::optional<typename Router<EndpointData_T, URLChar_T>::Router_T&>
// typename Router<EndpointData_T, URLChar_T>::Router_T*
Router<EndpointData_T, URLChar_T>::GetDirectChildOptional(Router_T& parent, const Key_T& key)
{
	using opt = boost::optional<Router_T&>;

	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const typename NativeIter_T::value_type& node, const Key_T& key){
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
 * @brief get child by binary search.
 * Function do not check param correctness, 
 * this is responsibility of the calling func.
 * 
 * @param key is key of child. 
 * */
template <typename EndpointData_T, typename URLChar_T>
boost::optional<typename Router<EndpointData_T, URLChar_T>::Router_T&>
Router<EndpointData_T, URLChar_T>::GetChildOptional(Router_T& parent, const Key_T& key)
{
	using opt = boost::optional<Router_T&>;

	std::vector<Key_T> urlPathPieces = UrlUtils<typename Key_T::value_type, typename Key_T::traits_type>::
		SplitUrlPath(key);
	Router_T* parentNode = nullptr;

	opt child = GetDirectChildOptional(parent, urlPathPieces.front());
	std::size_t index = 1;
	while(child && index < urlPathPieces.size())
	{
		parentNode = std::addressof(child.get());
		child = GetDirectChildOptional(*parentNode, urlPathPieces[index++]);
	}

	return child ? child : opt{};
};



/**
 * @brief put direct child. If child already
 * exists do nothing.
 * Function do not check param correctness, 
 * this is responsibility of the calling func.
 * 
 * @param key is key of direct child.
 * @param child is child to put
 * */
template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Router_T&
Router<EndpointData_T, URLChar_T>::PutDirectChild(Router_T& parent, 
	const Key_T& key, const Router_T& child)
{
	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const typename NativeIter_T::value_type& node, const Key_T& key){
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
 * Function do not check param correctness, 
 * this is responsibility of the calling func.
 * 
 * @param key is key of direct child.
 * @param child is child to add
 * */
template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Router_T&
Router<EndpointData_T, URLChar_T>::AddDirectChild(Router_T& parent, 
	const Key_T& key, const Router_T& child)
{
	bool found = false;
	auto lower = std::lower_bound(parent.begin(), parent.end(),
		key, [&found](const Key_T& key, const typename NativeIter_T::value_type& node){
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




template <typename EndpointData_T, typename URLChar_T>
std::pair<typename Router<EndpointData_T, URLChar_T>::iterator, bool>
// int
Router<EndpointData_T, URLChar_T>::InsertRoute(
	const Key_T& url
	, const EndpointData_T& value
	, const char* realm)
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::
		CheckUrlPathCorrectness(url), "Invalid url path");

	std::vector<Key_T> urlPaths = 
		thisUrlUtils::SplitUrlPath(url);
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
// template <typename EndpointData_T, typename URLChar_T>
// std::pair<typename Router<EndpointData_T, URLChar_T>::iterator, bool>
// Router<EndpointData_T, URLChar_T>::InsertRoute_hint(
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
// 	if (CheckUrlPathCorrectness(url))
// 	{
// 		std::vector<> 
// 	} else 
// 	{
// 		URL_TREE_throw("Invalid path");
// 	}
// };


template <typename EndpointData_T, typename URLChar_T>
void
Router<EndpointData_T, URLChar_T>::clear() noexcept
{
	m_router.clear();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::FindRoute(const Key_T& url)
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::
		CheckUrlPathCorrectness(url), "Invalid url path!");

	if (auto child = GetChildOptional(m_router, url);
			child)
	{
		return iterator{child.get()};
	}

	return end();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::FindRoute(const Key_T& url) const
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::CheckUrlPathCorrectness(url), "Invalid url path!");

	if (auto child = GetChildOptional(m_router, url);
			child)
	{
		return const_iterator{child.get()};
	}

	return cend();
};


template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::FindRoute(const Key_T& url, std::nothrow_t) noexcept
{
	if (!UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
	{
		return end();
	}

	if (auto child = m_router.get_child_optional(Path_T{url, '/'});
				child)
	{
		return iterator{child.get()};
	}

	return end();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::FindRoute(const Key_T& url, std::nothrow_t) const noexcept
{
	if (!UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
	{
		return cend();
	}

	if (auto child = m_router.get_child_optional(Path_T{url, '/'});
				child)
	{
		return const_iterator{child.get()};
	}

	return cend();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::FindRouteOrNearestParent(const Key_T& urlPath)
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::CheckUrlPathCorrectness(urlPath), "Invalid url path!");
	
	std::vector<Key_T> urlPathPieces = 
		UrlUtils<typename Key_T::value_type, typename Key_T::traits_type>::SplitUrlPath(urlPath);

	const std::size_t numPathPieces = urlPathPieces.size();

	Router_T* parent = &m_router;
	boost::optional child = GetDirectChildOptional(
		*parent, urlPathPieces.front());
	
	/* get child until possible */
	std::size_t numCurrPathPiece = 1;
	while (child && numCurrPathPiece < numPathPieces)
	{
		parent = std::addressof(child.get());
		
		child = GetDirectChildOptional(*parent,
			urlPathPieces[numCurrPathPiece++]);
	}

	if (child)
	{
		return iterator{child.get()};
	}

	return iterator{parent};
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::FindRouteOrNearestParent(const Key_T& urlPath) const 
{
	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::CheckUrlPathCorrectness(urlPath), "Invalid url path!");
	
	std::vector<Key_T> urlPathPieces = 
		UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::SplitUrlPath(urlPath);

	const std::size_t numPathPieces = urlPathPieces.size();

	Router_T* parent = &m_router;
	boost::optional child = GetDirectChildOptional(
		*parent, urlPathPieces.front());
	
	/* get child until possible */
	std::size_t numCurrPathPiece = 1;
	while (child && numCurrPathPiece < numPathPieces)
	{
		parent = std::addressof(child.get());
		
		child = GetDirectChildOptional(*parent,
			urlPathPieces[numCurrPathPiece++]);
	}

	if (child)
	{
		return const_iterator{child.get()};
	}

	return const_iterator{parent};
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::iterator
Router<EndpointData_T, URLChar_T>::FindRouteOrNearestParent(const Key_T& urlPath, std::nothrow_t) noexcept
{
	if (UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(urlPath))
	{
		return end();
	}
	
	std::vector<Key_T> urlPathPieces = 
		UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::SplitUrlPath(urlPath);

	const std::size_t numPathPieces = urlPathPieces.size();

	Router_T* parent = &m_router;
	boost::optional child = GetDirectChildOptional(
		*parent, urlPathPieces.front());
	
	/* get child until possible */
	std::size_t numCurrPathPiece = 1;
	while (child && numCurrPathPiece < numPathPieces)
	{
		parent = std::addressof(child.get());
		
		child = GetDirectChildOptional(*parent,
			urlPathPieces[numCurrPathPiece++]);
	}

	if (child)
	{
		return iterator{child.get()};
	}

	return iterator{parent};
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::FindRouteOrNearestParent(const Key_T& urlPath, std::nothrow_t) const noexcept
{
	if (UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(urlPath))
	{
		return cend();
	}
	
	std::vector<Key_T> urlPathPieces = 
		UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::SplitUrlPath(urlPath);

	const std::size_t numPathPieces = urlPathPieces.size();

	Router_T* parent = &m_router;
	boost::optional child = GetDirectChildOptional(
		*parent, urlPathPieces.front());
	
	/* get child until possible */
	std::size_t numCurrPathPiece = 1;
	while (child && numCurrPathPiece < numPathPieces)
	{
		parent = std::addressof(child.get());
		
		child = GetDirectChildOptional(*parent,
			urlPathPieces[numCurrPathPiece++]);
	}

	if (child)
	{
		return const_iterator{child.get()};
	}

	return const_iterator{parent};
};




// template <typename EndpointData_T, typename URLChar_T>
// typename Router<EndpointData_T, URLChar_T>::iterator
// Router<EndpointData_T, URLChar_T>::MveInsertionPair::MveInsertionPair(
// 	const std::string& url,
// 	EndpointData_T&& data)
// 	: m_url(url)
// 	, m_data(data)
// {
// 	static_assert(std::is_move_constructible_v<EndpointData_T>, "move");
// };


// insert all-in-time
// template <typename EndpointData_T, typename URLChar_T>
// template <typename... Args, typename U>
// std::array<std::pair<typename Router<EndpointData_T, URLChar_T>::iterator, bool>, sizeof... (Args)>
// Router<EndpointData_T, URLChar_T>::InsertAIT(Args&&... args)
// {
// 	std::array<const CpyInsertionPair&, sizeof... (Args)> allDataArr {args...};

// 	UTREE_ASSERT(std::all_of(allDataArr.cbegin(), allDataArr.cend(), [](const CpyInsertionPair& insertionPair){
// 		return UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(insertionPair.url);
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
template <typename EndpointData_T, typename URLChar_T>
template <typename... Args>
typename Router<EndpointData_T, URLChar_T>::Key_T
Router<EndpointData_T, URLChar_T>::FindCommonPath(
	const Key_T& url, 
	const Args&... urls)
{
	// need expand for all that convertable for Key_T
	static_assert(std::conjunction_v<std::is_same<Args, Key_T>...>, "Invalid args' types");

	std::array<const Key_T*, sizeof... (Args)> urlsArr {&urls...};
	std::size_t commonPathPos = url.length();

	std::size_t thatLastSlashPos = 0;
	for (std::size_t i = 0; i < urlsArr.size(); ++i)
	{
		const Key_T& currUrl = *urlsArr[i];
		
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
template <typename EndpointData_T, typename URLChar_T>
std::pair<typename Router<EndpointData_T, URLChar_T>::Router_T&, bool>
Router<EndpointData_T, URLChar_T>::InsertChild(
	Router_T& router,
	const Key_T& path, 
	const EndpointData_T& data)
{
	/* this router type */
	auto iter = InsertRoute(path, data);
	return std::make_pair(iter.first.m_nativeIter.second, iter.second);
}


/**
 * @brief Insert the default node at the given path if
 * child is not exists
 * 
 * @ret return pair of the child and bool indicated whether
 * child was inserted
 */
template <typename EndpointData_T, typename URLChar_T>
std::pair<typename Router<EndpointData_T, URLChar_T>::Router_T&, bool>
Router<EndpointData_T, URLChar_T>::InsertLazyDefaultChild(
	Router_T& router,
	const Key_T& path)
{
	/**
	 * REPLACE: REPLACE NODE
	 * NOTHING: NOTHING
	 * */

	bool isInserted = false;
	std::vector<Key_T> urlPathPieces =
		UrlUtils<typename Key_T::value_type, typename Key_T::traits_type>::SplitUrlPath(path);

	Router_T* parent = std::addressof(router);

	boost::optional child = GetDirectChildOptional(
		*parent, 
		urlPathPieces.front());
	std::size_t index = 1;
	while (child && index < urlPathPieces.size())
	{
		parent = std::addressof(child.get());
		child = GetDirectChildOptional(*parent, urlPathPieces[index++]);
	}

	if (!child)
	{
		/* insert default */
		Node_T defaultData{parent};
		do
		{
			/* this is new born parent line
				we do not need to check order
			*/
			parent = std::addressof(
				parent->add(urlPathPieces[index - 1], defaultData));
		} while(index < urlPathPieces.size());
		
		isInserted = true;
	}

	return {child.get(), isInserted};
}

// template <typename EndpointData_T, typename URLChar_T>
// const typename Router<EndpointData_T, URLChar_T>::Router_T&
// Router<EndpointData_T, URLChar_T>::insert_child(const Path_T& path, 
// 	const EndpointData_T& data) const
// {
// 	/* this router type */
// 	return InsertRoute(path, data).first.m_nativeIter.second;
// }


template <typename F, typename S, std::size_t SIZE,
	typename F_translator, typename S_translator>
class PackToArrayFolder 
{
public:
	typedef std::array<std::pair<F, S>, SIZE> arrPair_T;

	// difference between trivially and default
	// default is passed but trivially not!
	static_assert(std::is_default_constructible_v<arrPair_T>, "Invalid array type!"); 

	// static_assert(std::is_invocable<F_translator>::value && std::is_invocable<S_translator>::value,
	// 	"Invalid tranlators!");

	// since c++17
	// static_assert(std::is_same_v<std::invoke_result<F_translator>::type, F> && 
	// 	std::is_same_v<std::invoke_result<S_translator>, S>, "Invalid tranlators!");

	template <typename... Args>
	std::array<std::pair<F,S>, SIZE> Fold(Args&&... args)
	{
		PlacementMakePair(0, std::forward<Args>(args)...);
		return std::move(foldingArray);
	};
private:
	template <typename T1, typename T2, typename... Args>
	void PlacementMakePair(
		std::size_t& offset,
		T1&& first, T2&& second,
		Args&&... args)
	{
		foldingArray[offset] = std::make_pair(F_translator{}->operator()(first), S_translator{}->operator()(second));
		++offset;

		PlacementMakePair(offset, std::forward<Args>(args)...);
	};


	template <typename T1, typename T2, typename... Args>
	void PlacementMakePair(
		std::size_t&& offset, 
		T1&& first, T2&& second,
		Args&&... args)
	{
		foldingArray[offset] = std::make_pair(F_translator{}->operator()(first), S_translator{}->operator()(second));
		++offset;

		PlacementMakePair(offset, std::forward<Args>(args)...);
	};


	void PlacementMakePair(
		std::size_t&) { };

	arrPair_T foldingArray;
};




/**
 * @brief insert all sibling nodes
 * 
 * @return return array of pair of iter on insertion node and 
 * bool indicates inserted or not
 * */
template <typename EndpointData_T, typename URLChar_T>
template <typename... Args>
std::array<std::pair<typename Router<EndpointData_T, URLChar_T>::iterator, bool>, sizeof... (Args) / 2>
Router<EndpointData_T, URLChar_T>::InsertSiblings(
	const Key_T& commonPath, Args&&... args)
{
	/* Args == const [Key_T|const URLChar_T]& , const EndpointData_t&, Key_T& and etc */
	static_assert(metaUtils::SelectionOp<InsertionFusion, Args...>::value(
		std::make_index_sequence<sizeof... (Args)>{}), "Invalid args!");

	typedef UrlUtils<URLChar_T, URLTraits_T> thisUrlUtils;

	UTREE_ASSERT(thisUrlUtils::CheckUrlPathCorrectness(commonPath), "Invalid url path!");

	auto EmptyLambda = [](auto&& v) { return std::addressof(v); };
	auto AllAsStringToStringView = [](auto&& asString) // const char*, char[N], 
	{
		if constexpr (std::is_same_v<std::remove_cvref_t<decltype(asString)>, Key_T>)
		{
			return std::basic_string_view<URLChar_T, URLTraits_T>(asString.data());
		}

		return std::basic_string_view<URLChar_T, URLTraits_T>(asString);
	};

	auto pack = PackToArrayFolder<std::basic_string_view<URLChar_T, URLTraits_T>, const EndpointData_T*, sizeof... (Args),
		decltype(&AllAsStringToStringView), decltype(&EmptyLambda)>{}.Fold(std::forward<Args>(args)...);

	for (std::size_t i = 0; i < sizeof ... (Args) / 2; i = i + 2)
	{
		/* TODO: optimization needed 
			possible to check all urls in time!
		*/
		auto& key = pack[i].first;
		UTREE_ASSERT(thisUrlUtils::CheckUrlPathCorrectness(pack[i].first) && 
		// TODO: optimization search, if found second '/' break loop;
			(std::count(key.data(), key.data() + key.length(), static_cast<URLChar_T>('/')) == 1), "Invalid url path!");
	}

	std::array<std::pair<iterator, bool>, sizeof... (Args) / 2> arrIterAndIsInserted;

	
	// // check all have same parent route:
	// // this parent route is empty or end with `/'
	// // and last route must not have `/'

	// /* go to the parent */
	// auto& [parent, isInserted] = insert_default_child(m_router, commonPath);

	// if (isInserted)
	// {
	// 	/* we do not need to check existing. Node is a newbie */
	// 	for (std::size_t i = 0; i < sizeof... (Args); i = i + 2)
	// 	{
	// 		arrIterAndIsInserted[i] = {parent.put_child(*pack[i], *pack[i + 1]), true};
	// 	}
	// } else 
	// {
	// 	/* this is one possible solution. Benchmark needed! */
	// 	const std::size_t numberOfChildren = 
	// 		std::distance(parent.begin(), parent.cend());

		
	// 	constexpr std::size_t magicNumber = 15;
	// 	if (numberOfChildren < magicNumber)
	// 	{
	// 		/* O(N^2) */
	// 		std::for_each(parent.begin(), parent.cend());
	// 	} else 
	// 	{
	// 		/* std::unordered_map */
	// 	}

		
	// 	for (std::size_t i = 0; i < sizeof... (Args); ++i)
	// 	{

	// 	}
	// }


	// return arrIterAndIsInserted;
}


// template <typename EndpointData_T, typename URLChar_T>
// typename Router<EndpointData_T::LNRIterator>
// Router<EndpointData_T, URLChar_T>::FindRoute(const std::string& url) /*noexcept*/
// {
// 	if (UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
// 	{
// 		// what happens if url ends with `/'
// 		if (auto child = m_router.get_child_optional(Path_T{url, '/'});
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


// template <typename EndpointData_T, typename URLChar_T>
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


// template <typename EndpointData_T>, typename URLChar_T 
// int 
// Router<EndpointData_T, URLChar_T>::AddRoute(
// 	const typename Routers_t::Key_T& url, 
// 	EndpointData_t* endpoint_data) noexcept(false)
// {
// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	m_router.put(ptree::Path_T{url.substr(1ul, url.length() - back_shift), '/'},
// 		std::shared_ptr<EndpointData_t>{endpoint_data});
	
// 	return 0;
// };


// template <typename EndpointData_T, typename URLChar_T>
// template <typename... Args> 
// int Router<EndpointData_T, URLChar_T>::AddRoute(std::nothrow_t nthw,
// 	const typename Routers_t::Key_T& url, Args&&... args) noexcept
// {
// 	static_assert(std::is_constructible_v<EndpointData_t, Args...>,
// 		"Data type must be constructible with args pack!");

// 	if (!UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
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


// template <typename EndpointData_T, typename URLChar_T>
// template <typename... Args> 
// int Router<EndpointData_T, URLChar_T>::AddRoute(
// 	const typename Routers_t::Key_T& url, Args&&... args) noexcept(false)
// {
// 	static_assert(std::is_constructible_v<EndpointData_t, Args...>,
// 		"Data type must be constructible with args pack!");

// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	std::allocator<EndpointData_t> alloc;
// 	EndpointData_t* temp = std::allocator_traits<decltype(alloc)>::allocate(alloc, 1);
// 	std::allocator_traits<decltype(alloc)>::construct(alloc, temp, std::forward<Args>(args)...);

// 	return AddRoute(url, temp);
// };


// template <typename EndpointData_T>, typename URLChar_T	
// typename Router<EndpointData_T, URLChar_T>::EndpointData_t*
// Router<EndpointData_T, URLChar_T>::FindRoute(std::nothrow_t nthw,
// 	const typename Routers_t::Key_T& url) noexcept
// {
// 	if (!UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
// 	{
// 		return nullptr;
// 	}

// 	try 
// 	{
// 		size_t back_shift = url.back() == '/' ? 1 : 0;

// 		auto boost_optional_value = 
// 			m_router.get_child_optional(ptree::Path_T{url.substr(1ul, url.length() - back_shift), '/'});

// 		return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 			nullptr;
// 	} catch(...)
// 	{
// 		// nothing
// 	}

// 	return nullptr;
// };


// template <typename EndpointData_T>, typename URLChar_T	
// typename Router<EndpointData_T, URLChar_T>::EndpointData_t*
// Router<EndpointData_T, URLChar_T>::FindRoute(
// 	const typename Routers_t::Key_T& url) noexcept(false)
// {
// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	auto boost_optional_value = 
// 		m_router.get_child_optional(ptree::Path_T{url.substr(1ul, url.length() - back_shift), '/'});

// 	return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 		nullptr;
// };


// template <typename EndpointData_T>, typename URLChar_T	
// const typename Router<EndpointData_T, URLChar_T>::EndpointData_t* 
// Router<EndpointData_T, URLChar_T>::FindRoute(std::nothrow_t nthw,
// 	const typename Routers_t::Key_T& url) const noexcept
// {
// 	(void)nthw;

// 	if (UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectness(url))
// 	{
// 		return nullptr;
// 	}

// 	try
// 	{
// 		size_t back_shift = url.back() == '/' ? 1 : 0;

// 		auto boost_optional_value = 
// 			m_router.get_child_optional(ptree::Path_T{url.substr(1ul, url.length() - back_shift), '/'});

// 		return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 			nullptr;
// 	} catch(...)
// 	{
// 		// nothing
// 	}

// 	return nullptr;
// };


// template <typename EndpointData_T>, typename URLChar_T	
// const typename Router<EndpointData_T, URLChar_T>::EndpointData_t* 
// Router<EndpointData_T, URLChar_T>::FindRoute(
// 	const typename Routers_t::Key_T& url) const noexcept(false)
// {
// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	size_t back_shift = url.back() == '/' ? 1 : 0;

// 	auto boost_optional_value = 
// 		m_router.get_child_optional(ptree::Path_T{url.substr(1ul, url.length() - back_shift), '/'});

// 	return boost_optional_value ? boost_optional_value->data().get() : // operator bool
// 		nullptr;

// };


// template <typename EndpointData_T>, typename URLChar_T	
// std::pair<std::string, 
// 	typename Router<EndpointData_T, URLChar_T>::EndpointData_t*>
// Router<EndpointData_T, URLChar_T>::FindNearestRoute(
// 		const typename Routers_t::Key_T& url) noexcept(false)
// {
// 	auto route_pieces = 
// 		UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::Split(url);

// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	std::string available_route; available_route.reserve(32);
// 	Routers_t* subtree = &m_router;

// 	for (size_t k = 1; k < route_pieces.size(); ++k)
// 	{
// 		available_route.push_back('/');

// 		try
// 		{
// 			subtree = 
// 				&subtree->get_child(ptree::Path_T{route_pieces[k].data(), '/'});

// 			available_route.append(route_pieces[k]);			

// 		} catch (const pt::ptree_bad_path&)
// 		{
// 			break;
// 		}
// 	}

// 	available_route.pop_back(); // remove `/'

// 	return {available_route, subtree->data().get()}; // RVNO?
// };

// template <typename EndpointData_T, typename URLChar_T>
// std::pair<std::string, 
// 	const typename Router<EndpointData_T, URLChar_T>::EndpointData_t*>
// Router<EndpointData_T, URLChar_T>::FindNearestRoute(
// 		const typename Routers_t::Key_T& url) const noexcept(false)
// {
// 	auto route_pieces = 
// 		UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::Split(url);

// 	UrlUtils<URLChar_T, URLTraits_T, typename Router::allocator_type>::CheckUrlPathCorrectnessThrow_V(url);

// 	std::string available_route; available_route.reserve(32);
// 	Routers_t* subtree = &m_router;

// 	for (size_t k = 1; k < route_pieces.size(); ++k)
// 	{
// 		available_route.push_back('/');

// 		try
// 		{
// 			subtree = 
// 				&subtree->get_child(ptree::Path_T{route_pieces[k].data(), '/'});

// 			available_route.append(route_pieces[k]);
// 		} catch (const pt::ptree_bad_path&)
// 		{
// 			break;
// 		}
// 	}

// 	available_route.pop_back(); // remove `/'

// 	return {available_route, subtree->data().get()}; // RNVO ?
// };