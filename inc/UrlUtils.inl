template <>
const char* GetUrlPathRegex() noexcept
{
	return "[^/#?]+(/[^/#?]+)*";
};


#if !(__cplusplus < 202002L)
template <>
const char8_t* GetUrlPathRegex() noexcept
{
	return u8"[^/#?]+(/[^/#?]+)*";
}
#endif // !__cplusplus > 202002L


template <>
const char16_t* GetUrlPathRegex() noexcept
{
	return u"[^/#?]+(/[^/#?]+)*";
}


template <>
const char32_t* GetUrlPathRegex() noexcept
{
	return U"[^/#?]+(/[^/#?]+)*";
}


template <>
const wchar_t* GetUrlPathRegex() noexcept
{
	return L"[^/#?]+(/[^/#?]+)*";
};


template <
	typename Char_T, 
	typename Traits_T,
	typename Alloc_T
>
std::basic_regex<Char_T, std::regex_traits<Char_T>> UrlUtils<Char_T, Traits_T, Alloc_T>::m_urlPathRegex(
	GetUrlPathRegex<Char_T>());


template <
	typename Char_T, 
	typename Traits_T, 
	typename Alloc_T
>
std::vector<std::basic_string<Char_T, Traits_T, Alloc_T>>
	UrlUtils<Char_T, Traits_T, Alloc_T>::SplitUrlPath(
		std::basic_string_view<Char_T, Traits_T> splitingPath)
{
	std::vector<std::basic_string<Char_T, Traits_T, Alloc_T>> splited_url;
	size_t rd_pos = 0;
    size_t curr_len = 0;

	for (size_t i = 0; i < splitingPath.length(); ++i)
	{
		if (splitingPath[i] == static_cast<Char_T>('/')) // is it safe?
		{
			splited_url.emplace_back(splitingPath.data() + rd_pos, curr_len);
			rd_pos = i + 1;
			curr_len = 0;
		} else 
		{
		    ++curr_len;
		}
	}

    if (rd_pos != splitingPath.length())
    {
        splited_url.emplace_back(splitingPath.data() + rd_pos, curr_len);
    };

	return splited_url; // RNVO
};

/**
 * @param stringString presents 
 * url path by standart without ending '/'
 * */
template <
	typename Char_T, 
	typename Traits_T, 
	typename Alloc_T
>
std::vector<std::basic_string<Char_T, Traits_T, Alloc_T>>
	UrlUtils<Char_T, Traits_T, Alloc_T>::SplitUrlPath(
		const std::basic_string<Char_T, Traits_T, Alloc_T>& splitingPath)
{
    constexpr Char_T delimiter = static_cast<Char_T>('/');
	auto numTokens = std::count(splitingPath.cbegin(),
		splitingPath.cend(), '/') + 1;

	std::vector<std::string> tokens;
    tokens.reserve(numTokens);
  
    size_t startPos = 0ul
    		, endPos = 0ul;

    while ((endPos = splitingPath.find(delimiter, startPos)) !=
    			std::string::npos)
    {
    	tokens.emplace_back(splitingPath.data(),
    	    startPos, endPos - startPos);
    	startPos = endPos + 1;
    }

    
	tokens.emplace_back(splitingPath.data(),
    	startPos, splitingPath.length());

    return tokens;
}


template <
	typename Char_T, 
	typename Traits_T, 
	typename Alloc_T
>
bool UrlUtils<Char_T, Traits_T, Alloc_T>::CheckUrlPathCorrectness(
	std::basic_string_view<Char_T, Traits_T> urlPath)
{
	std::match_results<const Char_T*> ignoring;
	return std::regex_match(urlPath.data(), urlPath.data() + 
		urlPath.length(), ignoring, m_urlPathRegex);
};



template <
	typename Char_T, 
	typename Traits_T, 
	typename Alloc_T
>
bool UrlUtils<Char_T, Traits_T, Alloc_T>::CheckUrlPathCorrectness(
	const std::basic_string<Char_T, Traits_T, Alloc_T>& urlPath)
{
	return UrlUtils::CheckUrlPathCorrectness(
		std::basic_string_view<Char_T, Traits_T>{urlPath.c_str()});
};


template <
	typename Char_T, 
	typename Traits_T, 
	typename Alloc_T
>
bool UrlUtils<Char_T, Traits_T, Alloc_T>::CheckUrlPathCorrectness(
	const Char_T* urlPath)
{
	std::basic_string_view urlPathViewWrapper(urlPath);
	return UrlUtils::CheckUrlPathCorrectness(
		urlPathViewWrapper);
};

// template <typename Char_T, 
// 	typename Traits_T,
// 	typename Alloc_T>
// void UrlUtils::EraseQueryParams(
// 	std::basic_string<Char_T, Traits_T, Alloc_T>& url) noexcept
// {
// 	std::size_t query_pm_spec_symbol_pos = url.find(static_cast<Char_T>('?'));

// 	if (query_pm_spec_symbol_pos != std::string::npos)
// 	{
// 		url.erase(query_pm_spec_symbol_pos);
// 	}
// }