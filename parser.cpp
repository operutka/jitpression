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

#include <cstring>

#include "parser.h"
#include "operator.h"
#include "expression.h"
#include "function.h"

using namespace jitpression;

parser::parser(const context* c, const char* expression) : tknzr(expression) {
    this->c = c;
    this->result = NULL;
}

parser::~parser() {
}

function* parser::parse() {
    if (!result) {
        name = NULL;
        tkn = tknzr.next();
        if (tkn.get_type() == TOKEN_TYPE_EPS) {
            numoperand *op = new numoperand(0);
            if (!op)
                throw "out of memory";  // TODO: fix this
            
            op_stack.push(op);
        } else
            input_def();
        
        expression* exp = new expression(args, op_stack_pop());
        if (!exp)
            throw "out of memory";
        
        result = new function(name, exp);
        if (!result)
            throw "out of memory";
    }

    return result;
}

function* parser::parse(const context* c, const char* exp) {
    parser p(c, exp);
    return p.parse();
}

void parser::input_def() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_KW_DEF:
            compare_token(TOKEN_TYPE_KW_DEF);
            name = copy_string(tkn.get_text());
            compare_token(TOKEN_TYPE_IDENTIFIER);
            compare_token(TOKEN_TYPE_SYM_LBR);
            input_def2();
            return;
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_as();
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_def2() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
            compare_token(TOKEN_TYPE_SYM_RBR);
            compare_token(TOKEN_TYPE_SYM_EQUAL);
            input_as();
            return;
        case TOKEN_TYPE_IDENTIFIER:
            input_defb();
            compare_token(TOKEN_TYPE_SYM_RBR);
            compare_token(TOKEN_TYPE_SYM_EQUAL);
            input_as();
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_defb() {
    const char *sVal;
    
    if (tkn.get_type() == TOKEN_TYPE_IDENTIFIER) {
        sVal = tkn.get_text();
        args.add_symbol(sVal);
        compare_token(TOKEN_TYPE_IDENTIFIER);
        input_defb2();
    } else {
        fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
        throw "parse exception";
    }
}

void parser::input_defb2() {
    const char *sVal;
    
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_COMMA:
            compare_token(TOKEN_TYPE_SYM_COMMA);
            sVal = tkn.get_text();
            args.add_symbol(sVal);
            compare_token(TOKEN_TYPE_IDENTIFIER);
            input_defb2();
            return;
        case TOKEN_TYPE_SYM_RBR:
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_as() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_md();
            input_as2();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_as2() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_PLUS:
            compare_token(TOKEN_TYPE_SYM_PLUS);
            input_md();
            output_add();
            input_as2();
            return;
        case TOKEN_TYPE_SYM_MINUS:
            compare_token(TOKEN_TYPE_SYM_MINUS);
            input_md();
            output_sub();
            input_as2();
            return;
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_SYM_COMMA:
        case TOKEN_TYPE_EPS:
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_md() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_p();
            input_md2();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_md2() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_STAR:
            compare_token(TOKEN_TYPE_SYM_STAR);
            input_p();
            output_mul();
            input_md2();
            return;
        case TOKEN_TYPE_SYM_SLASH:
            compare_token(TOKEN_TYPE_SYM_SLASH);
            input_p();
            output_div();
            input_md2();
            return;
        case TOKEN_TYPE_SYM_PLUS:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_SYM_COMMA:
        case TOKEN_TYPE_EPS:
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_p() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_s();
            input_p2();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_p2() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_POW:
            compare_token(TOKEN_TYPE_SYM_POW);
            input_s();
            output_pow();
            input_p2();
            return;
        case TOKEN_TYPE_SYM_PLUS:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_SYM_STAR:
        case TOKEN_TYPE_SYM_SLASH:
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_SYM_COMMA:
        case TOKEN_TYPE_EPS:
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_s() {
    char *id;
    
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_MINUS:
            compare_token(TOKEN_TYPE_SYM_MINUS);
            input_s4();
            return;
        case TOKEN_TYPE_SYM_LBR:
            compare_token(TOKEN_TYPE_SYM_LBR);
            input_as();
            compare_token(TOKEN_TYPE_SYM_RBR);
            return;
        case TOKEN_TYPE_NUMBER:
            output_number(tkn.get_value());
            compare_token(TOKEN_TYPE_NUMBER);
            return;
        case TOKEN_TYPE_IDENTIFIER:
            id = copy_string(tkn.get_text());
            compare_token(TOKEN_TYPE_IDENTIFIER);
            input_s2(id);
            delete [] id;
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_s2(const char *id) {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
            compare_token(TOKEN_TYPE_SYM_LBR);
            create_functor(id);
            input_s3();
            return;
        case TOKEN_TYPE_SYM_PLUS:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_SYM_STAR:
        case TOKEN_TYPE_SYM_SLASH:
        case TOKEN_TYPE_SYM_POW:
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_SYM_COMMA:
        case TOKEN_TYPE_EPS:
            output_variable(id);
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_s3() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_RBR:
            compare_token(TOKEN_TYPE_SYM_RBR);
            output_functor();
            return;
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_fb();
            compare_token(TOKEN_TYPE_SYM_RBR);
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_s4() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_NUMBER:
            output_number(-tkn.get_value());
            compare_token(TOKEN_TYPE_NUMBER);
            return;
        case TOKEN_TYPE_IDENTIFIER:
            output_variable(tkn.get_text());
            output_inv();
            compare_token(TOKEN_TYPE_IDENTIFIER);
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_fb() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            input_as();
            output_functor_argument();
            input_fb2();
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::input_fb2() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_RBR:
            return;
        case TOKEN_TYPE_SYM_COMMA:
            compare_token(TOKEN_TYPE_SYM_COMMA);
            input_as();
            output_functor_argument();
            input_fb2();
            return;
        default:
            break;
    }
    
    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::output_inv() {
    operand *op = op_stack_pop();
    op = new invop(op);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_add() {
    operand *r = op_stack_pop();
    operand *l = op_stack_pop();
    operand *op = new addop(l, r);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_sub() {
    operand *r = op_stack_pop();
    operand *l = op_stack_pop();
    operand *op = new subop(l, r);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_mul() {
    operand *r = op_stack_pop();
    operand *l = op_stack_pop();
    operand *op = new mulop(l, r);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_div() {
    operand *r = op_stack_pop();
    operand *l = op_stack_pop();
    operand *op = new divop(l, r);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_pow() {
    operand *r = op_stack_pop();
    operand *l = op_stack_pop();
    operand *op = new numoperand(0);
    op_stack_push(op);
    // TODO: add pow() functor on the top of the stack
    
    //operand *op = new powop(l, r);
    //if (!op)
    //    throw "out of memory";
    
    //op_stack_push(op);
}

void parser::output_number(int val) {
    operand *op = new numoperand(val);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_variable(const char* id) {
    int index = args.get_argument_index(id);
    if (index < 0)
        throw "variable is not defined";
        
    operand *op = new symoperand(id, index);
    if (!op)
        throw "out of memory";
    
    op_stack_push(op);
}

void parser::output_functor() {
    if (fnctr_arg_index != fnctr->get_argument_count())
        throw "invalid argument count";
    
    op_stack_push(fnctr);
}

void parser::output_functor_argument() {
    if (fnctr_arg_index >= fnctr->get_argument_count())
        throw "invalid argument count";
    
    operand* op = op_stack_pop();
    fnctr->set_argument(fnctr_arg_index++, op);
}

void parser::create_functor(const char* fid) {
    const function* f = c->get_function(fid);
    if (!f)
        throw "unknown function";
    
    fnctr = new functor(f);
    if (!fnctr)
        throw "out of memory";
    
    fnctr_arg_index = 0;
}

void parser::op_stack_push(operand *op) {
    op_stack.push(op);
}

operand * parser::op_stack_pop() {
    operand *op = op_stack.top();
    op_stack.pop();
    
    return op;
}

void parser::net_a() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            net_b();
            net_a3();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_a2() {
    operand *l, *r, *op;

    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_PLUS:
            compare_token(TOKEN_TYPE_SYM_PLUS);
            net_b();
            r = op_stack.top();
            op_stack.pop();
            l = op_stack.top();
            op_stack.pop();
            op = new addop(l, r);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        case TOKEN_TYPE_SYM_MINUS:
            compare_token(TOKEN_TYPE_SYM_MINUS);
            net_b();
            r = op_stack.top();
            op_stack.pop();
            l = op_stack.top();
            op_stack.pop();
            op = new subop(l, r);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_a3() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_PLUS:
        case TOKEN_TYPE_SYM_MINUS:
            net_a2();
            net_a3();
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_EPS:
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_b() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_NUMBER:
        case TOKEN_TYPE_IDENTIFIER:
            net_c();
            net_b3();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_b2() {
    operand *l, *r, *op;

    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_STAR:
            compare_token(TOKEN_TYPE_SYM_STAR);
            net_c();
            r = op_stack.top();
            op_stack.pop();
            l = op_stack.top();
            op_stack.pop();
            op = new mulop(l, r);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        case TOKEN_TYPE_SYM_SLASH:
            compare_token(TOKEN_TYPE_SYM_SLASH);
            net_c();
            r = op_stack.top();
            op_stack.pop();
            l = op_stack.top();
            op_stack.pop();
            op = new divop(l, r);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_b3() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_STAR:
        case TOKEN_TYPE_SYM_SLASH:
            net_b2();
            net_b3();
        case TOKEN_TYPE_SYM_PLUS:
        case TOKEN_TYPE_SYM_MINUS:
        case TOKEN_TYPE_SYM_RBR:
        case TOKEN_TYPE_EPS:
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_c() {
    operand* op;
    const char* sVal;
    int iVal;

    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYM_LBR:
            compare_token(TOKEN_TYPE_SYM_LBR);
            net_a();
            compare_token(TOKEN_TYPE_SYM_RBR);
            return;
        case TOKEN_TYPE_SYM_MINUS:
            compare_token(TOKEN_TYPE_SYM_MINUS);
            net_c2();
            return;
        case TOKEN_TYPE_NUMBER:
            iVal = tkn.get_value();
            compare_token(TOKEN_TYPE_NUMBER);
            op = new numoperand(iVal);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        case TOKEN_TYPE_IDENTIFIER:
            sVal = tkn.get_text();
            iVal = args.add_symbol(sVal);
            op = new symoperand(sVal, iVal);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            compare_token(TOKEN_TYPE_IDENTIFIER);
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_c2() {
    operand* op;
    const char* sVal;
    int iVal;

    switch (tkn.get_type()) {
        case TOKEN_TYPE_NUMBER:
            iVal = tkn.get_value();
            compare_token(TOKEN_TYPE_NUMBER);
            op = new numoperand(-iVal);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            return;
        case TOKEN_TYPE_IDENTIFIER:
            sVal = tkn.get_text();
            iVal = args.add_symbol(sVal);
            op = new symoperand(sVal, iVal);
            if (!op)
                throw "out of memory";
            op = new invop(op);
            if (!op)
                throw "out of memory";
            op_stack.push(op);
            compare_token(TOKEN_TYPE_IDENTIFIER);
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::compare_token(int token_type) {
    if (tkn.get_type() != token_type) {
        fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
        throw "unexpected token";
    }

    tkn = tknzr.next();
}

char * parser::copy_string(const char* str) const {
    size_t len = strlen(str);
    char *result = new char[len + 1];
    if (!result)
        throw "out of memory";  // TODO: fix this
    
    strcpy(result, str);
    
    return result;
}
