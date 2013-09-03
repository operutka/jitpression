/*
 * Copyright (C) 2013 Ondrej Perutka
 * 
 * This file is part of Jitpression.
 * 
 * Jitpression is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Jitpression is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Jitpression.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOKEN_H
#define	TOKEN_H

#include <cstdlib>

#define TOKEN_TYPE_SYM_PLUS     0
#define TOKEN_TYPE_SYM_MINUS    1
#define TOKEN_TYPE_SYM_STAR     2
#define TOKEN_TYPE_SYM_SLASH    3
#define TOKEN_TYPE_SYM_POW      4
#define TOKEN_TYPE_SYM_LBR      5
#define TOKEN_TYPE_SYM_RBR      6
#define TOKEN_TYPE_SYM_EQUAL    7
#define TOKEN_TYPE_SYM_COMMA    8
#define TOKEN_TYPE_KW_DEF       9
#define TOKEN_TYPE_NUMBER       10
#define TOKEN_TYPE_IDENTIFIER   11
#define TOKEN_TYPE_EPS          12

namespace jitpression {

    class token {
    public:
        token(const char* text, int type, size_t offset, int value);
        token(const char* text, size_t offset);
        token(int offset);
        token(const token& orig);
        token();

        const char* get_text() const;
        int get_type() const;
        size_t get_offset() const;
        int get_value() const;
    private:
        const char* text;
        int type;
        size_t offset;
        int value;

        void init(const char* text, int type, size_t offset, int value);
    };

}

#endif	/* TOKEN_H */

