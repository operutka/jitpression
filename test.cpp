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

#include <cstdlib>
#include <cstdio>
#include <omp.h>

#include "function.h"
#include "llvm_compiler.h"

int main() {
    char line[4096];
    
    try {
        printf("Enter an expression:\n");
        fgets(line, 4095, stdin);

        llvm_compiler compiler;
        function fi = function::parse(line);
        function fc = function::parse(line, &compiler);
        const argtable* at = fi.get_arguments();

        int* args = new int[at->get_argument_count()];
        if (!args)
            throw "out of memory";

        printf("Enter %lu integral arguments:\n", at->get_argument_count());
        for (size_t i = 0; i < at->get_argument_count(); i++)
            scanf("%d", args + i);
        
        int test_runs, result = 0;
        printf("Enter number of test runs:\n");
        scanf("%d", &test_runs);
        
        double ts = omp_get_wtime();
        
        printf("Testing interpreter...\n");
        for (int i = 0; i < test_runs; i++)
            result = fi.call(args);
        printf("%d test runs took %g seconds (with result %d).\n", test_runs, omp_get_wtime() - ts, result);
        
        ts = omp_get_wtime();
        
        printf("Testing compiled function...\n");
        for (int i = 0; i < test_runs; i++)
            result = fc.call(args);
        printf("%d test runs took %g seconds (with result %d).\n", test_runs, omp_get_wtime() - ts, result);
        
        delete [] args;
    } catch (const char* ex) {
        printf("ERROR: %s\n", ex);
    }
    
    return 0;
}

