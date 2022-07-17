#pragma once

#define COPY_CTOR(arg) arg (const arg &)
#define MOVE_CTOR(arg) arg (arg &&)

#define COPY_ASSIGN(arg) arg &operator = (const arg &)
#define MOVE_ASSIGN(arg) arg &operator = (arg &&)

#define LINEANDFILEA __FILE__ << ":" << __LINE__ << " In function " << __PRETTY_FUNCTION__ << ": "
#define LINEANDFILEB std::string(__FILE__) + ":" + std::to_string(__LINE__) + " In function " + __PRETTY_FUNCTION__ + ": "

typedef unsigned char Uint8;

