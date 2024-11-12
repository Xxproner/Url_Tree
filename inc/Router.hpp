#ifndef ROUTER_HPP
#define ROUTER_HPP

#if defined URL_TREE_HEADER
#	error "Url tree error!"
#else
#	define URL_TREE_HEADER
#endif

#include <memory>
#include <type_traits>
#include <new> // std::nothrow_t
#include <string_view>
#include <iterator>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/string_path.hpp"

namespace pt = boost::property_tree;

namespace metaUtils
{	
	template <typename T>
	struct rule_of_3
	{
		constexpr static size_t value = 
			std::is_default_constructible_v<T> &&
			std::is_copy_constructible_v<T> &&
			std::is_move_constructible_v<T>;
	};
}; // metaUtils


// struct Url_T // for property_tree overloading
// {
// 	template <typename String>
// 	Url(const String& url);
// };

// namespace boost {
//   namespace property_tree {
//     template<typename String, typename Translator> class string_path;

//     template<typename Ch, typename Traits, typename Alloc> 
//       struct path_of<std::basic_string< Ch, Traits, Alloc >>;
//     template<typename String, typename Translator> 
//       string_path< String, Translator > 
//       operator/(string_path< String, Translator > p1, 
//                 const string_path< String, Translator > & p2);
//     template<typename String, typename Translator> 
//       string_path< String, Translator > 
//       operator/(string_path< String, Translator > p1, 
//                 const typename String::value_type * p2);
//     template<typename String, typename Translator> 
//       string_path< String, Translator > 
//       operator/(const typename String::value_type * p1, 
//                 const string_path< String, Translator > & p2);
//   }
// }

// namespace boost { namespace property_tree {
	
// 	template<>
// 	struct path_of<std::string> // this overloading already exists
// 	{
// 		typedef std::string key_type; // ptree key_type 
// 		typedef std::string path_type; // 

// 		struct type
// 		{
// 		private:
// 			path_type 	m_path;
// 			size_t 		m_parseIdx;
// 		public:
// 			// all that is convertible to path_type
// 			explicit type(key_type url) = delete;

// 			/**
// 			 * @param urlPath only path either relative or absolute 
// 			 * not url!
// 			 * */
// 			template <typename String>
// 			type(const String& urlRawPath);
// 		};
// 	};

	// template <>
	// std::string operator/(path_of<struct Url>::type&);
	
// };/*property_tree*/}; // boost


template <class EndpointData_T
// ,typename Alloc = std::allocator<EndpointData_T>
>
class Router
{
public:
	struct LNRIterator;
private:
	struct Node_T;
	
	// think about void cause of deleting allocator<void> from c++17
	static_assert(not std::is_same_v<EndpointData_T, void>, "void");
	// static_assert(not std::is_pointer_v<EndpointData_T>, "ptr");
	static_assert(not std::is_reference_v<EndpointData_T>, "ref");

	// should take care data is inited or not! or optional 
	static_assert(metaUtils::rule_of_3<EndpointData_T>::value, "rule of 3");

	using key_type = std::string;
	using Router_T = pt::basic_ptree<key_type, Node_T>;

	constexpr static const char* def_realm = nullptr;



	struct Node_T
	{
		EndpointData_T m_endpointData; // context

		// be carefull of lifetime of m_realm
		const char* m_realm; // realm 


		Node_T();


		Node_T(EndpointData_T, const char*, Router_T*);


		// why should I implement this
		// m_endpointData has copy_ctor
		Node_T(const Node_T& otherNode);


		Node_T& operator=(const Node_T& otherNode);


		// Node_T(Node_T&& otherNode);


		// Node_T& operator=(Node_T&& otherNode);


		const char* realm() const noexcept { return m_realm; };


		EndpointData_T& data() noexcept { return m_endpointData; };


		const EndpointData_T& data() const noexcept { return m_endpointData; };
	private:
		Router_T* /*const*/ m_ptrParent;


		Router_T* Grandparent() const;


		Router_T* Parent() const;
	
		// ~Node_T() = default;

		friend struct LNRIterator;
		// parent ptr is stored in this only for fast iterating
		// only last nodes have parent ptr not null
		// also for iterating
	};

	// using rebindA_T = std::allocator_traits<Alloc>::rebind_alloc<Node_T>;
	// using rebindATrs_T = std::allocator_traits<Alloc>::rebind_traits<Node_T>;


	using CharT = typename key_type::value_type;

	using path_type = typename Router_T::path_type;

	using native_iter_T = typename Router_T::iterator;
	using native_const_iter_T = typename Router_T::const_iterator;
public:
	// Requirement of AllocatorAwareContainer
	// using value_type = typename rebindA_T::value_type;

	using value_type = Node_T;
	using reference = value_type&;
	using const_reference = const value_type&;


	/**
	 * LNRIterator covers pt::iterator
	 * but its value_type is Node_T!
	 * */
	struct LNRIterator
	{
	private:
		using room_type = Router<EndpointData_T>;
		using value_type = typename room_type::value_type; // removed in c++20
	public:
		using difference_type = typename native_iter_T::difference_type;
		// using reference = typename native_iter_T::reference_type;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;

	private:
		// native iterator or pointer?
		// std::pair<key_type, property_tree>
		native_iter_T m_nativeIter;
		
		// LNRIterator(reference);


		LNRIterator(native_iter_T);


		LNRIterator(typename room_type::Router_T*);


		LNRIterator(typename room_type::Router_T&);


		native_iter_T GetIterFromThis(typename room_type::Router_T* node) const;


		reference Data() const;
	public:

		LNRIterator() = default;


		// LNRIterator& operator++();


		// LNRIterator operator++(int);


		reference operator*() const;


		pointer operator->() const;


		bool operator!=(LNRIterator other) const;


		bool operator==(LNRIterator other) const;


		friend Router<EndpointData_T>;
	};

	using iterator = struct LNRIterator;
	using const_iterator = struct const_LNRIterator;

	// using difference_type = typename iterator::deference_type;

	// using allocator_type = rebindA_T;

	// iterator is bidirectional!
	// nlr, lnr, lrn (прямой, центрированный, обратный); url: 
	// https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%85%D0%BE%D0%B4_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%B0

	
	// for all that is convertible to std::string
	template <typename T, typename U>
	Router(const T& host, const U& scheme, uint16_t port = 80);


	std::pair<iterator, bool>
	// int 
	InsertRoute(
		const std::string& url,
		const EndpointData_T& value,
		const char* realm = def_realm);


	// implement noexcept overloading once
	LNRIterator FindRoute(const std::string& url) /*noexcept*/;


	// implement noexcept overloading once
	LNRIterator FindRouteOrNearestParent(const std::string& value) /*noexcept*/;

	// template <typename... Args>
	// std::pair<iterator, bool>
	// EmplaceRoute(
	// 	const std::string& url,
	// 	const char* realm = def_realm,
	// 	Args&&... args);


	// std::pair<iterator, bool>
	// TryEmplaceRoute();


	LNRIterator begin();



	LNRIterator end();



	void clear() noexcept;
	// int AddRoute(const typename key_type& url, EndpointData_t* endpoint_data) noexcept(false); 


	// template <typename... Args>
	// int AddRoute(std::nothrow_t nthw,
	// 	const typename key_type& url, Args&&... args) noexcept;


	// template <typename... Args>
	// int AddRoute(const typename key_type& url, Args&&... args) noexcept(false);


	// std::pair<std::string, EndpointData_T*>
	// FindNearestRoute(const typename key_type& url) noexcept(false);


	// std::pair<std::string, const EndpointData_t*>
	// FindNearestRoute(const typename key_type& url) const noexcept(false);


	// EndpointData_t* FindRoute(std::nothrow_t nthw,
	// 	const typename key_type& url) noexcept;


	// EndpointData_t* FindRoute(
	// 	const typename key_type& url) noexcept(false);


	// const EndpointData_t* FindRoute(std::nothrow_t nthw,
	// 	const typename key_type& url) const noexcept;


	// const EndpointData_t* FindRoute(
	// 	const typename key_type& url) const noexcept(false);

	friend void TestTraverse();
private:
	std::string m_host; // standart allocator
	std::string m_scheme; 
	uint16_t m_port;
public:
	Router_T m_router;
};

#include "Router.inl"

#endif // ROUTER_HPP