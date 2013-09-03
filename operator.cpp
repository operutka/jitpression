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

#include "operator.h"
#include "opvisitor.h"

using namespace jitpression;

invop::invop(operand* inner) {
    this->inner = inner;
}

invop::invop(const invop& orig) {
    this->inner = orig.inner->clone();
}

invop::~invop() {
    delete inner;
}

invop& invop::operator=(const invop& other) {
    delete inner;
    inner = other.inner->clone();
    return *this;
}

const operand* invop::get_inner() const {
    return inner;
}

void invop::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

bool invop::is_leaf() const {
    return false;
}

invop* invop::clone() const {
    return new invop(inner->clone());
}

binop::binop(operand* left, operand* right) {
    this->left = left;
    this->right = right;
}

binop::binop(const binop& orig) {
    this->left = orig.left->clone();
    this->right = orig.right->clone();
}

binop::~binop() {
    delete left;
    delete right;
}

binop& binop::operator=(const binop& other) {
    delete left;
    delete right;
    left = other.left->clone();
    right = other.right->clone();
    return *this;
}

bool binop::is_leaf() const {
    return false;
}

const operand* binop::get_left() const {
    return left;
}

const operand* binop::get_right() const {
    return right;
}

addop::addop(operand* left, operand* right) : binop(left, right) {
}

addop::addop(const addop& orig) : binop(orig) {
}

addop::~addop() {
}

void addop::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

addop* addop::clone() const {
    return new addop(get_left()->clone(), get_right()->clone());
}

subop::subop(operand* left, operand* right) : binop(left, right) {
}

subop::subop(const subop& orig) : binop(orig) {
}

subop::~subop() {
}

void subop::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

subop* subop::clone() const {
    return new subop(get_left()->clone(), get_right()->clone());
}

mulop::mulop(operand* left, operand* right) : binop(left, right) {
}

mulop::mulop(const mulop& orig) : binop(orig) {
}

mulop::~mulop() {
}

void mulop::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

mulop* mulop::clone() const {
    return new mulop(get_left()->clone(), get_right()->clone());
}

divop::divop(operand* left, operand* right) : binop(left, right) {
}

divop::divop(const divop& orig) : binop(orig) {
}

divop::~divop() {
}

void divop::visit(opvisitor* visitor) const {
    visitor->visit(this);
}

divop* divop::clone() const {
    return new divop(get_left()->clone(), get_right()->clone());
}
