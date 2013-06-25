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

#define TOKEN_TYPE_NUMBER       0
#define TOKEN_TYPE_SYMBOL       1
#define TOKEN_TYPE_IDENTIFIER   2
#define TOKEN_TYPE_EPS          3

#define SYMBOL_PLUS             0
#define SYMBOL_MINUS            1
#define SYMBOL_STAR             2
#define SYMBOL_SLASH            3
#define SYMBOL_LBR              4
#define SYMBOL_RBR              5

class token {
public:
    token(const char* text, int type, size_t offset, int symbol, int value);
    token(const char* text, size_t offset);
    token(int offset);
    token(const token& orig);
    token();
    
    const char* get_text() const;
    int get_type() const;
    size_t get_offset() const;
    int get_symbol() const;
    int get_value() const;
private:
    const char* text;
    int type;
    size_t offset;
    int symbol;
    int value;
    
    void init(const char* text, int type, size_t offset, int symbol, int value);
};

#endif	/* TOKEN_H */

