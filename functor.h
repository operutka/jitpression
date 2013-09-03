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

#ifndef FUNCTOR_H
#define	FUNCTOR_H

#include "operand.h"

#include <cstdlib>

namespace jitpression {
    
    class function;

    class functor : public operand {
    public:
        functor(const function* f);
        functor(const functor& orig);
        virtual ~functor();

        virtual void visit(opvisitor* visitor) const;
        virtual bool is_leaf() const;
        virtual functor* clone() const;

        const function* get_function() const;
        const operand* get_argument(size_t index) const;
        void set_argument(size_t index, operand* arg);
        size_t get_argument_count() const;
    private:
        const function* f;
        size_t arg_count;
        operand** args;
    };

}

#endif	/* FUNCTOR_H */

