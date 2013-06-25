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

#ifndef PARSER_H
#define	PARSER_H

#include <stack>

#include "tokenizer.h"
#include "argtable.h"

class expression;
class operand;

class parser {
public:
    parser(const char* exp);
    virtual ~parser();
    
    expression* parse();
    static expression* parse(const char* exp);
private:
    tokenizer tknzr;
    token tkn;
    
    std::stack<operand*> op_stack;
    argtable args;
    
    expression* result;
    
    void net_a();
    void net_a2();
    void net_a3();
    void net_b();
    void net_b2();
    void net_b3();
    void net_c();
    void net_c2();
    
    void compare_symbol(int symbol);
    int compare_number();
    void compare_identifier();
};

#endif	/* PARSER_H */

