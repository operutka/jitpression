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

#include <cstring>
#include <cstdlib>

#include "tokenizer.h"

using namespace jitpression;

tokenizer::tokenizer(const char* input) {
    init(input, 0, strlen(input));
}

tokenizer::tokenizer(const char* input, size_t offset, size_t length) {
    init(input, offset, length);
}

void tokenizer::init(const char* input, size_t offset, size_t length) {
    this->input = input;
    this->input_offset = offset;
    this->input_length = length;
    this->temp_string_size = DEFAULT_TEMP_STRING_SIZE;
    this->temp_string = new char[DEFAULT_TEMP_STRING_SIZE];
    
    if (!temp_string)
        throw "out of memory";  // TODO: fix this
}

tokenizer::~tokenizer() {
    delete [] temp_string;
}

bool tokenizer::has_next() const {
    return input_offset < input_length;
}

const token tokenizer::next() {
    if (!has_next())
        current_token = token(input_offset);
    else if (input[input_offset] >= '0' && input[input_offset] <= '9')
        next_number();
    else if (input[input_offset] == '+')
        next_symbol(TOKEN_TYPE_SYM_PLUS, 1);
    else if (input[input_offset] == '-')
        next_symbol(TOKEN_TYPE_SYM_MINUS, 1);
    else if (input[input_offset] == '*')
        next_symbol(TOKEN_TYPE_SYM_STAR, 1);
    else if (input[input_offset] == '/')
        next_symbol(TOKEN_TYPE_SYM_SLASH, 1);
    else if (input[input_offset] == '^')
        next_symbol(TOKEN_TYPE_SYM_POW, 1);
    else if (input[input_offset] == '(')
        next_symbol(TOKEN_TYPE_SYM_LBR, 1);
    else if (input[input_offset] == ')')
        next_symbol(TOKEN_TYPE_SYM_RBR, 1);
    else if (input[input_offset] == '=')
        next_symbol(TOKEN_TYPE_SYM_EQUAL, 1);
    else if (input[input_offset] == ',')
        next_symbol(TOKEN_TYPE_SYM_COMMA, 1);
    else
        next_identifier();

    skip_white_space();

    return current_token;
}

void tokenizer::next_number() {
    if (input[input_offset] == '0' && ((input_offset + 1) < input_length) && input[input_offset + 1] == 'x')
        next_hex_number();
    else if (input[input_offset] == '0')
        next_octal_number();
    else
        next_decimal_number();
}

void tokenizer::next_octal_number() {
    size_t i = input_offset;
    int value = 0;

    while (i < input_length && !is_white_space(input[i]) && !is_symbol(input[i])) {
        if (input[i] < '0' || input[i] > '7')
            throw "currupted octal format";

        value = value * 8 + input[i++] - '0';
    }

    char* tmp = substring(input, input_offset, i - input_offset);
    current_token = token(tmp, TOKEN_TYPE_NUMBER, input_offset, value);
    input_offset = i;
}

void tokenizer::next_hex_number() {
    size_t i = input_offset + 2;
    int value = 0;

    while (i < input_length && !is_white_space(input[i]) && !is_symbol(input[i])) {
        if (input[i] >= '0' && input[i] <= '9')
            value = value * 16 + input[i++] - '0';
        else if (input[i] >= 'a' && input[i] <= 'f')
            value = value * 16 + input[i++] - 'a' + 10;
        else if (input[i] >= 'A' && input[i] <= 'F')
            value = value * 16 + input[i++] - 'A' + 10;
        else
            throw "currupted hex format";       // TODO: fix this
    }

    char* tmp = substring(input, input_offset, i - input_offset);
    current_token = token(tmp, TOKEN_TYPE_NUMBER, input_offset, value);
    input_offset = i;
}

void tokenizer::next_decimal_number() {
    size_t i = input_offset;
    int value = 0;

    while (i < input_length && !is_white_space(input[i]) && !is_symbol(input[i])) {
        if (input[i] >= '0' && input[i] <= '9')
            value = value * 10 + input[i++] - '0';
        else
            throw "currupted decimal format";   // TODO: fix this
    }

    char* tmp = substring(input, input_offset, i - input_offset);
    current_token = token(tmp, TOKEN_TYPE_NUMBER, input_offset, value);
    input_offset = i;
}

void tokenizer::next_symbol(int type, int symbol_width) {
    char* tmp = substring(input, input_offset, symbol_width);
    current_token = token(tmp, type, input_offset, 0);
    input_offset += symbol_width;
}

void tokenizer::next_identifier() {
    size_t end = input_offset + 1;
    while (end < input_length && !is_white_space(input[end]) && !is_symbol(input[end]))
        end++;

    char* id = substring(input, input_offset, end - input_offset);
    if (!strcmp(id, "def"))
        current_token = token(id, TOKEN_TYPE_KW_DEF, input_offset, 0);
    else
        current_token = token(id, input_offset);
    
    input_offset = end;
}

bool tokenizer::is_symbol(char c) const {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '(':
        case ')':
        case '=':
        case ',':
            return true;
        default:
            return false;
    }
}

bool tokenizer::is_white_space(char c) const {
    switch (c) {
        case ' ':
        case '\n':
        case '\t':
            return true;
        default:
            return false;
    }
}

void tokenizer::skip_white_space() {
    while (input_offset < input_length && is_white_space(input[input_offset]))
        input_offset++;
}
    
char* tokenizer::substring(const char* string, size_t start, size_t length) {
    if (length >= temp_string_size) {
        delete [] temp_string;
        temp_string = new char[2 * length];
        if (!temp_string)
            throw "out of memory";      // TODO: fix this
    }
    
    memcpy(temp_string, string + start, length);
    temp_string[length] = '\0';
    
    return temp_string;
}
