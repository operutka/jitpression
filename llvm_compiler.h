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

#ifndef LLVM_COMPILER_H
#define	LLVM_COMPILER_H

#include <string>

#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include "opvisitor.h"
#include "compiler.h"

#define INT_WIDTH       32

namespace jitpression {

    class llvm_compiler : public compiler {
    public:
        llvm_compiler();
        virtual ~llvm_compiler();

        virtual void* compile(const expression* exp);
    private:
        llvm::Module *llvm_module;
        llvm::ExecutionEngine* llvm_engine;

        class compvisitor : public opvisitor {
        public:
            compvisitor(llvm_compiler* comp);
            virtual ~compvisitor();

            void* get_result();

            virtual void visit(const numoperand* op);
            virtual void visit(const symoperand* op);
            virtual void visit(const invop* op);
            virtual void visit(const addop* op);
            virtual void visit(const subop* op);
            virtual void visit(const mulop* op);
            virtual void visit(const divop* op);
            virtual void visit(const functor* op);
            virtual void visit(const expression* op);
        private:
            void* result;

            llvm::Module* llvm_module;
            llvm::ExecutionEngine* llvm_engine;
            llvm::IRBuilder<> llvm_builder;

            std::map<std::string, llvm::Value*> named_llvm_values;
            llvm::Value* llvm_value;
        };
    };

}

#endif	/* LLVM_COMPILER_H */
