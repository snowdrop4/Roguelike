#include "ConfigFile.hpp"

#include <fstream>

// -----------------------------------------------------------------------------
// public
// -----------------------------------------------------------------------------
ConfigFile::ConfigFile(const std::string &filePath) :
	words(),
	filePath()
{
	size_t lineNum = 1;
	std::ifstream file(filePath);
	
	if (file.is_open() == false)
		throw std::runtime_error(LINEANDFILEB + "could not open file '" +
			filePath + "'");
	
	std::string lineString;
	std::stringstream lineStream;
	
	while (getline(file, lineString))
	{
		// sets the internal string, but not the error state nor the internal
		lineStream.str(lineString);
		lineStream.seekg(0); // fix the position pointer
		lineStream.clear();  // clear error and eof flags
		
		std::string inString;
		while (lineStream >> inString)
		{
			if (inString == "//") break;
			
			// if the string we just read in is specified as a string literal:
			if (inString[0] == '"')
			{
				// remove the " char from front
				inString = inString.substr(1);
				
				// if a string literal is specified with no whitespace, it is
				// read incorrectly by the stream
				if (inString.back() == '"')
					inString = inString.substr(0, inString.size() - 1);
				else while (1)
				{
					if (lineStream.eof() == true)
						throw std::runtime_error(LINEANDFILEB + "encountered "
							" unterminated string literal on line " +
							std::to_string(lineNum));
					
					// interpret escaped " (aka. \") characters
					if (lineStream.peek() == '"' && inString.back() == '\\')
					{
						inString.back() = '"';
						lineStream.get();
					}
					
					char nextChar = lineStream.get();
					if (nextChar == '"') break;
					inString += nextChar;
				}
			}
			
			words.emplace_back(inString, lineNum);
		}
		
		++lineNum;
	}
}

ConfigFile::~ConfigFile() { }

const std::deque<Word> &ConfigFile::get_words() const
{ return words; }

// -----------------------------------------------------------------------------
// private
// -----------------------------------------------------------------------------
size_t ConfigFile::pop_var_name(const std::string &varName)
{
	// check EOF
	if (words.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "encountered EOF when trying to "
			"read variable '" + varName + "'");
	
	size_t line = words[0].line;
	// check correct variable name
	if (words[0].word != varName)
		throw std::runtime_error(LINEANDFILEB + "encountered variable '" + words[0].word +
			"' on line " + std::to_string(line) + "but expected variable '" +
			varName + "'");
	
	words.pop_front();
	return line;
}

void ConfigFile::pop_operator(const std::string &varName,
	const std::string &operatorA, size_t line)
{
	// check EOF
	if (words.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "encounterd EOF when trying to "
		"read operator '" + operatorA + "' of variable '" + varName + "'");
	
	// check (possible) presence
	if (words[0].line != line)
		throw std::runtime_error(LINEANDFILEB + "encountered variable '" +
			varName + "' on line " + std::to_string(line) + " with missing "
			"operator '" + operatorA + "'");
	
	// check correct assignment operator
	if (words[0].word != operatorA)
		throw std::runtime_error(LINEANDFILEB + "encountered '" + words[0].word +
			"' on line " + std::to_string(line) + "but expected operator '" +
			operatorA + "'");
	
	words.pop_front();
}

void ConfigFile::check_single_value(const std::string &varName, size_t line)
{
	// check EOF
	if (words.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "encountered EOF when trying to "
		"read value of variable '" + varName + "'");
	
	// check presence
	if (words[0].line != line)
		throw std::runtime_error(LINEANDFILEB + "encountered variable '" +
			varName + "' on line " + std::to_string(line) + " with a missing "
			"value after assignment operator");
	
	// check presence of other values on same line
	if (words.size() > 1 && words[1].line == line)
		throw std::runtime_error(LINEANDFILEB + "encountered non-array "
			"variable '" + varName + "' on line " + std::to_string(line) +
			" with multiple values after assignment operator");
}
