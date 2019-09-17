/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * PluginTest.cpp
 *
 *  Created on: 14.06.2017
 *      Author: philipp
 */

#include <iostream>

#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Gen.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Identity.h>

#include "IFDSSFB901TaintAnalysis.h"
using namespace std;
using namespace psr;

namespace psr {

unique_ptr<IFDSTabulationProblemPlugin>
makeIFDSSFB901TaintAnalysis(LLVMBasedICFG &I, vector<string> EntryPoints) {
  return unique_ptr<IFDSTabulationProblemPlugin>(
      new IFDSSFB901TaintAnalysis(I, EntryPoints));
}

__attribute__((constructor)) void init() {
  cout << "init - IFDSSFB901TaintAnalysis\n";
  IFDSTabulationProblemPluginFactory["ifds_testplugin"] =
      &makeIFDSSFB901TaintAnalysis;
}

__attribute__((destructor)) void fini() {
  cout << "fini - IFDSSFB901TaintAnalysis\n";
}

IFDSSFB901TaintAnalysis::IFDSSFB901TaintAnalysis(LLVMBasedICFG &I,
                                                 vector<string> EntryPoints)
    : IFDSTabulationProblemPlugin(I, createZeroValue(), EntryPoints) {}

const FlowFact *IFDSSFB901TaintAnalysis::createZeroValue() {
  // create a special value to represent the zero value!
  return new FlowFactWrapper<const llvm::Value *>(LLVMZeroValue::getInstance());
}

bool IFDSSFB901TaintAnalysis::isZeroValue(const FlowFact *d) const {
  const FlowFactWrapper<const llvm::Value *> *d1 =
      static_cast<const FlowFactWrapper<const llvm::Value *> *>(d);
  return LLVMZeroValue::getInstance()->isLLVMZeroValue(d1->get());
}

void IFDSSFB901TaintAnalysis::printDataFlowFact(std::ostream &os,
                                                const FlowFact *d) const {
  const FlowFactWrapper<const llvm::Value *> *d1 =
      static_cast<const FlowFactWrapper<const llvm::Value *> *>(d);
  os << llvmIRToString(d1->get());
}

shared_ptr<FlowFunction<const FlowFact *>>
IFDSSFB901TaintAnalysis::getNormalFlowFunction(const llvm::Instruction *curr,
                                               const llvm::Instruction *succ) {
  cout << "IFDSSFB901TaintAnalysis::getNormalFlowFunction()\n";
  return Identity<const FlowFact *>::getInstance();
}

shared_ptr<FlowFunction<const FlowFact *>>
IFDSSFB901TaintAnalysis::getCallFlowFunction(const llvm::Instruction *callStmt,
                                             const llvm::Function *destMthd) {
  cout << "IFDSSFB901TaintAnalysis::getCallFlowFunction()\n";
  return Identity<const FlowFact *>::getInstance();
}

shared_ptr<FlowFunction<const FlowFact *>>
IFDSSFB901TaintAnalysis::getRetFlowFunction(const llvm::Instruction *callSite,
                                            const llvm::Function *calleeMthd,
                                            const llvm::Instruction *exitStmt,
                                            const llvm::Instruction *retSite) {
  cout << "IFDSSFB901TaintAnalysis::getRetFlowFunction()\n";
  return Identity<const FlowFact *>::getInstance();
}

shared_ptr<FlowFunction<const FlowFact *>>
IFDSSFB901TaintAnalysis::getCallToRetFlowFunction(
    const llvm::Instruction *callSite, const llvm::Instruction *retSite,
    set<const llvm::Function *> callees) {
  cout << "IFDSSFB901TaintAnalysis::getCallToRetFlowFunction()\n";
  return Identity<const FlowFact *>::getInstance();
}

shared_ptr<FlowFunction<const FlowFact *>>
IFDSSFB901TaintAnalysis::getSummaryFlowFunction(
    const llvm::Instruction *callStmt, const llvm::Function *destMthd) {
  cout << "IFDSSFB901TaintAnalysis::getSummaryFlowFunction()\n";
  return nullptr;
}

map<const llvm::Instruction *, set<const FlowFact *>>
IFDSSFB901TaintAnalysis::initialSeeds() {
  cout << "IFDSSFB901TaintAnalysis::initialSeeds()\n";
  map<const llvm::Instruction *, set<const FlowFact *>> SeedMap;
  SeedMap.insert(std::make_pair(
      &icfg.getMethod("run_service_contrast_cpu")->front().front(),
      set<const FlowFact *>({zeroValue()})));
  return SeedMap;
}

} // namespace psr
