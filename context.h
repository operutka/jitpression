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

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <map>
#include <string>

#include "compiler.h"

namespace jitpression {
    
    class function;

    class context {
    public:
        context();
        context(compiler* comp);
        virtual ~context();
        
        void register_function(const char* name, const function* f);
        void remove_function(const char* name);
        const function* get_function(const char* name) const;
        
        compiler* get_compiler();
    private:
        std::map<std::string,const function*> function_map;
        compiler *comp;
        bool free_compiler;
    };

}

#endif	/* CONTEXT_H */

