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


#if !defined(__GNUC__)
#error "Only for gcc compiler("
#endif // !defined(__GNUC__)

#if __cpp_exceptions
#	define URL_TREE_throw(x) throw x
#else
#	define URL_TREE_throw(x) std::terminate()
#endif // !__cpp_exceptions


#if defined(__URL_TREE_NO_CORRECTNESS__) &&\
		__URL_TREE_NO_CORRECTNESS__ == 1
#	define UTREE_ASSERT(x, exception) if(true) {}
#else
#	define UTREE_ASSERT(x, exception)\
		if (!(x))\
		{\
			URL_TREE_throw(exception);\
		}
#endif // defined(__URL_TREE_CORRECTNESS__) &&
	   // __URL_TREE_CORRECTNESS__ == 1

namespace pt = boost::property_tree;

namespace metaUtils
{	
	template <typename T>
	struct rule_of_3
	{
		constexpr static std::size_t value = 
			std::is_default_constructible_v<T> &&
			std::is_copy_constructible_v<T> &&
			std::is_move_constructible_v<T>;
	};


	template <typename T>
	struct add_cref
	{
		typedef std::add_const_t<
			std::add_lvalue_reference_t<T>> type;
	};


	template <typename T>
	using add_cref_t = typename add_cref<T>::type;


	template <typename T, T N>
	struct even_sequence;



	template <typename T, T N>
	struct odd_sequence;



	template <std::size_t N>
	struct is_even : std::conditional_t<bool(N % 2 == 0), std::true_type, std::false_type>
	{
	};


	template <std::size_t N>
	constexpr auto is_even_v = is_even<N>::value;
		

	template <class Unary_op<typename>, typename... Args>
	struct SelectionOp
	{
		using pack = std::tuple<Args...>;
		

		template <std::size_t... index>
		static constexpr bool value(std::integer_sequence<std::size_t, index...> seq)
		{
			std::conjunction<
				Unary_op<
					typename std::tuple_element<index, pack>::type, is_even<index>::value
				>...>::value;
				
			return 0;
		};
	};
}; // namespace metaUtils


template <class EndpointData_T
// ,typename Alloc = std::allocator<EndpointData_T>
>
class Router
{
	// EndpointData_T is comparable with bool to indicate whether initide or not
public:
	struct LNRIterator; 
private:
	struct Node_T; // demand copy constructibled

	constexpr static auto cpyCable = std::is_copy_constructible_v<EndpointData_T>;

	constexpr static auto mvCable = std::is_move_constructible_v<EndpointData_T>;

	// think about void cause of deleting allocator<void> from c++17
	static_assert(not std::is_same_v<EndpointData_T, void>, "void");
	// static_assert(not std::is_pointer_v<EndpointData_T>, "ptr");
	static_assert(not std::is_reference_v<EndpointData_T>, "ref");

	// should take care data is inited or not! or optional 
	static_assert(metaUtils::rule_of_3<EndpointData_T>::value, "rule of 3");

	// static_assert(std::is_same_v<decltype(std::declval<EndpointData_T>() != false), bool>, 
	// 	"false initicates default initialization!");

	using key_type = std::string;

	/* children should be sorted */
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


	using CharT = typename key_type::value_type;

	using path_type = typename Router_T::path_type;

	using NativeIter_T = typename Router_T::iterator;
	using ConstNativeIter_T = typename Router_T::const_iterator;


	static boost::optional 
	GetChildOptional(Router_T& router, const key_type&);

public:
	// Requirement of AllocatorAwareContainer
	// using value_type = typename rebindA_T::value_type;

	using value_type = Node_T;
	using reference = std::add_lvalue_reference_t<value_type>;
	using const_reference = const value_type&;


	/**
	 * LNRIterator covers pt::iterator
	 * but its value_type is Node_T!
	 * */
	struct LNRIterator
	{
	private:
		using room_type = Router<EndpointData_T>;
#if !__cplusplus > 202002L
		using value_type = std::add_const_t<
			typename room_type::value_type>; // removed in c++20
#endif // !__cplusplus > 202002L
	public:
		using difference_type = typename NativeIter_T::difference_type;
		// using reference = typename NativeIter_T::reference_type;
		using reference = std::add_lvalue_reference_t<value_type>;
		using pointer = std::add_pointer_t<value_type>;
		using iterator_category = std::output_iterator_tag;

	private:
		NativeIter_T m_nativeIter;
		

		LNRIterator(NativeIter_T);


		LNRIterator(typename room_type::Router_T*);


		LNRIterator(typename room_type::Router_T&);


		NativeIter_T GetIterFromThis(typename room_type::Router_T* node) const;


		reference Data() const;
	public:

		LNRIterator() = default;


		// LegacyIterator is incrementable (the behavior of the expression ++It is defined) 
		LNRIterator& operator++();


		LNRIterator operator++(int);


		reference operator*() const;


		pointer operator->() const;


		bool operator!=(LNRIterator other) const;


		bool operator==(LNRIterator other) const;


		friend Router<EndpointData_T>;
	};


	struct const_LNRIterator
	{
	private:
		using room_type = Router<EndpointData_T>;
#if !__cplusplus > 202002L
		using value_type = std::add_const_t<
			typename room_type::value_type>; // removed in c++20
#endif // !__cplusplus > 202002L
	public:
		using difference_type = typename ConstNativeIter_T::difference_type;
		// using reference = typename NativeIter_T::reference_type;
		using reference = std::add_lvalue_reference_t<value_type>;
		using pointer = std::add_pointer_t<value_type>;
		using iterator_category = std::output_iterator_tag;

	private:
		// native iterator or pointer?
		// std::pair<key_type, property_tree>
		ConstNativeIter_T m_constNativeIter;
		
		// LNRIterator(reference);

		const_LNRIterator(ConstNativeIter_T);


		const_LNRIterator(const typename room_type::Router_T*);


		const_LNRIterator(const typename room_type::Router_T&);


		ConstNativeIter_T GetIterFromThis(const typename room_type::Router_T* node) const;


		reference Data() const;
	public:

		const_LNRIterator() = default;


		const_LNRIterator& operator++();


		const_LNRIterator operator++(int);


		reference operator*() const;


		pointer operator->() const;


		bool operator!=(const_LNRIterator other) const;


		bool operator==(const_LNRIterator other) const;


		friend Router<EndpointData_T>;
	};

	using iterator = struct LNRIterator;
	using const_iterator = struct const_LNRIterator;

	
	// for all that is convertible to std::string
	template <typename T, typename U>
	Router(const T& host, const U& scheme, uint16_t port = 80);


	reference
	operator[](const key_type& url);


	
	std::pair<iterator, bool>
	// int
	InsertRoute(
		const key_type& url,
		const EndpointData_T& value,
		const char* realm = def_realm);


	std::pair<iterator, bool> InsertRoute_hint(
			const key_type& url,
			const EndpointData_T& value,
			const char* realm /*= def_realm*/,
			const_iterator hint);


	iterator FindRoute(const key_type& urlPath) /*noexcept*/;


	iterator FindRoute(const key_type& urlPath, std::nothrow_t) noexcept;


	iterator FindRouteOrNearestParent(const key_type& urlPath) /*noexcept*/;


	iterator FindRouteOrNearestParent(const key_type& urlPath, std::nothrow_t) noexcept;



	// children should be sorted by key_type

	// if move_constructible and
	// move_assignable
	// not needed cause of property_tree 
	// does not care move semantic
	// struct MveInsertionPair
	// {
	// 	MveInsertionPair(const std::string& url, 
	// 		EndpointData_T&& data);

	// 	const std::string& m_url;
	// 	EndpointData_T&& m_data;
	// };


	/* extremely hard */
	template <typename... Args, 
		typename = std::enable_if_t<
			std::conjunction_v<std::is_same<Args, CpyInsertionPair>...>
		>
	>
	std::array<std::pair<iterator, bool>, sizeof... (Args)>
	InsertAIT(Args&&... args);



	template <typename T = void, typename U = void>
	struct InsertionPair : public std::false_type
	{
		InsertionPair(const T&, const U&) = default;
	};



	template <>
	struct InsertionPair<key_type, EndpointData_T> : public std::true_type
	{
		InsertionPair(const key_type& key, const EndpointData_T& data)
			: m_key(key)
			, m_data(data)
		{

		};
		const key_type& m_key;
		const EndpointData_T m_data;
	};



	template <typename T, bool parity>
	struct InsertionFusion;


	template <>
	struct InsertionFusion<key_type, true>
	{
		
	};


	template <>
	struct InsertionFusion<EndpointData_T, false>
	{

	};



	template <typename... Args
		, std::enable_if_t<
			metaUtils::SelectionOp<
				InsertionFusion, Args...>::value(std::make_index_sequence<sizeof... Args>{})
		>
	>
	std::array<std::pair<iterator, bool>, sizeof... (Args) / 2>
	InsertSiblings(const key_type& commonPath, const Args&... args);



	void clear() noexcept; 



	static std::pair<Router_T&, bool>
	insert_child(Router& router, 
		const key_type& path, const EndpointData_T& data);



	static std::pair<Router_T&, bool>
	insert_default_child(Router& router,
		const key_type& path);



	LNRIterator end();



	const_LNRIterator cend();
private:
	LNRIterator begin();



	const_LNRIterator cbegin();



	template <typename... Args,
		typename = std::enable_if_t<
			std::conjunction_v<std::is_same<Args, key_type>...>
		>
	>
	key_type
	FindCommonPath(const key_type& url, const Args&... urls);
	// need expand for all that convertable for key_type

	#if defined(UNIT_NATIVE_TEST)
	friend class NativeTestClass;
	#endif // defined(UNIT_NATIVE_TEST) 

private:
	std::string m_host; // standart allocator
	std::string m_scheme;
	uint16_t m_port;
public:
	Router_T m_router;
};

#include "Router.inl"

#endif // ROUTER_HPP