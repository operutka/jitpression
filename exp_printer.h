/* 
 * File:   exp_printer.h
 * Author: daemonx
 *
 * Created on 8. srpen 2013, 9:19
 */

#ifndef EXP_PRINTER_H
#define	EXP_PRINTER_H

#include "opvisitor.h"

#include <cstdio>

class exp_printer : public opvisitor {
public:
    exp_printer();
    exp_printer(FILE* out);
    
    virtual void visit(const numoperand* op);
    virtual void visit(const symoperand* op);
    virtual void visit(const invop* op);
    virtual void visit(const addop* op);
    virtual void visit(const subop* op);
    virtual void visit(const mulop* op);
    virtual void visit(const divop* op);
    virtual void visit(const expression* op);
    
private:
    FILE* stream;
};

#endif	/* EXP_PRINTER_H */

