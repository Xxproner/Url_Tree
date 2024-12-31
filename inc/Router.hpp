#ifndef _ROUTER_HPP_
#define _ROUTER_HPP_

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

#include <boost/optional.hpp>
#include "boost/property_tree/ptree.hpp"


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



	template <typename T>
	using ptr = std::add_pointer_t<T>;


	template <std::size_t N>
	struct is_even : std::conditional_t<bool(N % 2 == 0), std::true_type, std::false_type>
	{
	};


	template <std::size_t N>
	constexpr auto is_even_v = is_even<N>::value;
		

	template <template <typename, std::size_t> class Unary_op, typename... Args>
	struct SelectionOp
	{
		using pack = std::tuple<Args...>;

		template <std::size_t... index>
		static constexpr bool value(std::integer_sequence<std::size_t, index...>)
		{
			return std::conjunction<
				Unary_op<
					typename std::tuple_element<index, pack>::type, index
				>...>::value;
		};
	};


	template <
	    typename T,
	    typename Cont_T,
	    typename Alloc_T
	>
	struct is_erasable
	{
	    constexpr static auto value = 
	        std::is_invocable_v<typename std::allocator_traits<Alloc_T>::destroy, Alloc_T, std::add_pointer_t<T>> &&
	        std::is_same_v<typename Cont_T::value_type, T> &&
	        std::is_same_v<typename Cont_T::allocator_type, typename std::allocator_traits<Alloc_T>::template rebind_alloc<T>>;
	};

	template <
	    typename T,
	    typename Cont_T,
	    typename Alloc_T
	>
	using is_erasable_v = struct is_erasable<T, Cont_T, Alloc_T>;
}; // namespace metaUtils


#if __cplusplus < 202002L
namespace std
{
	template<class T>
	struct remove_cvref
	{
	    using type = remove_cv_t<remove_reference_t<T>>;
	};

	template <class T>
	using remove_cvref_t = typename remove_cvref<T>::type;

}; // namespace std
#endif // !__cplusplus > 202002L


template <
	class EndpointData_T,
	class URLChar_T = char>
class Router
{
	// EndpointData_T is comparable with bool to indicate whether initiate or not
public:
	struct LNRIterator; 
private:

	using Self_T = Router<EndpointData_T, URLChar_T>;

	constexpr static auto cpyCable = std::is_copy_constructible_v<EndpointData_T>;
	constexpr static auto mvCable = std::is_move_constructible_v<EndpointData_T>;

	// this types are not erasable (https://en.cppreference.com/w/cpp/named_req/Erasable)


	// think about void cause of deleting allocator<void> from c++17
	static_assert(not std::is_same_v<EndpointData_T, void>, "void");
	static_assert(not std::is_reference_v<EndpointData_T>, "ref");
	// static_assert(not std::is_pointer_v<EndpointData_T>, "ptr");

	// should take care data is inited or not! or optional 
	static_assert(metaUtils::rule_of_3<EndpointData_T>::value, "rule of 3");

	// static_assert(std::is_same_v<decltype(std::declval<EndpointData_T>() != false), bool>, 
	// 	"false initicates default initialization!");

	using URLTraits_T = std::char_traits<URLChar_T>;
	using Key_T = std::basic_string<URLChar_T, URLTraits_T>;

	struct Node_T;
	/* children sorted by descending order */
	using Router_T = pt::basic_ptree<Key_T, Node_T>; // self_type

	constexpr static const URLChar_T* def_realm = nullptr;

	struct Node_T
	{
		EndpointData_T m_endpointData; // content


		Node_T(Router_T* const parent = nullptr);


		Node_T(EndpointData_T data, const char* realm, Router_T* parent);


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
		const char* m_realm;
		Router_T* const m_ptrParent;


		Router_T* Grandparent() const;


		Router_T* Parent() const;
	
		// ~Node_T() = default;


		friend struct LNRIterator;
	};


	using Char_T = typename Key_T::value_type;

	using Path_T = typename Router_T::path_type;

	using NativeIter_T = typename Router_T::iterator;
	using ConstNativeIter_T = typename Router_T::const_iterator;


	static boost::optional<Router_T&>
	GetDirectChildOptional(Router_T& parent, const Key_T& key);



	static boost::optional<Router_T&>
	GetChildOptional(Router_T& parent, const Key_T& key);



	static Router_T&
	PutDirectChild(Router_T& parent, 
		const Key_T& key, const Router_T& child);



	static Router_T&
	AddDirectChild(Router_T& parent, 
		const Key_T& key, const Router_T& child);

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
		using room_type = Self_T;
	public:
#if !__cplusplus > 202002L
		using value_type = std::add_const_t<
			typename room_type::value_type>; // removed in c++20
#endif // !__cplusplus > 202002L

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


		friend Router<EndpointData_T, URLChar_T>;
	};
	using iterator = struct LNRIterator;

	struct const_LNRIterator
	{
	private:
		using room_type = Self_T;
#if __cplusplus < 202002L
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
		ConstNativeIter_T m_constNativeIter;

		const_LNRIterator(ConstNativeIter_T);


		const_LNRIterator(typename room_type::iterator);


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


		friend Router<EndpointData_T, URLChar_T>;
	};
	using const_iterator = struct const_LNRIterator;

	using difference_type = typename iterator::difference_type;
	using size_type = std::size_t;
	
	Router(const Key_T& host, const Key_T& scheme, uint16_t port = 80);


	reference
	operator[](const Key_T& url);


	
	std::pair<iterator, bool>
	InsertRoute(
		const Key_T& url,
		const EndpointData_T& value,
		const char* realm = def_realm);


	// std::pair<iterator, bool> InsertRoute_hint(
	// 		const Key_T& url,
	// 		const EndpointData_T& value,
	// 		const char* realm /*= def_realm*/,
	// 		const_iterator hint);


	iterator FindRoute(const Key_T& urlPath) /*noexcept*/;


	// iterator FindRoute(const Key_T& urlPath, std::nothrow_t) noexcept;


	iterator FindRouteOrNearestParent(const Key_T& urlPath) /*noexcept*/;


	// iterator FindRouteOrNearestParent(const Key_T& urlPath, std::nothrow_t) noexcept;


	/* extremely hard */
	// template <typename... Args>
	// std::array<std::pair<iterator, bool>, sizeof... (Args)>
	// InsertAIT(Args&&... args);



	template <typename... Args>
	std::array<std::pair<iterator, bool>, sizeof... (Args) / 2>
	InsertSiblings(const Key_T& commonPath, Args&&... args);



	void clear() noexcept; 



	iterator end();



	const_iterator cend();
private:

	template <typename T, std::size_t>
	struct InsertionFusion;


	// possible to move to .cpp
	template <std::size_t N>
	struct InsertionFusion<Key_T, N> 
	{
		constexpr static auto value = metaUtils::is_even<N>::value;	
	};


	template <std::size_t S, std::size_t N>
	struct InsertionFusion<URLChar_T[S], N>
	{
		constexpr static auto value = metaUtils::is_even<N>::value;
	};


	template <std::size_t S, std::size_t N>
	struct InsertionFusion<const URLChar_T (&)[S], N>
	{
		constexpr static auto value = metaUtils::is_even<N>::value;
	};


	template <std::size_t N>
	struct InsertionFusion<metaUtils::ptr<URLChar_T>, N>
	{
		constexpr static auto value = metaUtils::is_even<N>::value;
	};


	template <std::size_t N>
	struct InsertionFusion<EndpointData_T, N>
	{
		constexpr static auto value = !metaUtils::is_even<N>::value;
	};


	static std::pair<Router_T&, bool>
	InsertChild(
		Router_T& router, 
		const Key_T& path,
		const EndpointData_T& data);



	static std::pair<Router_T&, bool>
	InsertLazyDefaultChild(
		Router_T& router,
		const Key_T& path);



	iterator begin();



	const_iterator cbegin();


	template <typename... Args>
	// need expand for all that convertable for Key_T
	Key_T
	FindCommonPath(const Key_T& url, const Args&... urls);
	

	#if defined(UNIT_NATIVE_TEST)
	friend class NativeTestClass;
	#endif // defined(UNIT_NATIVE_TEST) 

private:
	Key_T m_host; // standart allocator
	Key_T m_scheme;
	uint16_t m_port;
// public:
	Router_T m_router;
};

#include "Router.inl"

#endif // _ROUTER_HPP_