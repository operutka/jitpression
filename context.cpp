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

#include "context.h"
#include "function.h"
#include "llvm_compiler.h"

using namespace jitpression;

context::context() {
    comp = new llvm_compiler();
    free_compiler = true;
}

context::context(compiler* comp) {
    this->comp = comp;
    this->free_compiler = false;
}

context::~context() {
    if (free_compiler)
        delete comp;
}

void context::register_function(const char* name, const function* f) {
    if (function_map.count(name) > 0)
        throw "a function with the given name is already defined";
    
    function_map[name] = f;
}

void context::remove_function(const char* name) {
    function_map.erase(name);
}

const function* context::get_function(const char* name) const {
    return function_map.at(name);
}

compiler* context::get_compiler() {
    return comp;
}
