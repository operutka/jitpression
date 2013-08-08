#include "llvm_compiler.h"

#include <cstdlib>
#include <string>

#include <llvm/Support/TargetSelect.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>

llvm_compiler::llvm_compiler() {
    llvm::InitializeNativeTarget();
    
    llvm_module = new llvm::Module("jitpression", llvm::getGlobalContext());
    if (!llvm_module)
        throw "out of memory";
    
    llvm_engine = llvm::EngineBuilder(llvm_module).create();
    if (!llvm_engine)
        throw "unable to initialize JIT engine";
}

llvm_compiler::~llvm_compiler() {
    delete llvm_engine;
}

void* llvm_compiler::compile(const expression* exp) {
    compvisitor visitor(this);
    exp->visit(&visitor);
    
    return visitor.get_result();
}

llvm_compiler::compvisitor::compvisitor(llvm_compiler* comp)
        : llvm_builder(llvm::getGlobalContext()) {
    
    llvm_module = comp->llvm_module;
    llvm_engine = comp->llvm_engine;
}

llvm_compiler::compvisitor::~compvisitor() {
}

void* llvm_compiler::compvisitor::get_result() {
    return result;
}

void llvm_compiler::compvisitor::visit(const numoperand* op) {
    llvm::APInt value(INT_WIDTH, op->get_value());
    llvm_value = llvm::ConstantInt::get(llvm::getGlobalContext(), value);
}

void llvm_compiler::compvisitor::visit(const symoperand* op) {
    llvm::Value* val_ptr = named_llvm_values[op->get_symbol()];
    llvm_value = llvm_builder.CreateLoad(val_ptr, "tmpop");
}

void llvm_compiler::compvisitor::visit(const invop* op) {
    op->get_inner()->visit(this);
    llvm_value = llvm_builder.CreateNeg(llvm_value, "tmpneg");
}

void llvm_compiler::compvisitor::visit(const addop* op) {
    op->get_left()->visit(this);
    llvm::Value* l = llvm_value;
    op->get_right()->visit(this);
    llvm::Value* r = llvm_value;
    
    llvm_value = llvm_builder.CreateAdd(l, r, "tmpadd");
}

void llvm_compiler::compvisitor::visit(const subop* op) {
    op->get_left()->visit(this);
    llvm::Value* l = llvm_value;
    op->get_right()->visit(this);
    llvm::Value* r = llvm_value;
    
    llvm_value = llvm_builder.CreateSub(l, r, "tmpsub");
}

void llvm_compiler::compvisitor::visit(const mulop* op) {
    op->get_left()->visit(this);
    llvm::Value* l = llvm_value;
    op->get_right()->visit(this);
    llvm::Value* r = llvm_value;
    
    llvm_value = llvm_builder.CreateMul(l, r, "tmpmul");
}

void llvm_compiler::compvisitor::visit(const divop* op) {
    op->get_left()->visit(this);
    llvm::Value* l = llvm_value;
    op->get_right()->visit(this);
    llvm::Value* r = llvm_value;
    
    llvm_value = llvm_builder.CreateSDiv(l, r, "tmpdiv");
}

void llvm_compiler::compvisitor::visit(const expression* op) {
    named_llvm_values.clear();
    
    llvm::LLVMContext &context = llvm::getGlobalContext();
    llvm::Type* rt = llvm::Type::getInt32Ty(context);
    llvm::Type* pt = llvm::Type::getInt32PtrTy(context);
    std::vector<llvm::Type*> pts(1, pt);
    
    llvm::FunctionType* ft = llvm::FunctionType::get(rt, pts, false);
    
    llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "f", llvm_module);
    
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context, "entry", f);
    llvm_builder.SetInsertPoint(block);
    
    llvm::Value* arg_pointer = f->arg_begin();
    llvm::Value* tmp;
    
    const argtable *args = op->get_arguments();
    for (size_t i = 0; i < args->get_argument_count(); i++) {
        tmp = llvm_builder.CreateConstGEP1_32(arg_pointer, i, args->get_argument_name(i));
        named_llvm_values[args->get_argument_name(i)] = tmp;
    }
    
    op->get_top_operand()->visit(this);
    llvm_builder.CreateRet(llvm_value);
    llvm::verifyFunction(*f);
    
#ifdef JP_DEBUG
    f->dump();
#endif
    
    result = llvm_engine->getPointerToFunction(f);
}
