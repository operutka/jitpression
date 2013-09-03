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
#include "context.h"
#include "functor.h"

namespace jitpression {

    class expression;
    class operand;

    class parser {
    public:
        parser(const context* c, const char* exp);
        virtual ~parser();

        function* parse();
        static function* parse(const context* c, const char* exp);
    private:
        const context* c;
        
        tokenizer tknzr;
        token tkn;

        std::stack<operand*> op_stack;
        argtable args;
        functor *fnctr;
        size_t fnctr_arg_index;

        char *name;
        function* result;

        void net_a();
        void net_a2();
        void net_a3();
        void net_b();
        void net_b2();
        void net_b3();
        void net_c();
        void net_c2();

        void input_def();
        void input_def2();
        void input_defb();
        void input_defb2();
        void input_as();
        void input_as2();
        void input_md();
        void input_md2();
        void input_p();
        void input_p2();
        void input_s();
        void input_s2(const char *id);
        void input_s3();
        void input_s4();
        void input_fb();
        void input_fb2();

        void output_inv();
        void output_add();
        void output_sub();
        void output_mul();
        void output_div();
        void output_pow();
        void output_number(int val);
        void output_variable(const char *id);
        void output_functor();
        void output_functor_argument();
        
        void create_functor(const char* fid);

        void op_stack_push(operand *op);
        operand * op_stack_pop();

        void compare_token(int token_type);

        char * copy_string(const char *str) const;
    };

}

#endif	/* PARSER_H */

