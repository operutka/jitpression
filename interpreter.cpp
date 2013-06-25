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

#include "interpreter.h"

interpreter::interpreter() {
}

interpreter::~interpreter() {
}

int interpreter::evaluate(const expression* exp, const int *arguments) const {
    intvisitor iv(arguments);
    iv.visit(exp);
    return iv.get_result();
}

interpreter::intvisitor::intvisitor(const int *arguments) {
    this->arguments = arguments;
    this->result = 0;
}

interpreter::intvisitor::~intvisitor() {
}

void interpreter::intvisitor::visit(const numoperand* op) {
    result = op->get_value();
}

void interpreter::intvisitor::visit(const symoperand* op) {
    result = arguments[op->get_argument_index()];
}

void interpreter::intvisitor::visit(const invop* op) {
    op->get_inner()->visit(this);
    result = -result;
}

void interpreter::intvisitor::visit(const addop* op) {
    op->get_left()->visit(this);
    int left = result;
    op->get_right()->visit(this);
    result = left + result;
}

void interpreter::intvisitor::visit(const subop* op) {
    op->get_left()->visit(this);
    int left = result;
    op->get_right()->visit(this);
    result = left - result;
}

void interpreter::intvisitor::visit(const mulop* op) {
    op->get_left()->visit(this);
    int left = result;
    op->get_right()->visit(this);
    result = left * result;
}

void interpreter::intvisitor::visit(const divop* op) {
    op->get_left()->visit(this);
    int left = result;
    op->get_right()->visit(this);
    result = left / result;
}

void interpreter::intvisitor::visit(const expression* op) {
    result = 0;
    op->get_top_operand()->visit(this);
}

int interpreter::intvisitor::get_result() const {
    return result;
}

