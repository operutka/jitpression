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
#include "jitpression.h"
#include "llvm_compiler.h"

using namespace jitpression;

llvm_compiler* comp = NULL;

jp_context* jp_create_context() {
    return new context();
}

void jp_free_context(jp_context* c) {
    delete (context*)c;
}

void jp_register_function(jp_context* c, const char* name, const jp_function* f) {
    context* jc = (context*)c;
    jc->register_function(name, (const function*)f);
}

void jp_remove_function(jp_context* c, const char* name) {
    context* jc = (context*)c;
    jc->remove_function(name);
}

const jp_function* jp_get_function(const jp_context* c, const char* name) {
    context* jc = (context*)c;
    return jc->get_function(name);
}

jitpression::compiler* get_compiler() {
    if (comp != NULL)
        return comp;
    
    comp = new llvm_compiler();
    if (comp == NULL)
        throw "unable to initialize LLVM compiler";
    
    return comp;
}

jp_function* jp_parse_function(jp_context* c, const char* exp, int compile) {
    return function::parse((context*)c, exp, compile);
}

jp_function* jp_derive_function(const jp_function* f, const char* var) {
    return NULL;
}

void jp_free_function(jp_function* f) {
    delete (function*)f;
}

int jp_call_function(const jp_function* f, int* args) {
    function* jf = (function*)f;
    return jf->call(args);
}

size_t jp_function_arg_count(const jp_function* f) {
    function* jf = (function*)f;
    return jf->get_arguments()->get_argument_count();
}
