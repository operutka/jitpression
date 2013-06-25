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

#ifndef EXPRESSION_H
#define	EXPRESSION_H

#include "argtable.h"

class opvisitor;
class operand;

class expression {
public:
    expression(const argtable& args, operand* top);
    expression(const expression& orig);
    virtual ~expression();
    
    expression& operator=(const expression& other);
    
    virtual void visit(opvisitor* visitor) const;
    const operand* get_top_operand() const;
    const argtable* get_arguments() const;
private:
    argtable args;
    operand* op;
};

#endif	/* EXPRESSION_H */

