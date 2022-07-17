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

#define COPY_CTOR(arg) arg (const arg &)
#define MOVE_CTOR(arg) arg (arg &&)

#define COPY_ASSIGN(arg) arg &operator = (const arg &)
#define MOVE_ASSIGN(arg) arg &operator = (arg &&)

#define LINEANDFILEA __FILE__ << ":" << __LINE__ << " In function " << __PRETTY_FUNCTION__ << ": "
#define LINEANDFILEB std::string(__FILE__) + ":" + std::to_string(__LINE__) + " In function " + __PRETTY_FUNCTION__ + ": "

typedef unsigned char Uint8;

