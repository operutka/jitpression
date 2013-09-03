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
#include "context.h"

namespace jitpression {

    class function {
    public:
        function(char* name, expression* exp);
        function(char* name, expression* exp, int (*addr)(int*));
        function(const function& orig);
        virtual ~function();

        int call(int* arguments) const;
        const argtable* get_arguments() const;
        const char* get_name() const;

        static function* parse(jitpression::context* c, const char* exp, bool compile);
    private:
        static interpreter intp;

        int (*addr)(int* args);
        expression* exp;
        char* name;
        
        void compile(compiler* comp);
        char * copy_string(const char *str) const;
    };

}

#endif	/* FUNCTION_H */

