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

#ifndef ARGTABLE_H
#define	ARGTABLE_H

#include <string>
#include <vector>

class argtable {
public:
    argtable();
    argtable(const argtable& orig);
    virtual ~argtable();
    
    size_t add_symbol(const char *symbol);
    size_t get_argument_index(const char *symbol) const;
    size_t get_argument_count() const;
    const char* get_argument_name(size_t index) const;
private:
    std::vector<std::string> args;
};

#endif	/* ARGTABLE_H */

