/* 
 * File:   exp_optimizer.h
 * Author: daemonx
 *
 * Created on 7. srpen 2013, 18:14
 */

#ifndef EXP_OPTIMIZER_H
#define	EXP_OPTIMIZER_H

#include "opvisitor.h"

class exp_optimizer : public opvisitor {
public:
    exp_optimizer();
    operand* get_optimized();
    expression* get_optimized_exp();
    
    virtual void visit(const numoperand* op);
    virtual void visit(const symoperand* op);
    virtual void visit(const invop* op);
    virtual void visit(const addop* op);
    virtual void visit(const subop* op);
    virtual void visit(const mulop* op);
    virtual void visit(const divop* op);
    virtual void visit(const expression* op);
    
private:
    operand* optimized;
    expression* optimized_exp;
    bool numeric;
    int value;
};

#endif	/* EXP_OPTIMIZER_H */

