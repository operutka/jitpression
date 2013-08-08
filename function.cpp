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

#include "function.h"
#include "parser.h"

interpreter function::intp;

function::function(expression& exp, int (*addr)(int*)) : exp(exp) {
    this->addr = addr;
}

function::function(const function& orig) : exp(orig.exp) {
    this->addr = orig.addr;
}

function::~function() {
}

int function::call(int* arguments) const {
    if (addr)
        return addr(arguments);
    
    return intp.evaluate(&exp, arguments);
}

const argtable* function::get_arguments() const {
    return exp.get_arguments();
}

function function::parse(const char* exp) {
    expression* e = parser::parse(exp);
    function result(*e, NULL);
    delete e;
    
    return result;
}
    
function function::parse(const char* exp, compiler* comp) {
    expression* e = parser::parse(exp);
    int (*code)(int*) = (int(*)(int*))comp->compile(e);
    
    function result(*e, code);
    delete e;
    
    return result;
}
