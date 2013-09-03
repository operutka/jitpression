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

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "jitpression.h"

int main() {
    char line[4096];
    jp_context* c;
    jp_function *fi, *fc;
    size_t arg_count, i;
    int test_runs, result, j, *args;
    double ts;
    
    printf("Enter an expression:\n");
    fgets(line, 4095, stdin);

    c = jp_create_context();
    if (!c) {
        printf("out of memory");
        return 1;
    }
    
    fi = jp_parse_function(c, line, 0);
    fc = jp_parse_function(c, line, 1);
    arg_count = jp_function_arg_count(fi);

    args = malloc(sizeof(int) * arg_count);
    if (!args) {
        printf("out of memory");
        return 1;
    }

    printf("Enter %lu integral arguments:\n", arg_count);
    for (i = 0; i < arg_count; i++)
        scanf("%d", args + i);

    result = 0;
    printf("Enter number of test runs:\n");
    scanf("%d", &test_runs);

    ts = omp_get_wtime();

    printf("Testing interpreter...\n");
    for (j = 0; j < test_runs; j++)
        result = jp_call_function(fi, args);
    printf("%d test runs took %g seconds (with result %d).\n", test_runs, omp_get_wtime() - ts, result);

    ts = omp_get_wtime();

    printf("Testing compiled function...\n");
    for (j = 0; j < test_runs; j++)
        result = jp_call_function(fc, args);
    printf("%d test runs took %g seconds (with result %d).\n", test_runs, omp_get_wtime() - ts, result);

    free(args);
    jp_free_function(fi);
    jp_free_function(fc);
    jp_free_context(c);
    
    return 0;
}


