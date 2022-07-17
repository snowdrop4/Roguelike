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

#include "Item.hpp"

#include "constants.hpp"
#include "ConfigFile.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Item::Item(const std::string &filePath) :
	texture(),
	statModifier(),
	filePath(filePath)
{
	ConfigFile file(filePath);
	
	// -------------------------------------------------------------------------
	// info
	// -------------------------------------------------------------------------
	std::string name        = file.parse<std::string>("name");
	std::string description = file.parse<std::string>("description");
	std::string useWord     = file.parse<std::string>("useWord");
	
	// -------------------------------------------------------------------------
	// texture
	// -------------------------------------------------------------------------
	// randomly select texture from list of possible textures:
	auto textureNames = file.parse_array<std::string>("texture");
	
	if (textureNames.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "no textures set in file '" + filePath + "'");
	
	auto textureName = textureNames[rand() % textureNames.size()] + ".png";
	
	texture.load_color_key(move_up_dir(filePath) + textureName, Colors::textureColorKey);
	
	// -------------------------------------------------------------------------
	// stats
	// -------------------------------------------------------------------------
	statModifier.reset(new Stats::Item(
	      name,
	      description,
	      useWord,
		{ file.parse<int>("healthCur"), file.parse<int>("healthMax") },
		{ file.parse<int>("manaCur"),   file.parse<int>("manaMax")   },
		{ file.parse<int>("weightCur"), file.parse<int>("weightMax") },
		  file.parse<int>("strength"),
		  file.parse<int>("intelligence"),
		  file.parse<int>("dexterity"),
		  file.parse<int>("constitution"),
		  file.parse<int>("initiative"),
		  file.parse<int>("uses"),
		  file.parse<int>("effectLength")));
}

Item::~Item() { }

void Item::draw(const Window &window, int x, int y) const
{ texture.draw(window, x, y); }

void Item::draw_literal(const Window &window, int x, int y) const
{ texture.draw_literal(window, x, y); }

const Stats::Item &Item::get_stats() const
{ return *statModifier; }
