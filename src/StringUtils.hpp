#pragma once

/*
 * Copyright 2012 Alyxr
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#include <deque>
#include <functional>
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
