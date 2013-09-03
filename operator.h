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

#ifndef OPERATOR_H
#define	OPERATOR_H

#include "operand.h"

namespace jitpression {

    class invop : public operand {
    public:
        invop(operand* inner);
        invop(const invop& orig);
        virtual ~invop();

        virtual invop& operator=(const invop& other);

        virtual bool is_leaf() const;
        virtual void visit(opvisitor* visitor) const;
        virtual invop* clone() const;
        const operand* get_inner() const;
    protected:
        operand* inner;
    };

    class binop : public operand {
    public:
        binop(operand* left, operand* right);
        binop(const binop& orig);
        virtual ~binop();

        virtual binop& operator=(const binop& other);

        virtual bool is_leaf() const;
        const operand* get_left() const;
        const operand* get_right() const;
    private:
        operand* left;
        operand* right;
    };

    class addop : public binop {
    public:
        addop(operand* left, operand* right);
        addop(const addop& orig);
        virtual ~addop();

        virtual void visit(opvisitor* visitor) const;
        virtual addop* clone() const;
    private:
    };

    class subop : public binop {
    public:
        subop(operand* left, operand* right);
        subop(const subop& orig);
        virtual ~subop();

        virtual void visit(opvisitor* visitor) const;
        virtual subop* clone() const;
    private:
    };

    class mulop : public binop {
    public:
        mulop(operand* left, operand* right);
        mulop(const mulop& orig);
        virtual ~mulop();

        virtual void visit(opvisitor* visitor) const;
        virtual mulop* clone() const;
    private:
    };

    class divop : public binop {
    public:
        divop(operand* left, operand* right);
        divop(const divop& orig);
        virtual ~divop();

        virtual void visit(opvisitor* visitor) const;
        virtual divop* clone() const;
    private:
    };

}

#endif	/* OPERATOR_H */

