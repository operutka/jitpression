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

#ifndef TOKENIZER_H
#define	TOKENIZER_H

#include "token.h"

#define DEFAULT_TEMP_STRING_SIZE        256

class tokenizer {
public:
    tokenizer(const char* input);
    tokenizer(const char* input, size_t offset, size_t length);
    virtual ~tokenizer();
    bool has_next() const;
    const token next();
private:
    const char* input;
    size_t input_offset;
    size_t input_length;
    
    token current_token;
    char *temp_string;
    size_t temp_string_size;
    
    void init(const char* input, size_t offset, size_t length);
    
    void next_number();
    void next_octal_number();
    void next_hex_number();
    void next_decimal_number();
    void next_symbol(int symbol, int symbol_width);
    void next_identifier();
    
    bool is_symbol(char c) const;
    bool is_white_space(char c) const;
    
    void skip_white_space();
    
    char* substring(const char* string, size_t start, size_t length);
};

#endif	/* TOKENIZER_H */

