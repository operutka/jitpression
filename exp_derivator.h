/* 
 * File:   exp_derivator.h
 * Author: daemonx
 *
 * Created on 7. srpen 2013, 18:56
 */

#ifndef EXP_DERIVATOR_H
#define	EXP_DERIVATOR_H

#include "opvisitor.h"

class exp_derivator : public opvisitor {
public:
    exp_derivator(const char* symbol);
    operand* get_derivative();
    expression* get_exp_derivative();
    
    virtual void visit(const numoperand* op);
    virtual void visit(const symoperand* op);
    virtual void visit(const invop* op);
    virtual void visit(const addop* op);
    virtual void visit(const subop* op);
    virtual void visit(const mulop* op);
    virtual void visit(const divop* op);
    virtual void visit(const expression* op);
    
private:
    class function_checker : public opvisitor {
    public:
        function_checker();
        
        void set_variable(const char* symbol);
        bool is_function();
        
        virtual void visit(const numoperand* op);
        virtual void visit(const symoperand* op);
        virtual void visit(const invop* op);
        virtual void visit(const addop* op);
        virtual void visit(const subop* op);
        virtual void visit(const mulop* op);
        virtual void visit(const divop* op);
        virtual void visit(const expression* op);
    private:
        const char* symbol;
        bool contains_symbol;
    };
    
    const char* symbol;
    operand* derivative;
    expression* exp_derivative;
    function_checker f_checker;
};

#endif	/* EXP_DERIVATOR_H */

