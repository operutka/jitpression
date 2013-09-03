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

#include "exp_printer.h"

using namespace jitpression;

exp_printer::exp_printer() {
    stream = stdout;
}

exp_printer::exp_printer(FILE* out) {
    stream = out;
}

void exp_printer::visit(const numoperand* op) {
    fprintf(stream, "%d", op->get_value());
}

void exp_printer::visit(const symoperand* op) {
    fprintf(stream, "%s", op->get_symbol());
}

void exp_printer::visit(const invop* op) {
    fprintf(stream, "-");
    if (op->get_inner()->is_leaf())
        op->get_inner()->visit(this);
    else {
        fprintf(stream, "(");
        op->get_inner()->visit(this);
        fprintf(stream, ")");
    }
}

void exp_printer::visit(const addop* op) {
    const operand* l = op->get_left();
    if (l->is_leaf())
        l->visit(this);
    else {
        fprintf(stream, "(");
        l->visit(this);
        fprintf(stream, ")");
    }
    
    fprintf(stream, " + ");
    
    const operand* r = op->get_right();
    if (r->is_leaf())
        r->visit(this);
    else {
        fprintf(stream, "(");
        r->visit(this);
        fprintf(stream, ")");
    }
}

void exp_printer::visit(const subop* op) {
    const operand* l = op->get_left();
    if (l->is_leaf())
        l->visit(this);
    else {
        fprintf(stream, "(");
        l->visit(this);
        fprintf(stream, ")");
    }
    
    fprintf(stream, " - ");
    
    const operand* r = op->get_right();
    if (r->is_leaf())
        r->visit(this);
    else {
        fprintf(stream, "(");
        r->visit(this);
        fprintf(stream, ")");
    }
}

void exp_printer::visit(const mulop* op) {
    const operand* l = op->get_left();
    if (l->is_leaf())
        l->visit(this);
    else {
        fprintf(stream, "(");
        l->visit(this);
        fprintf(stream, ")");
    }
    
    fprintf(stream, " * ");
    
    const operand* r = op->get_right();
    if (r->is_leaf())
        r->visit(this);
    else {
        fprintf(stream, "(");
        r->visit(this);
        fprintf(stream, ")");
    }
}

void exp_printer::visit(const divop* op) {
    const operand* l = op->get_left();
    if (l->is_leaf())
        l->visit(this);
    else {
        fprintf(stream, "(");
        l->visit(this);
        fprintf(stream, ")");
    }
    
    fprintf(stream, " / ");
    
    const operand* r = op->get_right();
    if (r->is_leaf())
        r->visit(this);
    else {
        fprintf(stream, "(");
        r->visit(this);
        fprintf(stream, ")");
    }
}

void exp_printer::visit(const functor* op) {
    throw "not supported yet";
}

void exp_printer::visit(const expression* op) {
    op->get_top_operand()->visit(this);
}
