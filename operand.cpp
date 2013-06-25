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

#include "operand.h"
#include "opvisitor.h"

numoperand::numoperand(int value) {
    this->value = value;
}

numoperand::numoperand(const numoperand& orig) {
    this->value = orig.value;
}

numoperand::~numoperand() {
}

void numoperand::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

bool numoperand::is_leaf() const {
    return true;
}

numoperand* numoperand::clone() const {
    return new numoperand(value);
}

int numoperand::get_value() const {
    return value;
}

symoperand::symoperand(const char* symbol, int arg_index) {
    this->symbol = copy_string(symbol);
    this->arg_index = arg_index;
}

symoperand::symoperand(const symoperand& orig) {
    this->symbol = copy_string(orig.symbol);
    this->arg_index = orig.arg_index;
}

symoperand& symoperand::operator=(const symoperand& other) {
    delete [] symbol;
    symbol = copy_string(other.symbol);
    arg_index = other.arg_index;
    return *this;
}

symoperand::~symoperand() {
    delete [] symbol;
}

void symoperand::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

bool symoperand::is_leaf() const {
    return true;
}

symoperand* symoperand::clone() const {
    return new symoperand(symbol, arg_index);
}

const char* symoperand::get_symbol() const {
    return symbol;
}

int symoperand::get_argument_index() const {
    return arg_index;
}

char* symoperand::copy_string(const char* string) {
    size_t len = strlen(string);
    char *result = new char[len + 1];
    if (!result)
        throw "out of memory";  // TODO: fix this
    
    strcpy(result, string);
    
    return result;
}
