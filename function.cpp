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

#include <cstring>

using namespace jitpression;

interpreter function::intp;

function::function(char* name, expression* exp) {
    this->name = name;
    this->exp = exp;
    this->addr = NULL;
}

function::function(char* name, expression* exp, int (*addr)(int*)) {
    this->name = name;
    this->exp = exp;
    this->addr = addr;
}

function::function(const function& orig) : exp(orig.exp) {
    if (orig.name)
        this->name = copy_string(orig.name);
    else
        this->name = NULL;
    
    this->exp = orig.exp->clone();
    this->addr = orig.addr;
}

function::~function() {
    if (name)
        delete [] name;
    delete exp;
}

int function::call(int* arguments) const {
    if (addr)
        return addr(arguments);
    
    return intp.evaluate(exp, arguments);
}

const argtable* function::get_arguments() const {
    return exp->get_arguments();
}

const char* function::get_name() const {
    return name;
}

void function::compile(compiler* comp) {
    if (!addr)
        addr = (int(*)(int*))comp->compile(exp);
}

function* function::parse(context* c, const char* exp, bool compile) {
    function* f = parser::parse(c, exp);
    if (compile)
        f->compile(c->get_compiler());
    
    return f;
}

char * function::copy_string(const char* str) const {
    size_t len = strlen(str);
    char *result = new char[len + 1];
    if (!result)
        throw "out of memory";  // TODO: fix this
    
    strcpy(result, str);
    
    return result;
}
