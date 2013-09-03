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

#include "token.h"

using namespace jitpression;

token::token(const char* text, int type, size_t offset, int value) {
    init(text, type, offset, value);
}

token::token(const char* text, size_t offset) {
    init(text, TOKEN_TYPE_IDENTIFIER, offset, 0);
}

token::token(int offset) {
    init("EOF", TOKEN_TYPE_EPS, offset, 0);
}

token::token(const token& orig) {
    init(orig.text, orig.type, orig.offset, orig.value);
}

token::token() {
    init(NULL, -1, -1, 0);
}

void token::init(const char* text, int type, size_t offset, int value) {
    this->text = text;
    this->type = type;
    this->offset = offset;
    this->value = value;
}

const char* token::get_text() const {
    return text;
}

int token::get_type() const {
    return type;
}

size_t token::get_offset() const {
    return offset;
}

int token::get_value() const {
    return value;
}
