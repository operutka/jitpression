#include "exp_derivator.h"

#include <cstring>

exp_derivator::exp_derivator(const char* symbol) {
    this->symbol = symbol;
    this->derivative = NULL;
    this->exp_derivative = NULL;
}

operand* exp_derivator::get_derivative() {
    return derivative;
}

expression* exp_derivator::get_exp_derivative() {
    return exp_derivative;
}

void exp_derivator::visit(const numoperand* op) {
    derivative = new numoperand(0);
}

void exp_derivator::visit(const symoperand* op) {
    if (!strcmp(symbol, op->get_symbol()))
        derivative = new numoperand(1);
    else
        derivative = new numoperand(0);
}

void exp_derivator::visit(const invop* op) {
    f_checker.set_variable(symbol);
    op->get_inner()->visit(&f_checker);
    
    if (f_checker.is_function()) {
        op->get_inner()->visit(this);
        derivative = new invop(derivative);
    } else
        derivative = new numoperand(0);
}

void exp_derivator::visit(const addop* op) {
    f_checker.set_variable(symbol);
    op->get_left()->visit(&f_checker);
    
    derivative = NULL;
    if (f_checker.is_function())
        op->get_left()->visit(this);
    operand* l_operand = derivative;
    
    f_checker.set_variable(symbol);
    op->get_right()->visit(&f_checker);
    
    derivative = NULL;
    if (f_checker.is_function())
        op->get_right()->visit(this);
    
    if (l_operand && derivative)
        derivative = new addop(l_operand, derivative);
    else if (l_operand)
        derivative = l_operand;
    else if (!derivative)
        derivative = new numoperand(0);
}

void exp_derivator::visit(const subop* op) {
    f_checker.set_variable(symbol);
    op->get_left()->visit(&f_checker);
    
    derivative = NULL;
    if (f_checker.is_function())
        op->get_left()->visit(this);
    operand* l_operand = derivative;
    
    f_checker.set_variable(symbol);
    op->get_right()->visit(&f_checker);
    
    derivative = NULL;
    if (f_checker.is_function())
        op->get_right()->visit(this);
    
    if (l_operand && derivative)
        derivative = new subop(l_operand, derivative);
    else if (l_operand)
        derivative = l_operand;
    else if (derivative)
        derivative = new invop(derivative);
    else
        derivative = new numoperand(0);
}

void exp_derivator::visit(const mulop* op) {
    f_checker.set_variable(symbol);
    op->get_left()->visit(&f_checker);
    bool l_function = f_checker.is_function();
    
    f_checker.set_variable(symbol);
    op->get_right()->visit(&f_checker);
    bool r_function = f_checker.is_function();
    
    if (l_function || r_function) {
        op->get_left()->visit(this);
        operand* l_operand = derivative;
        op->get_right()->visit(this);
        
        derivative = new addop(new mulop(l_operand, op->get_right()->clone()),
                               new mulop(op->get_left()->clone(), derivative));
    } else
        derivative = new numoperand(0);
}

void exp_derivator::visit(const divop* op) {
    f_checker.set_variable(symbol);
    op->get_left()->visit(&f_checker);
    bool l_function = f_checker.is_function();
    
    f_checker.set_variable(symbol);
    op->get_right()->visit(&f_checker);
    bool r_function = f_checker.is_function();
    
    if (l_function || r_function) {
        op->get_left()->visit(this);
        operand* l_operand = derivative;
        op->get_right()->visit(this);

        operand* num = new subop(new mulop(l_operand, op->get_right()->clone()),
                                 new mulop(op->get_left()->clone(), derivative));

        operand* den = new mulop(op->get_right()->clone(), op->get_right()->clone());

        derivative = new divop(num, den);
    } else
        derivative = new numoperand(0);
}

void exp_derivator::visit(const expression* op) {
    const argtable* args = op->get_arguments();
    op->get_top_operand()->visit(this);
    exp_derivative = new expression(*args, derivative);
}

exp_derivator::function_checker::function_checker() {
    contains_symbol = false;
}

void exp_derivator::function_checker::set_variable(const char* symbol) {
    this->symbol = symbol;
    this->contains_symbol = false;
}

bool exp_derivator::function_checker::is_function() {
    return contains_symbol;
}

void exp_derivator::function_checker::visit(const numoperand* op) { }

void exp_derivator::function_checker::visit(const symoperand* op) {
    if (!strcmp(symbol, op->get_symbol()))
        contains_symbol = true;
}

void exp_derivator::function_checker::visit(const invop* op) {
    op->get_inner()->visit(this);
}

void exp_derivator::function_checker::visit(const addop* op) {
    op->get_left()->visit(this);
    op->get_right()->visit(this);
}

void exp_derivator::function_checker::visit(const subop* op) {
    op->get_left()->visit(this);
    op->get_right()->visit(this);
}

void exp_derivator::function_checker::visit(const mulop* op) {
    op->get_left()->visit(this);
    op->get_right()->visit(this);
}

void exp_derivator::function_checker::visit(const divop* op) {
    op->get_left()->visit(this);
    op->get_right()->visit(this);
}

void exp_derivator::function_checker::visit(const expression* op) {
    op->get_top_operand()->visit(this);
}
