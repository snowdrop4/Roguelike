#include "StringUtils.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>

std::string move_up_dir(const std::string &filePath)
{
	std::string path = filePath;
	
	do
	{
		path.erase(path.end() - 1);
	} while (path.back() != '/');
	
	return path;
}

// -----------------------------------------------------------------------------

static std::deque<std::string> split_string_worker(
	const std::function<bool (char c)> &predicate, const std::string &str)
{
	std::deque<std::string> separated(1);
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ')
		{
			auto n = str.find_first_not_of(' ', i);
			if (n == std::string::npos)
				break;
			else if (predicate(str[n]) == true)
				i = n;
		}
		if (predicate(str[i]) == true)
		{
			separated.emplace_back();
			while (str[i + 1] == ' ')
				++i;
		}
		else separated.back() += str[i];
	}
	
	return separated;
}

std::deque<std::string> split_string(char delim, const std::string &str)
{
	return split_string_worker(
		[delim](char c) -> bool
		{ return c == delim; },
		str);
}

std::deque<std::string> split_string(
	const std::function<bool (char c)> &predicate, const std::string &str)
{
	return split_string_worker(predicate, str);
}

// -----------------------------------------------------------------------------

std::string replace_all(const std::string &str, const std::string &match,
	const std::string &replace)
{	
	std::string ret = str;
	size_t pos = 0;
	while ((pos = ret.find(match, pos)) != std::string::npos)
	{
		ret.replace(pos, match.length(), replace);
		pos += replace.length();
	}
	return ret;
}

// -----------------------------------------------------------------------------

std::string to_upper(const std::string &str)
{
	std::string ret = str;
	std::transform(ret.begin(), ret.end(), ret.begin(), toupper);
	return ret;
}

std::string to_lower(const std::string &str)
{
	std::string ret = str;
	std::transform(ret.begin(), ret.end(), ret.begin(), tolower);
	return ret;
}

// -----------------------------------------------------------------------------

template<>
bool from_string<bool>(const std::string &str)
{
	if (str == "true" || str == "yes" || str == "t" || str == "y" || str == "1")
		return true;
	if (str == "false" || str == "no" || str == "f" || str == "n" || str == "0")
		return false;
	
	throw std::runtime_error(LINEANDFILEB + "'" + str + "' could not be "
		"converted to bool");
}

template<>
std::string from_string<std::string>(const std::string &str)
{ return str; }

// -----------------------------------------------------------------------------

std::string uint8_to_hex_str(unsigned int n)
{
	std::stringstream str;
	str << std::setfill('0') << std::setw(2) << std::hex << n;
	return str.str();
}

// -----------------------------------------------------------------------------

std::string int_to_pad_str(int i, unsigned int padding)
{
	auto ret = std::to_string(i);
	
	while (ret.size() < padding)
		ret.insert(ret.begin(), (i >= 0 ? '0' : ' '));
	
	return ret;
}

// -----------------------------------------------------------------------------

bool is_prefix_of(const std::string &prefix, const std::string &of)
{ return prefix == of.substr(0, prefix.length()); }
