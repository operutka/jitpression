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

#ifndef OPVISITOR_H
#define	OPVISITOR_H

#include "operand.h"
#include "operator.h"
#include "expression.h"
#include "functor.h"

namespace jitpression {

    class opvisitor {
    public:
        virtual ~opvisitor() { }

        virtual void visit(const numoperand* op) = 0;
        virtual void visit(const symoperand* op) = 0;
        virtual void visit(const invop* op) = 0;
        virtual void visit(const addop* op) = 0;
        virtual void visit(const subop* op) = 0;
        virtual void visit(const mulop* op) = 0;
        virtual void visit(const divop* op) = 0;
        virtual void visit(const functor* op) = 0;

        virtual void visit(const expression* op) = 0;
    };

}

#endif	/* OPVISITOR_H */

