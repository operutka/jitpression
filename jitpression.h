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

#ifndef JITPRESSION_H
#define	JITPRESSION_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef void jp_context;
typedef void jp_function;
    
jp_context* jp_create_context();
void jp_free_context(jp_context* c);

void jp_register_function(jp_context* c, const char* name, const jp_function* f);
void jp_remove_function(jp_context* c, const char* name);
const jp_function* jp_get_function(const jp_context* c, const char* name);

jp_function* jp_parse_function(jp_context* c, const char* exp, int compile);
jp_function* jp_derive_function(const jp_function* f, const char* var);
void jp_free_function(jp_function* f);

int jp_call_function(const jp_function* f, int* args);
size_t jp_function_arg_count(const jp_function* f);

#ifdef	__cplusplus
}
#endif

#endif	/* JITPRESSION_H */

