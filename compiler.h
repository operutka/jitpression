/* 
 * File:   compiler.h
 * Author: daemonx
 *
 * Created on 7. srpen 2013, 18:07
 */

#ifndef COMPILER_H
#define	COMPILER_H

#include "expression.h"

class compiler {
public:
    virtual ~compiler() { }
    virtual void* compile(const expression* exp) = 0;
};

#endif	/* COMPILER_H */

