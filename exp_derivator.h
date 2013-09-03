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

#ifndef EXP_DERIVATOR_H
#define	EXP_DERIVATOR_H

#include "opvisitor.h"

namespace jitpression {

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
        virtual void visit(const functor* op);
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
            virtual void visit(const functor* op);
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

}

#endif	/* EXP_DERIVATOR_H */

