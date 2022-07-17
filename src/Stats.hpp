#pragma once

////////////////////////////////////////////////////////////////////////////////
// if you add any stats to a struct with a to_string() method, remember to
// update the code in those methods!
////////////////////////////////////////////////////////////////////////////////

#include <deque>
#include <string>
#include "UtilDefines.hpp"

class Text;

namespace Stats
{

struct DynamicStat
{
	int cur, max;
	
	int to_percentage() const;
	
	std::string to_string() const;
	
	// Returns coloured text on a red → green spectrum corresponding to the
	// ratio of 'cur' to 'max'.
	// 
	// If 'swap' is set to true, it will be on a green → red spectrum.
	Text to_text(bool swap = false) const;
};

struct Basic
{
	Basic(
		DynamicStat hp,
		DynamicStat mp,
		DynamicStat wt,
		int strength,
		int intelligence,
		int dexterity,
		int constitution,
		int initiative);
	virtual ~Basic();
	
	/// Lists each stat, along with its name, each on a seperate line.
	// 
	/// Includes blank lines between unrelated stats for readability.
	virtual std::string to_string() const;
	
	DynamicStat hp;
	DynamicStat mp;
	DynamicStat wt;
	
	int strength;     /// modifier for melee attacks  / carry weight / 
	int intelligence; /// modifier for magic attacks  / 
	int dexterity;    /// modifier for ranged attacks / chance to avoid melee attacks
	int constitution; /// negates damage
	int initiative;   /// priority Character has when making its turn
};

struct Character : public Basic
{
	Character(
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
		int initiative);
	virtual ~Character();
	
	virtual std::string to_string() const override;
	
	std::string name;
	std::string faction;
	
	int xp;
	int level;
};

struct Item : public Basic
{
	Item(
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
		int effectLength);
	virtual ~Item();
	
	virtual std::string to_string() const override;
	
	std::string name;
	std::string description;
	std::string useWord;
	
	// -1 == unlimited
	int uses;
	int effectLength;
};

struct Abilities
{
	Abilities(
		int fencing,
		int archery,
		int sorcery);
	
	std::string to_string() const;
	
	int fencing; /// chance to hit with a melee attack
	int archery; /// chance to hit with a ranged attack
	int sorcery; /// chance to succesfully cast a spell
};

} // namespace Stats
