#include "Stats.hpp"

#include "constants.hpp"
#include "Text.hpp"
#include "StringUtils.hpp"

//------------------------------------------------------------------------------
// DynamicStat
// -----------------------------------------------------------------------------
int Stats::DynamicStat::to_percentage() const
{ return (cur / max) * 100; }

std::string Stats::DynamicStat::to_string() const
{
	return int_to_pad_str(cur, 3) + "/" + int_to_pad_str(max, 3);
}

Text Stats::DynamicStat::to_text(bool swap) const
{
	double percent = (double)cur / (double)max * 100;
	SDL_Colour color =
	{
		Uint8((percent < 50) ? 255 : 256 - (percent - 50) * 5),
		Uint8((percent > 50) ? 255 : percent * 5.1),
		0
	};
	
	if (swap) std::swap(color.r, color.g);
	
	return Text(int_to_pad_str(cur, 3) + "/" + int_to_pad_str(max, 3), color);
}

//------------------------------------------------------------------------------
// Basic
// -----------------------------------------------------------------------------
Stats::Basic::Basic(
		DynamicStat hp,
		DynamicStat mp,
		DynamicStat wt,
		int strength,
		int intelligence,
		int dexterity,
		int constitution,
		int initiative) :
	hp(hp),
	mp(mp),
	wt(wt),
	strength(strength),
	intelligence(intelligence),
	dexterity(dexterity),
	constitution(constitution),
	initiative(initiative)
{

}

Stats::Basic::~Basic() { }

std::string Stats::Basic::to_string() const
{
	return "hp cur   - "   + int_to_pad_str(hp.cur, 3) + "\n" +
	       "hp max   - "   + int_to_pad_str(hp.max, 3) + "\n" +
	       "\n" +
	       "mp cur     - "   + int_to_pad_str(mp.cur, 3)   + "\n" +
	       "mp max     - "   + int_to_pad_str(mp.max, 3)   + "\n" +
	       "\n" +
	       "wt cur   - "   + int_to_pad_str(wt.cur, 3) + "\n" +
	       "wt max   - "   + int_to_pad_str(wt.max, 3) + "\n" +
	       "\n" +
	       "Strength     - " + int_to_pad_str(strength,     3) + "\n" +
	       "Intelligence - " + int_to_pad_str(intelligence, 3) + "\n" +
	       "Dexterity    - " + int_to_pad_str(dexterity,    3) + "\n" +
	       "Constitution - " + int_to_pad_str(constitution, 3) + "\n" +
	       "Initiative   - " + int_to_pad_str(initiative,   3);
}

//------------------------------------------------------------------------------
// Character
// -----------------------------------------------------------------------------
Stats::Character::Character(
		const std::string &name,
		const std::string &faction,
		int xp,
		int level,
		DynamicStat hp,
		DynamicStat mp,
		DynamicStat wt,
		int strength,
		int intelligence,
		int dexterity,
		int constitution,
		int initiative) :
	Basic(
		hp,
		mp,
		wt,
		strength,
		intelligence,
		dexterity,
		constitution,
		initiative),
	name(name),
	faction(faction),
	xp(xp),
	level(level)
{
	
}

Stats::Character::~Character() { }

std::string Stats::Character::to_string() const
{
	return "Name       - " + name    + "\n" +
	       "Faction    - " + faction + "\n" +
	       "\n" +
	       "XP         - " + std::to_string(xp)    + "\n" +
	       "Level      - " + std::to_string(level) + "\n" +
	       "\n" +
           Basic::to_string();
}

//------------------------------------------------------------------------------
// Item
// -----------------------------------------------------------------------------
Stats::Item::Item(
		const std::string &name,
		const std::string &description,
		const std::string &useWord,
		DynamicStat hp,
		DynamicStat mp,
		DynamicStat wt,
		int strength,
		int intelligence,
		int dexterity,
		int constitution,
		int initiative,
		int uses,
		int effectLength) :
	Basic(
		hp,
		mp,
		wt,
		strength,
		intelligence,
		dexterity,
		constitution,
		initiative),
	name(name),
	description(description),
	useWord(useWord),
	uses(uses),
	effectLength(effectLength)
{
	
}

Stats::Item::~Item() { }

std::string Stats::Item::to_string() const
{
	std::string usesStr = (uses == -1)         ? "unlimited" : std::to_string(uses + 1);
	std::string effeStr = (effectLength == -1) ? "unlimited" : std::to_string(effectLength + 1);
	
	return "Name        - " + name        + "\n" +
	       "Description - " + description + "\n" +
	       "\n" +
	       Basic::to_string() + "\n" +
	       "\n" +
	       "Uses          - " + usesStr + "\n" +
	       "Effect length - " + effeStr;
}

//------------------------------------------------------------------------------
// Abilities
// -----------------------------------------------------------------------------
Stats::Abilities::Abilities(
		int fencing,
		int archery,
		int sorcery) :
	fencing(fencing),
	archery(archery),
	sorcery(sorcery)
{
	
}

std::string Stats::Abilities::to_string() const
{
	return "Fencing - " + int_to_pad_str(fencing, 3) + "\n" +
	       "Archery - " + int_to_pad_str(archery, 3) + "\n" +
	       "Sorcery - " + int_to_pad_str(sorcery, 3);
}
