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

#include "argtable.h"

argtable::argtable() {
}

argtable::argtable(const argtable& orig) {
    args = orig.args;
}

argtable::~argtable() {
}

size_t argtable::add_symbol(const char *symbol) {
    std::string ssymbol(symbol);
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == ssymbol)
            return i;
    }
    
    args.push_back(ssymbol);
    return args.size() - 1;
}

size_t argtable::get_argument_index(const char *symbol) const {
    std::string ssymbol(symbol);
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == ssymbol)
            return i;
    }
    
    return -1;
}

size_t argtable::get_argument_count() const {
    return args.size();
}

const char* argtable::get_argument_name(size_t index) const {
    return args[index].c_str();
}
