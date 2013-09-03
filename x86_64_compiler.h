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

#ifndef X86_64_COMPILER_H
#define	X86_64_COMPILER_H

#include <cstdint>

#include "opvisitor.h"
#include "compiler.h"

#define DEFAULT_BUF_SIZE        4096

#define REG_EAX         0x00
#define REG_EBX         0x03
#define REG_ECX         0x01
#define REG_EDX         0x02
#define REG_ESP         0x04
#define REG_EBP         0x05
#define REG_ESI         0x06
#define REG_EDI         0x07

namespace jitpression {

    class x86_64_compiler : public compiler {
    public:
        x86_64_compiler();
        virtual ~x86_64_compiler();

        virtual void* compile(const expression* exp);
    private:
        unsigned char *buffer;
        size_t buffer_offset;
        size_t buffer_size;

        void log(const char* format, ...);

        unsigned char* get_buffer();
        size_t get_buffer_size();
        size_t get_buffer_offset();
        unsigned char* realloc_buffer(size_t size);
        void clear_buffer();

        void instr(size_t length, ...);
        unsigned char get_rex_prefix(bool size_64, int rm_reg, int sib_index, int rm);

        void instr_mov_rr64(int dst_reg, int src_reg);
        void instr_mov_rr(int dst_reg, int src_reg);
        void instr_mov_ri(int dst_reg, int32_t val);
        void instr_mov_rm(int dst_reg, int src_mem_reg, int32_t displacement);
        void instr_mov_mr(int dst_mem_reg, int src_reg, int32_t displacement);

        void instr_add_rr(int dst_reg, int src_reg);
        void instr_add_rr64(int dst_reg, int src_reg);
        void instr_add_ri(int dst_reg, int32_t val);
        void instr_add_ri64(int dst_reg, int32_t val);
        void instr_add_rm(int dst_reg, int src_mem_reg, int32_t displacement);
        void instr_add_mr(int dst_mem_reg, int src_reg, int32_t displacement);

        void instr_sub_rr(int dst_reg, int src_reg);
        void instr_sub_rr64(int dst_reg, int src_reg);
        void instr_sub_ri(int dst_reg, int32_t val);
        void instr_sub_ri64(int dst_reg, int32_t val);
        void instr_sub_rm(int dst_reg, int src_mem_reg, int32_t displacement);
        void instr_sub_mr(int dst_mem_reg, int src_reg, int32_t displacement);

        void instr_imul_rr(int dst_reg, int src_reg);
        void instr_imul_rri(int dst_reg, int src_reg, int32_t val);
        void instr_imul_rmi(int dst_reg, int src_mem_reg, int32_t displacement, int32_t val);
        void instr_imul_rm(int dst_reg, int src_mem_reg, int32_t displacement);

        void instr_idiv_rr(int src_reg);
        void instr_idiv_rm(int src_mem_reg, int32_t displacement);

        void instr_xor_rr(int dst_reg, int src_reg);

        void instr_inc_r(int reg);

        void instr_sar_ri(int reg, u_int8_t pos);
        void instr_sal_ri(int reg, u_int8_t pos);

        void instr_push_r(int reg);
        void instr_pop_r(int reg);

        void instr_ret();

        const char* get_reg_name(int reg);

        class compvisitor : public opvisitor {
        public:
            compvisitor(x86_64_compiler* comp);
            virtual ~compvisitor();

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
            const expression* exp;
            x86_64_compiler* comp;

            int dst_reg;
            int tmp_reg;

            void swap_dst_tmp();
        };
    };

}

#endif	/* X86_64_COMPILER_H */

