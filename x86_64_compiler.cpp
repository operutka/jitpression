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

#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <sys/mman.h>
#include <stdio.h>

#include "x86_64_compiler.h"
#include "expression.h"
#include "argtable.h"

x86_64_compiler::x86_64_compiler() {
    buffer_size = DEFAULT_BUF_SIZE;
    buffer = new unsigned char[DEFAULT_BUF_SIZE];
    if (!buffer)
        throw "out of memory";  // TODO: fix this
}

x86_64_compiler::~x86_64_compiler() {
    delete [] buffer;
}

void* x86_64_compiler::compile(const expression* exp) {
    compvisitor visitor(this);
    clear_buffer();
    exp->visit(&visitor);
    
    size_t al = buffer_offset % 256;
    void *code_addr = mmap(NULL, buffer_offset + 256 - al, 
            PROT_READ | PROT_WRITE | PROT_EXEC, 
            MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    memcpy(code_addr, buffer, buffer_offset);
    
#ifdef JP_DEBUG
    FILE* fout = fopen("function.bin", "wb");
    if (!fout)
        throw "unable to output compiled code";
    
    fwrite(buffer, sizeof(unsigned char), buffer_offset, fout);
    fflush(fout);
    fclose(fout);
#endif
    
    return code_addr;
}

void x86_64_compiler::log(const char* format, ...) {
#ifdef JP_DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

unsigned char* x86_64_compiler::get_buffer() {
    return buffer;
}

size_t x86_64_compiler::get_buffer_size() {
    return buffer_size;
}

size_t x86_64_compiler::get_buffer_offset() {
    return buffer_offset;
}

unsigned char* x86_64_compiler::realloc_buffer(size_t size) {
    unsigned char *newBuffer = new unsigned char[size];
    if (!newBuffer)
        throw "out of memory";
    
    size_t min = buffer_size < size ? buffer_size : size;
    memcpy(newBuffer, buffer, min);
    
    delete [] buffer;
    buffer = newBuffer;
    
    return buffer;
}

void x86_64_compiler::clear_buffer() {
    buffer_offset = 0;
}

void x86_64_compiler::instr(size_t length, ...) {
    if ((buffer_offset + length) > buffer_size)
        realloc_buffer(buffer_size + DEFAULT_BUF_SIZE);
    
    va_list opcode;
    va_start(opcode, length);
    for (size_t i = 0; i < length; i++)
        buffer[buffer_offset++] = va_arg(opcode, int);
    va_end(opcode);
}

unsigned char x86_64_compiler::get_rex_prefix(bool size_64, int rm_reg, int sib_index, int rm) {
    unsigned char result = 0x40;
    result |= size_64 ? 0x08 : 0;
    result |= (rm_reg & 0x08) >> 1;
    result |= (sib_index & 0x08) >> 2;
    result |= (rm & 0x08) >> 3;
    
    return result;
}

void x86_64_compiler::instr_mov_rr64(int dst_reg, int src_reg) {
    instr(3, get_rex_prefix(true, src_reg, 0, dst_reg), 0x89, 0xc0 + (src_reg << 3) + dst_reg);
    log("mov %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_mov_rr(int dst_reg, int src_reg) {
    instr(2, 0x89, 0xc0 + (src_reg << 3) + dst_reg);
    log("mov %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_mov_ri(int dst_reg, int32_t val) {
    instr(5, 0xb8 + dst_reg, val, val >> 8, val >> 16, val >> 24);
    log("mov %s, 0x%x\n", get_reg_name(dst_reg), val);
}

void x86_64_compiler::instr_mov_rm(int dst_reg, int src_mem_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x8b, (dst_reg << 3) + src_mem_reg);
        log("mov %s, [%s]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x8b, 0x80 + (dst_reg << 3) + src_mem_reg,
                displacement, 
                displacement >> 8, 
                displacement >> 16, 
                displacement >> 24);
        log("mov %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    } else {
        instr(3, 0x8b, 0x40 + (dst_reg << 3) + src_mem_reg, displacement);
        log("mov %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    }
}

void x86_64_compiler::instr_mov_mr(int dst_mem_reg, int src_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x89, (src_reg << 3) + dst_mem_reg);
        log("mov [%s], %s\n", get_reg_name(dst_mem_reg), get_reg_name(src_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x89, 0x80 + (src_reg << 3) + dst_mem_reg,
                displacement,
                displacement >> 8, 
                displacement >> 16,
                displacement >> 24);
        log("mov [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    } else {
        instr(3, 0x89, 0x40 + (src_reg << 3) + dst_mem_reg, displacement);
        log("mov [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    }
}

void x86_64_compiler::instr_add_rr(int dst_reg, int src_reg) {
    instr(2, 0x01, 0xc0 + (src_reg << 3) + dst_reg);
    log("add %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_add_rr64(int dst_reg, int src_reg) {
    instr(3, get_rex_prefix(true, src_reg, 0, dst_reg), 0x01, 0xc0 + (src_reg << 3) + dst_reg);
    log("add %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_add_ri(int dst_reg, int32_t val) {
    instr(6, 0x81, 0xc0 + dst_reg, val, val >> 8, val >> 16, val >> 24);
    log("add %s, 0x%x\n", get_reg_name(dst_reg), val);
}

void x86_64_compiler::instr_add_ri64(int dst_reg, int32_t val) {
    instr(7, get_rex_prefix(true, 0, 0, dst_reg), 0x81, 0xc0 + dst_reg, val, val >> 8, val >> 16, val >> 24);
    log("add %s, 0x%x\n", get_reg_name(dst_reg), val);
}

void x86_64_compiler::instr_add_rm(int dst_reg, int src_mem_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x03, (dst_reg << 3) + src_mem_reg);
        log("add %s, [%s]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x03, 0x80 + (dst_reg << 3) + src_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("add %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    } else {
        instr(3, 0x03, 0x40 + (dst_reg << 3) + src_mem_reg, displacement);
        log("add %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    }
}

void x86_64_compiler::instr_add_mr(int dst_mem_reg, int src_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x01, (src_reg << 3) + dst_mem_reg);
        log("add [%s], %s\n", get_reg_name(dst_mem_reg), get_reg_name(src_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x01, 0x80 + (src_reg << 3) + dst_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("add [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    } else {
        instr(3, 0x01, 0x40 + (src_reg << 3) + dst_mem_reg, displacement);
        log("add [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    }
}

void x86_64_compiler::instr_sub_rr(int dst_reg, int src_reg) {
    instr(2, 0x29, 0xc0 + (src_reg << 3) + dst_reg);
    log("sub %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_sub_rr64(int dst_reg, int src_reg) {
    instr(3, get_rex_prefix(true, src_reg, 0, dst_reg), 0x29, 0xc0 + (src_reg << 3) + dst_reg);
    log("sub %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_sub_ri(int dst_reg, int32_t val) {
    instr(6, 0x81, 0xc0 + (0x05 << 3) + dst_reg, val, val >> 8, val >> 16, val >> 24);
    log("sub %s, 0x%x\n", get_reg_name(dst_reg), val);
}

void x86_64_compiler::instr_sub_ri64(int dst_reg, int32_t val) {
    instr(7, get_rex_prefix(true, 0, 0, dst_reg), 0x81, 0xc0 + (0x05 << 3) + dst_reg, val, val >> 8, val >> 16, val >> 24);
    log("sub %s, 0x%x\n", get_reg_name(dst_reg), val);
}

void x86_64_compiler::instr_sub_rm(int dst_reg, int src_mem_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x2b, (dst_reg << 3) + src_mem_reg);
        log("sub %s, [%s]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x2b, 0x80 + (dst_reg << 3) + src_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("sub %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    } else {
        instr(3, 0x2b, 0x40 + (dst_reg << 3) + src_mem_reg, displacement);
        log("sub %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    }
}

void x86_64_compiler::instr_sub_mr(int dst_mem_reg, int src_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0x29, (src_reg << 3) + dst_mem_reg);
        log("sub [%s], %s\n", get_reg_name(dst_mem_reg), get_reg_name(src_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0x29, 0x80 + (src_reg << 3) + dst_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("sub [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    } else {
        instr(3, 0x29, 0x40 + (src_reg << 3) + dst_mem_reg, displacement);
        log("sub [%s%+d], %s\n", get_reg_name(dst_mem_reg), displacement, get_reg_name(src_reg));
    }
}

void x86_64_compiler::instr_imul_rr(int dst_reg, int src_reg) {
    instr(3, 0x0f, 0xaf, 0xc0 + (dst_reg << 3) + src_reg);
    log("imul %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_imul_rri(int dst_reg, int src_reg, int32_t val) {
    instr(6, 0x69, 0xc0 + (dst_reg << 3) + src_reg, val, val >> 8, val >> 16, val >> 24);
    log("imul %s, %s, 0x%x\n", get_reg_name(dst_reg), get_reg_name(src_reg), val);
}

void x86_64_compiler::instr_imul_rmi(int dst_reg, int src_mem_reg, int32_t displacement, int32_t val) {
    if (displacement == 0) {
        instr(6, 0x69, (dst_reg << 3) + src_mem_reg, val, val >> 8, val >> 16, val >> 24);
        log("imul %s, [%s], 0x%x\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), val);
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(10, 0x69, 0x80 + (dst_reg << 3) + src_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24,
                val, val >> 8, val >> 16, val >> 24);
        log("imul %s, [%s%+d], 0x%x\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement, val);
    } else {
        instr(7, 0x69, 0x40 + (dst_reg << 3) + src_mem_reg, displacement,
                val, val >> 8, val >> 16, val >> 24);
        log("imul %s, [%s%+d], 0x%x\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement, val);
    }
}

void x86_64_compiler::instr_imul_rm(int dst_reg, int src_mem_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(3, 0x0f, 0xaf, (dst_reg << 3) + src_mem_reg);
        log("imul %s, [%s]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(7, 0x0f, 0xaf, 0x80 + (dst_reg << 3) + src_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("imul %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    } else {
        instr(4, 0x0f, 0xaf, 0x40 + (dst_reg << 3) + src_mem_reg, displacement);
        log("imul %s, [%s%+d]\n", get_reg_name(dst_reg), get_reg_name(src_mem_reg), displacement);
    }
}

void x86_64_compiler::instr_idiv_rr(int src_reg) {
    instr(2, 0xf7, 0xc0 + (0x07 << 3) + src_reg);
    log("idiv %s\n", get_reg_name(src_reg));
}

void x86_64_compiler::instr_idiv_rm(int src_mem_reg, int32_t displacement) {
    if (displacement == 0) {
        instr(2, 0xf7, (0x07 << 3) + src_mem_reg);
        log("idiv [%s]\n", get_reg_name(src_mem_reg));
    } else if (displacement > 0x7f || displacement < -0x80) {
        instr(6, 0xf7, 0x80 + (0x07 << 3) + src_mem_reg,
                displacement,
                displacement >> 8,
                displacement >> 16,
                displacement >> 24);
        log("idiv [%s%+d]\n", get_reg_name(src_mem_reg), displacement);
    } else {
        instr(3, 0xf7, 0x40 + (0x07 << 3) + src_mem_reg, displacement);
        log("idiv [%s%+d]\n", get_reg_name(src_mem_reg), displacement);
    }
}

void x86_64_compiler::instr_xor_rr(int dst_reg, int src_reg) {
    instr(2, 0x31, 0xc0 + (src_reg << 3) + dst_reg);
    log("xor %s, %s\n", get_reg_name(dst_reg), get_reg_name(src_reg));
}

void x86_64_compiler::instr_inc_r(int reg) {
    instr(2, 0xff, 0xc0 + reg);
    log("inc %s\n", get_reg_name(reg));
}
    
void x86_64_compiler::instr_sar_ri(int reg, u_int8_t pos) {
    instr(3, 0xc1, 0xc0 + (0x07 << 3) + reg, pos);
    log("sar %s, %u\n", get_reg_name(reg), pos);
}

void x86_64_compiler::instr_sal_ri(int reg, u_int8_t pos) {
    instr(3, 0xc1, 0xc0 + (0x04 << 3) + reg, pos);
    log("sal %s, %u\n", get_reg_name(reg), pos);
}

void x86_64_compiler::instr_push_r(int reg) {
    instr(1, 0x50 + reg);
    log("push %s\n", get_reg_name(reg));
}

void x86_64_compiler::instr_pop_r(int reg) {
    instr(1, 0x58 + reg);
    log("pop %s\n", get_reg_name(reg));
}

void x86_64_compiler::instr_ret() {
    instr(1, 0xc3);
    log("ret\n");
}

const char* x86_64_compiler::get_reg_name(int reg) {
    switch (reg) {
        case REG_EAX: return "eax";
        case REG_EBX: return "ebx";
        case REG_ECX: return "ecx";
        case REG_EDX: return "edx";
        case REG_ESP: return "esp";
        case REG_EBP: return "ebp";
        case REG_ESI: return "esi";
        case REG_EDI: return "edi";
        default: return "unknown";
    }
}

x86_64_compiler::compvisitor::compvisitor(x86_64_compiler* comp) {
    this->comp = comp;
}

x86_64_compiler::compvisitor::~compvisitor() {
}
        
void x86_64_compiler::compvisitor::swap_dst_tmp() {
    int tmp = dst_reg;
    dst_reg = tmp_reg;
    tmp_reg = tmp;
}

void x86_64_compiler::compvisitor::visit(const numoperand* op) {
    comp->instr_mov_ri(dst_reg, op->get_value());
}

void x86_64_compiler::compvisitor::visit(const symoperand* op) {
    int disp = op->get_argument_index() + 1;
    comp->instr_mov_rm(dst_reg, REG_EBP, -4 * disp);
}

void x86_64_compiler::compvisitor::visit(const invop* op) {
    op->get_inner()->visit(this);
    comp->instr_mov_rr(tmp_reg, dst_reg);
    comp->instr_xor_rr(dst_reg, tmp_reg);
    comp->instr_inc_r(dst_reg);
}

void x86_64_compiler::compvisitor::visit(const addop* op) {
    op->get_left()->visit(this);
    if (op->get_right()->is_leaf()) {
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
    } else {
        comp->instr_push_r(dst_reg);
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
        comp->instr_pop_r(dst_reg);
    }
    
    comp->instr_add_rr(dst_reg, tmp_reg);
}

void x86_64_compiler::compvisitor::visit(const subop* op) {
    op->get_left()->visit(this);
    if (op->get_right()->is_leaf()) {
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
    } else {
        comp->instr_push_r(dst_reg);
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
        comp->instr_pop_r(dst_reg);
    }
    
    comp->instr_sub_rr(dst_reg, tmp_reg);
}

void x86_64_compiler::compvisitor::visit(const mulop* op) {
    op->get_left()->visit(this);
    if (op->get_right()->is_leaf()) {
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
    } else {
        comp->instr_push_r(dst_reg);
        swap_dst_tmp();
        op->get_right()->visit(this);
        swap_dst_tmp();
        comp->instr_pop_r(dst_reg);
    }
    
    comp->instr_imul_rr(dst_reg, tmp_reg);
}

void x86_64_compiler::compvisitor::visit(const divop* op) {
    int old_dst = dst_reg;
    int old_tmp = tmp_reg;
    
    dst_reg = REG_EAX;
    tmp_reg = REG_ECX;
    op->get_left()->visit(this);
    
    swap_dst_tmp();
    if (op->get_right()->is_leaf())
        op->get_right()->visit(this);
    else {
        comp->instr_push_r(REG_EAX);
        op->get_right()->visit(this);
        comp->instr_pop_r(REG_EAX);
    }
    
    comp->instr_mov_rr(REG_EDX, REG_EAX);
    comp->instr_sar_ri(REG_EDX, 0x1f);
    comp->instr_idiv_rr(REG_ECX);
    
    if (old_dst != REG_EAX)
        comp->instr_mov_rr(old_dst, REG_EAX);
    
    dst_reg = old_dst;
    tmp_reg = old_tmp;
}

void x86_64_compiler::compvisitor::visit(const expression* op) {
    exp = op;
    
    comp->instr_push_r(REG_EBP);
    comp->instr_mov_rr64(REG_EBP, REG_ESP);
    
    // copy arguments on stack
    size_t arg_count = op->get_arguments()->get_argument_count();
    for (size_t i = 0; i < arg_count; i++) {
        comp->instr_mov_rm(REG_EAX, REG_EDI, i * 4);
        comp->instr_mov_mr(REG_EBP, REG_EAX, -4 * (i + 1));
    }
    
    // move stack the pointer
    if (arg_count > 0)
        comp->instr_sub_ri64(REG_ESP, 4 * arg_count);
    
    dst_reg = REG_EAX;
    tmp_reg = REG_ECX;
    
    op->get_top_operand()->visit(this);
    
    // move back the stack pointer
    if (arg_count > 0)
        comp->instr_add_ri64(REG_ESP, 4 * arg_count);
    
    comp->instr_pop_r(REG_EBP);
    comp->instr_ret();
}
