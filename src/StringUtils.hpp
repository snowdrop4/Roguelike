#pragma once

#include <deque>
#include <functional>
#include <stdexcept>
#include "UtilDefines.hpp"

// 
// Takes a file path or a directory path so that:
//
// ../example/file.foo -> ../example/
// ../example/dir/     -> ../example/
// 
std::string move_up_dir(const std::string &);

// -----------------------------------------------------------------------------

std::deque<std::string> split_string(char delim, const std::string &str);

std::deque<std::string> split_string(
	const std::function<bool (char c)> &predicate, const std::string &str);

// -----------------------------------------------------------------------------

std::string replace_all(const std::string &str, const std::string &match,
	const std::string &replace);

// -----------------------------------------------------------------------------

/// converts the case of all the characters in the string
std::string to_upper(const std::string &);
std::string to_lower(const std::string &);

// -----------------------------------------------------------------------------

// 
// Bools can be converted from: 
//
// "true",  "yes", "t", "y", "1"
// "false", "no",  "f", "n", "0"
// 
// Strings are returned as-is. All other types are tried to pass through
// a stringstream for conversion.
// 
template<typename T>
T from_string(const std::string &str)
{
	std::stringstream converter(str);
	T value;
	
	if (!(converter >> value))
		throw std::runtime_error(LINEANDFILEB + "'" + str + "' could not be "
		"converted to specified type");
	
	return value;
}

template<>
bool from_string<bool>(const std::string &);

template<>
std::string from_string<std::string>(const std::string &);

// -----------------------------------------------------------------------------

std::string uint8_to_hex_str(unsigned int n);

// -----------------------------------------------------------------------------

std::string int_to_pad_str(int i, unsigned int padding);

// -----------------------------------------------------------------------------

bool is_prefix_of(const std::string &prefix, const std::string &of);
