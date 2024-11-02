#include "Router.hpp"


const std::regex UrlUtils::urlPathRegex("/?([^/#?]+/)*");

std::vector<std::string>
	UrlUtils::Split(std::string_view spliting_string)
{
	std::vector<std::string> splited_url;
	size_t rd_pos = 0;
    size_t curr_len = 0;

	for (size_t i = 0; i < spliting_string.length(); ++i)
	{
		if (spliting_string[i] == '/')
		{
			splited_url.emplace_back(spliting_string.data() + rd_pos, curr_len);
			rd_pos = i + 1;
			curr_len = 0;
		} else 
		{
		    ++curr_len;
		}
	}

    if (rd_pos != spliting_string.length())
    {
        splited_url.emplace_back(spliting_string.data() + rd_pos, curr_len);
    };

	return splited_url; // RNVO
}

/**
 * @param stringString presents 
 * url path by standart without ending '/' 
 * */
std::vector<std::string>
	UrlUtils::SplitString(const std::string& splitingString)
{
    constexpr char delimiter = '/'; 
	auto numTokens = std::count(splitingString.cbegin(),
		splitingString.cend(), '/') + 1;

	std::vector<std::string> tokens;
    tokens.reserve(numTokens);
  
    size_t startPos = 0ul
    		, endPos = 0ul;

    while ((endPos = splitingString.find(delimiter, startPos)) !=
    			std::string::npos)
    {
    	tokens.emplace_back(splitingString.data(),
    	    startPos, endPos - startPos);
    	startPos = endPos + 1;
    }

    tokens.emplace_back(splitingString.data(),
        startPos, splitingString.length());
    return tokens;
}



bool UrlUtils::CheckUrlCorrectness(
	std::string_view urlRawPath)
{
	std::cmatch ignoring;
	return std::regex_match(urlRawPath.data(), urlRawPath.data() + 
		urlRawPath.length(), ignoring, urlPathRegex);
};



bool UrlUtils::CheckUrlCorrectness(
	const std::string& urlRawPath)
{
	return UrlUtils::CheckUrlCorrectness(
		std::string_view{urlRawPath.c_str()});
};


bool UrlUtils::CheckUrlCorrectness(
	const char* urlRawPath)
{
	return CheckUrlCorrectness(std::string_view{urlRawPath});
};


void UrlUtils::EraseQueryParams(
	std::string& url) noexcept
{
	std::size_t query_pm_spec_symbol_pos = url.find('?');

	if (query_pm_spec_symbol_pos != std::string::npos)
	{
		url.erase(query_pm_spec_symbol_pos);
	}
}