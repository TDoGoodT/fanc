#ifndef TOKENS_HPP_
#define TOKENS_HPP_

#include <cstdlib>
#include <string>

enum error_type {
	INVALID_STRING = -1,
	UNKOWN_TOKEN = -2,
	UNCLOSED_STRING = -3
};


enum Types {
	VOID_T,
	INT_T,
	BYTE_T,
	BOOL_T,
	STRING_T,
    NOT_SET
};

static bool types_match(Types from, Types to, bool explicit_cast = false) {
	return (from == to) || (to == Types::INT_T && from == Types::BYTE_T) || (explicit_cast && from == Types::INT_T && to == Types::BYTE_T);
}

#endif /* TOKENS_HPP_ */
