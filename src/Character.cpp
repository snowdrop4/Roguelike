#include "Character.hpp"

#include "constants.hpp"
#include "Item.hpp"
#include "Map.hpp"
#include "MapTile.hpp"
#include "ConfigFile.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Character::Character(const TileCoord &position, const std::string &filePath,
                     const Map &map) :
	map(map),
	texture(),
	position(position),
	stats(),
	abilities(),
	inventory()
{
	ConfigFile file(filePath);
	
	// -------------------------------------------------------------------------
	// info
	// -------------------------------------------------------------------------
	std::string name    = file.parse<std::string>("name");
	std::string faction = file.parse<std::string>("faction");
	
	// -------------------------------------------------------------------------
	// texture
	// -------------------------------------------------------------------------
	// randomly select texture from list of possible textures:
	auto textureNames = file.parse_array<std::string>("texture");
	
	if (textureNames.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "in file '" + filePath + "': no textures set");
	
	auto textureName = textureNames[rand() % textureNames.size()] + ".png";
	
	texture.load_color_key(move_up_dir(filePath) + textureName, Colors::textureColorKey);
	
	// -------------------------------------------------------------------------
	// stats
	// -------------------------------------------------------------------------
	stats.reset(new Stats::Character(
		  name,
		  faction,
		  file.parse<int>("xp"),
		  file.parse<int>("level"),
		{ file.parse<int>("healthCur"), file.parse<int>("healthMax") },
		{ file.parse<int>("manaCur"),   file.parse<int>("manaMax")   },
		{ file.parse<int>("weightCur"), file.parse<int>("weightMax") },
		  file.parse<int>("strength"),
		  file.parse<int>("intelligence"),
		  file.parse<int>("dexterity"),
		  file.parse<int>("constitution"),
		  file.parse<int>("initiative")));
	
	// -------------------------------------------------------------------------
	// abilities
	// -------------------------------------------------------------------------
	abilities.reset(new Stats::Abilities(
		file.parse<int>("fencing"),
		file.parse<int>("sorcery"),
		file.parse<int>("archery")));
	
	file.parse_array<std::string>("spells");
	
	// -------------------------------------------------------------------------
	// inventory
	// -------------------------------------------------------------------------
	// items:
	auto itemNames = file.parse_array<std::string>("items");
	for (auto &i : itemNames)
		inventory.emplace_back(new Item(Defaults::itemPath + i));
	
	// equipped items:
	file.parse<std::string>("head");
	file.parse<std::string>("torso");
	file.parse<std::string>("gauntlets");
	file.parse<std::string>("greaves");
	file.parse<std::string>("boots");
	
	file.parse<std::string>("ring");
	file.parse<std::string>("necklace");
	
	file.parse<std::string>("weapon");
	file.parse<std::string>("shield");
}

Character::~Character() { }

void Character::draw(const Window &window) const
{ texture.draw(window, position.x * TILE_WIDTH, position.y * TILE_HEIGHT); }

MapTile &Character::get_map_tile() const
{ return map.at(position); }

const TileCoord &Character::get_xy() const
{ return position; }

const Stats::Character &Character::get_stats() const
{ return *stats; }

const Stats::Abilities &Character::get_abilities() const
{ return *abilities; }

const std::list<std::unique_ptr<Item>> &Character::get_inventory() const
{ return inventory; }

bool Character::is_alive() const
{ return stats->hp.cur > 0; }

bool Character::is_player() const
{ return false; }

int Character::be_melee_attacked(const Character &attacker)
{
	int attackerFencing = attacker.get_abilities().fencing;
	if (rand() % attackerFencing > stats->dexterity / 2)
	{
		int damage = attacker.get_stats().strength +
			rand() % (int)sqrt(attackerFencing) + (attackerFencing / 2);
		stats->hp.cur -= damage;
		
		if (is_alive() == false)
			handle_death();
		
		return damage;
	}
	
	return 0;
}

// -----------------------------------------------------------------------------
// Protected
// -----------------------------------------------------------------------------
void Character::move_to_tile(const TileCoord &newPos)
{
	map.at(position).move_character(map.at(newPos));
	position = newPos;
}

void Character::handle_death()
{
	map.at(position).set_character(nullptr);
	
	//map.at(position).set_decal(genericTileset->get_blood());
	
	for (auto &i : inventory)
		map.at(position).add_item(i);
	
	inventory.clear();
}

// -----------------------------------------------------------------------------
// Static protected
// -----------------------------------------------------------------------------
bool Character::astar_valid_tile(const MapTile &tile, int xDiff, int yDiff)
{ return tile.is_walkable(); }
