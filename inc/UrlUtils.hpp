#ifndef _URLUTILS_HPP_
#define _URLUTILS_HPP_

#include <vector>
#include <string>
#include <string_view>
#include <regex>

template <typename Char_T,
	typename Traits_T = std::char_traits<Char_T>,
	typename Alloc_T = std::allocator<Char_T>
>
class UrlUtils
{
public:
	static std::vector<std::basic_string<Char_T, Traits_T, Alloc_T>>
		SplitUrlPath(
			std::basic_string_view<Char_T, Traits_T> splitingPath);


	static std::vector<std::basic_string<Char_T, Traits_T, Alloc_T>>
		SplitUrlPath(
			const std::basic_string<Char_T, Traits_T, Alloc_T>& splitingPath);


	// static void
	// 	CheckUrlPathCorrectness(std::nothrow_t, 
	// 		std::basic_string_view<Char_T, Traits_T> url) noexcept;


	static bool
		CheckUrlPathCorrectness(
			std::basic_string_view<Char_T, Traits_T> urlPath);


	static bool
		CheckUrlPathCorrectness(
			const std::basic_string<Char_T, Traits_T, Alloc_T>& urlPath);


	static bool
		CheckUrlPathCorrectness(
			const Char_T* urlPath);

	// static void 
	// 	EraseQueryParams(std::basic_string<Char_T, Traits_T, Alloc_T>& url) noexcept;


private:
	static std::basic_regex<Char_T, Traits_T> m_urlPathRegex;
};


template <typename Char_T>
constexpr const Char_T* GetUrlPathRegex() noexcept;


#include "UrlUtils.inl"

#endif // _URLUTILS_HPP_