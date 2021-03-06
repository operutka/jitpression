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

#ifndef EXP_PRINTER_H
#define	EXP_PRINTER_H

#include "opvisitor.h"

#include <cstdio>

namespace jitpression {

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
        virtual void visit(const functor* op);
        virtual void visit(const expression* op);

    private:
        FILE* stream;
    };

}

#endif	/* EXP_PRINTER_H */

