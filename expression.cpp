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

#include "expression.h"
#include "opvisitor.h"

expression::expression(const argtable& args, operand* op) : args(args) {
    this->op = op;
}

expression::expression(const expression& orig) : args(orig.args) {
    this->op = orig.op->clone();
}

expression::~expression() {
    delete op;
}

expression& expression::operator=(const expression& other) {
    delete op;
    op = other.op->clone();
    args = other.args;
    return *this;
}

void expression::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

const operand* expression::get_top_operand() const {
    return op;
}

const argtable* expression::get_arguments() const {
    return &args;
}
