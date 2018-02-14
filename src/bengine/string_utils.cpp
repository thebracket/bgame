#include "string_utils.hpp"
#include <sstream>
#include <cctype>

std::vector<std::string> split ( const std::string &str, const char &delimiter )
{
     std::vector<std::string> internal;
     std::stringstream ss ( str ); // Turn the string into a stream.
     std::string tok;

     while ( getline ( ss, tok, delimiter ) ) {
          internal.push_back ( tok );
     }

     return internal;
}

std::string to_proper_noun_case(const std::string &original)
{
    std::string result;
    result += std::toupper(original[0]);
    for (std::size_t i = 1; i < original.size(); ++i)
    {
        result += std::tolower(original[i]);
    }
    return result;
}

std::string max_width_str(const std::string &original, const int &width) {
	if (original.size() <= width) return original;
	return original.substr(0,width);
}

std::string str_replace(std::string &s, const std::string &toReplace, const std::string &replaceWith)
{
    return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}

bool str_contains(const std::string &s, const std::string &search_for) {
    const auto finder = s.find(search_for);
	if (finder == std::string::npos) {
		return false;
	}
	else {
		return true;
	}
}