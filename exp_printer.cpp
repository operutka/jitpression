#include "exp_printer.h"

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

void exp_printer::visit(const expression* op) {
    op->get_top_operand()->visit(this);
}
