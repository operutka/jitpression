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

#include "exp_optimizer.h"
#include "exp_derivator.h"

using namespace jitpression;

exp_optimizer::exp_optimizer() {
    numeric = false;
    optimized = NULL;
    optimized_exp = NULL;
}

operand* exp_optimizer::get_optimized() {
    return optimized;
}

expression* exp_optimizer::get_optimized_exp() {
    return optimized_exp;
}

void exp_optimizer::visit(const numoperand* op) {
    optimized = op->clone();
    numeric = true;
    value = op->get_value();
}

void exp_optimizer::visit(const symoperand* op) {
    optimized = op->clone();
    numeric = false;
}

void exp_optimizer::visit(const invop* op) {
    op->get_inner()->visit(this);
    
    if (numeric) {
        delete optimized;
        value = -value;
        optimized = new numoperand(value);
    } else
        optimized = new invop(optimized);
}

void exp_optimizer::visit(const addop* op) {
    op->get_left()->visit(this);
    
    bool l_numeric = numeric;
    int l_value = value;
    operand* l_operand = optimized;
    
    op->get_right()->visit(this);
    
    if (l_numeric && numeric) {
        delete l_operand;
        delete optimized;
        value = l_value + value;
        optimized = new numoperand(value);
    } else if (l_numeric && l_value == 0) {
        delete l_operand;
    } else if (numeric && value == 0) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else {
        numeric = false;
        optimized = new addop(l_operand, optimized);
    }
}

void exp_optimizer::visit(const subop* op) {
    op->get_left()->visit(this);
    
    bool l_numeric = numeric;
    int l_value = value;
    operand* l_operand = optimized;
    
    op->get_right()->visit(this);
    
    if (l_numeric && numeric) {
        delete l_operand;
        delete optimized;
        value = l_value - value;
        optimized = new numoperand(value);
    } else if (l_numeric && l_value == 0) {
        delete l_operand;
        numeric = false;
        optimized = new invop(optimized);
    } else if (numeric && value == 0) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else {
        numeric = false;
        optimized = new subop(l_operand, optimized);
    }
}

void exp_optimizer::visit(const mulop* op) {
    op->get_left()->visit(this);
    
    bool l_numeric = numeric;
    int l_value = value;
    operand* l_operand = optimized;
    
    op->get_right()->visit(this);
    
    if (l_numeric && numeric) {
        delete l_operand;
        delete optimized;
        value = l_value * value;
        optimized = new numoperand(value);
    } else if (l_numeric && l_value == 0) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else if (l_numeric && l_value == 1) {
        delete l_operand;
    } else if (numeric && value == 0) {
        delete l_operand;
    } else if (numeric && value == 1) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else {
        numeric = false;
        optimized = new mulop(l_operand, optimized);
    }
}

void exp_optimizer::visit(const divop* op) {
    op->get_left()->visit(this);
    
    bool l_numeric = numeric;
    int l_value = value;
    operand* l_operand = optimized;
    
    op->get_right()->visit(this);
    
    if (l_numeric && numeric) {
        delete l_operand;
        delete optimized;
        value = l_value / value;
        optimized = new numoperand(value);
    } else if (l_numeric && l_value == 0) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else if (numeric && value == 1) {
        delete optimized;
        numeric = l_numeric;
        value = l_value;
        optimized = l_operand;
    } else {
        numeric = false;
        optimized = new divop(l_operand, optimized);
    }
}

void exp_optimizer::visit(const functor* op) {
    throw "not supported yet";
}

void exp_optimizer::visit(const expression* op) {
    const argtable* args = op->get_arguments();
    op->get_top_operand()->visit(this);
    optimized_exp = new expression(*args, optimized);
}
