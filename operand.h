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

#ifndef OPERAND_H
#define	OPERAND_H

namespace jitpression {

    class opvisitor;

    class operand {
    public:
        virtual ~operand() { }
        virtual void visit(opvisitor* visitor) const = 0;
        virtual bool is_leaf() const = 0;
        virtual operand* clone() const = 0;
    private:
    };

    class numoperand : public operand {
    public:
        numoperand(int value);
        numoperand(const numoperand& orig);
        virtual ~numoperand();

        virtual void visit(opvisitor* visitor) const;
        virtual bool is_leaf() const;
        virtual numoperand* clone() const;
        int get_value() const;
    private:
        int value;
    };

    class symoperand : public operand {
    public:
        symoperand(const char* symbol, int arg_index);
        symoperand(const symoperand& orig);
        virtual ~symoperand();

        symoperand& operator=(const symoperand& other);

        virtual void visit(opvisitor* visitor) const;
        virtual bool is_leaf() const;
        virtual symoperand* clone() const;
        const char* get_symbol() const;
        int get_argument_index() const;
    private:
        char* symbol;
        int arg_index;

        char* copy_string(const char* string);
    };

}

#endif	/* OPERAND_H */

