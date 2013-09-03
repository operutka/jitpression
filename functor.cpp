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

#include "functor.h"
#include "function.h"

using namespace jitpression;

functor::functor(const function* f) {
    this->f = f;
    
    const argtable* args = f->get_arguments();
    this->arg_count = args->get_argument_count();
    this->args = new operand*[arg_count];
    if (!this->args)
        throw "out of memory";
}

functor::functor(const functor& orig) {
    f = orig.f;
    
    const argtable* args = f->get_arguments();
    this->arg_count = args->get_argument_count();
    this->args = new operand*[arg_count];
    if (!this->args)
        throw "out of memory";
    
    for (size_t i = 0; i < arg_count; i++)
        this->args[i] = orig.args[i]->clone();
}

functor::~functor() {
    for (size_t i = 0; i < arg_count; i++)
        delete args[i];
    delete [] args;
}

void functor::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

bool functor::is_leaf() const {
    return arg_count == 0;
}

functor* functor::clone() const {
    return new functor(*this);
}

const function* functor::get_function() const {
    return f;
}

const operand* functor::get_argument(size_t index) const {
    return args[index];
}

void functor::set_argument(size_t index, operand* arg) {
    args[index] = arg;
}

size_t functor::get_argument_count() const {
    return arg_count;
}
