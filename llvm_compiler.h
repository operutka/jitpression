/* 
 * File:   llvm_compiler.h
 * Author: daemonx
 *
 * Created on 7. srpen 2013, 11:12
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

#endif	/* LLVM_COMPILER_H */
