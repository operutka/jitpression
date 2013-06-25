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

#include "parser.h"
#include "operator.h"
#include "expression.h"

parser::parser(const char* expression) : tknzr(expression) {
    result = NULL;
}

parser::~parser() {
}

expression* parser::parse() {
    if (!result) {
        tkn = tknzr.next();
        if (tkn.get_type() == TOKEN_TYPE_EPS) {
            numoperand *op = new numoperand(0);
            if (!op)
                throw "out of memory";  // TODO: fix this
            
            op_stack.push(op);
        } else
            net_a();
        
        result = new expression(args, op_stack.top());
        if (!result)
            throw "out of memory";
        op_stack.pop();
    }

    return result;
}

expression* parser::parse(const char* exp) {
    parser p(exp);
    return p.parse();
}

void parser::net_a() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYMBOL:
            if (tkn.get_symbol() != SYMBOL_LBR && tkn.get_symbol() != SYMBOL_MINUS)
                break;
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

    if (tkn.get_type() == TOKEN_TYPE_SYMBOL) {
        switch (tkn.get_symbol()) {
            case SYMBOL_PLUS:
                compare_symbol(SYMBOL_PLUS);
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
            case SYMBOL_MINUS:
                compare_symbol(SYMBOL_MINUS);
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
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_a3() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYMBOL:
            switch (tkn.get_symbol()) {
                case SYMBOL_PLUS:
                case SYMBOL_MINUS:
                    net_a2();
                    net_a3();
                case SYMBOL_RBR:
                    return;
                default:
                    break;
            }
            break;
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
        case TOKEN_TYPE_SYMBOL:
            if (tkn.get_symbol() != SYMBOL_LBR && tkn.get_symbol() != SYMBOL_MINUS)
                break;
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

    if (tkn.get_type() == TOKEN_TYPE_SYMBOL) {
        switch (tkn.get_symbol()) {
            case SYMBOL_STAR:
                compare_symbol(SYMBOL_STAR);
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
            case SYMBOL_SLASH:
                compare_symbol(SYMBOL_SLASH);
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
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::net_b3() {
    switch (tkn.get_type()) {
        case TOKEN_TYPE_SYMBOL:
            switch (tkn.get_symbol()) {
                case SYMBOL_STAR:
                case SYMBOL_SLASH:
                    net_b2();
                    net_b3();
                case SYMBOL_PLUS:
                case SYMBOL_MINUS:
                case SYMBOL_RBR:
                    return;
                default:
                    break;
            }
            break;
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
        case TOKEN_TYPE_SYMBOL:
            switch (tkn.get_symbol()) {
                case SYMBOL_LBR:
                    compare_symbol(SYMBOL_LBR);
                    net_a();
                    compare_symbol(SYMBOL_RBR);
                    return;
                case SYMBOL_MINUS:
                    compare_symbol(SYMBOL_MINUS);
                    net_c2();
                    return;
                default:
                    break;
            }
            break;
        case TOKEN_TYPE_NUMBER:
            iVal = compare_number();
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
            compare_identifier();
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
            iVal = compare_number();
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
            compare_identifier();
            return;
        default:
            break;
    }

    fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
    throw "parse exception";
}

void parser::compare_symbol(int symbol) {
    if (tkn.get_type() != TOKEN_TYPE_SYMBOL || tkn.get_symbol() != symbol) {
        fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
        throw "unexpected token";
    }

    tkn = tknzr.next();
}

int parser::compare_number() {
    if (tkn.get_type() != TOKEN_TYPE_NUMBER) {
        fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
        throw "unexpected token";
    }

    int number = tkn.get_value();
    tkn = tknzr.next();
    return number;
}

void parser::compare_identifier() {
    if (tkn.get_type() != TOKEN_TYPE_IDENTIFIER) {
        fprintf(stderr, "unexpected token \"%s\" at offset %lu\n", tkn.get_text(), tkn.get_offset());
        throw "unexpected token";
    }

    tkn = tknzr.next();
}
