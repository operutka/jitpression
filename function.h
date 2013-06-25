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

#ifndef FUNCTION_H
#define	FUNCTION_H

#include <cstdlib>

#include "argtable.h"
#include "compiler.h"
#include "interpreter.h"

class function {
public:
    function(expression& args, int (*addr)(int*));
    function(const function& orig);
    virtual ~function();
    
    int call(int* arguments) const;
    const argtable* get_arguments() const;
    
    static function parse(const char* exp);
    static function parse(const char* exp, bool compile);
private:
    static compiler comp;
    static interpreter intp;
    
    int (*addr)(int* args);
    expression exp;
};

#endif	/* FUNCTION_H */

