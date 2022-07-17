#pragma once

#include <memory>
#include "UtilDefines.hpp"
#include "Texture.hpp"
#include "Stats.hpp"

enum class EquipTypes
{
	none,
	head,
	torso,
	gauntlets,
	greaves,
	boots
};

class Window;

class Item
{
	public:
		Item(const std::string &filePath);
		COPY_CTOR(Item) = default;
		MOVE_CTOR(Item) = delete;
		COPY_ASSIGN(Item) = default;
		MOVE_ASSIGN(Item) = delete;
		virtual ~Item();
		
		void draw        (const Window &window, int, int) const;
		void draw_literal(const Window &window, int, int) const;
		
		/// Returns an appropriate label to use for the use command, such
		/// as "eat" in the case of some food, or "cast" in the case of a wand.
		const std::string &get_use_word() const;
		
		const Stats::Item  &get_stats() const;
		const std::string  &get_name() const;
		const std::string  &get_description() const;
		std::string get_full_description() const;
		
	protected:
		Texture texture;
		std::unique_ptr<Stats::Item> statModifier;
		
		std::string filePath;
};
