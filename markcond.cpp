// Mark conditions for MC/DC computation
//
// Copyright 2017 National University of Singapore
// See LICENSE.md for the license information.

#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"

#include <set>
#include <vector>
#include <iostream>
using namespace std;
#include "string"
/*
 * Main.
 */
int main(int argc, char **argv)
{
	if (argc < 2) {
		llvm::errs() << "Filename unspecified\n";
		return 1;
	}

	llvm::LLVMContext &Context = llvm::getGlobalContext();
	llvm::SMDiagnostic Err;
	llvm::Module *M = ParseIRFile(argv[1], Err, Context);

	if (M == 0) {
		llvm::errs() << "ERROR: failed to load " << argv[0] << "\n";
		return 1;
	}
	int icmp=0;
	int icmpp[10]={false,true,false,true,false,false,true};

	/*for (auto &F: *M) {
	  for (auto &BB: F) {
		for (auto &I: BB) {


			     if(llvm::isa<llvm::ICmpInst>(I))
			     {
				  icmp++;
                 if(icmp==4)
                 {
                	// Instruction *instruction = cmpInst->getNextNode();
                	 //llvm::CmpInst *cmpInst = llvm::dyn_cast<llvm::CmpInst>(&I);
                	 //llvm::Instruction *instruction = cmpInst->llvm::getNextNode();
                	 I.dump();

                	// I.dump();
                	 //	 cout << "\n" << I;
			     llvm::errs() << "\n \n \n" << icmp;

                 }
                 }
			    // if(llvm::isa<llvm::BranchInst>(I))
			     //{
			    	// I.dump();
			    // }





		}

	  }
  }*/

	for (auto &F: *M)  {
		for(llvm::Function::iterator b = F.begin(); b != F.end(); ++b){
			for(llvm::BasicBlock::iterator ins = b->begin(); ins != b->end(); ++ins){
				if(llvm::isa<llvm::ICmpInst>(ins))
				{
					if(icmp==2)
					{
						// Instruction *instruction = cmpInst->getNextNode();
						//llvm::CmpInst *cmpInst = llvm::dyn_cast<llvm::CmpInst>(&I);
						//llvm::Instruction *instruction = cmpInst->llvm::getNextNode();
						llvm::errs() << "\n \n" << icmp << "\n \n";;
						llvm::errs() << *ins << "\n";
						llvm::errs() << icmpp[icmp] << "\n";

						// I.dump();
						//	 cout << "\n" << I;

						if (llvm::Instruction *i = ins->getNextNode()) {
							if (llvm::BranchInst *bi = llvm::dyn_cast<llvm::BranchInst>(i)) {
								i->dump();
								if (bi->getNumOperands() == 3) {
									{
										std::string buf;
										llvm::raw_string_ostream stream(buf);
										bi->getOperand(1)->print(stream);
										stream.flush();
										std::size_t s = buf.find('\n');
										if (s != std::string::npos) {
											if (s == 0) {
												buf = buf.substr(1, buf.size() - 1);
												s = buf.find('\n');
												if (s != std::string::npos) {
													std::string label = buf.substr(0, s);
													llvm::errs() << "FIRST BRANCH: " << label << "\n";

												}
											}
										}
									}
									{
										std::string buf;
										llvm::raw_string_ostream stream(buf);
										bi->getOperand(2)->print(stream);
										stream.flush();
										std::size_t s = buf.find('\n');
										if (s != std::string::npos) {
											if (s == 0) {
												buf = buf.substr(1, buf.size() - 1);
												s = buf.find('\n');
												if (s != std::string::npos) {
													std::string label = buf.substr(0, s);
													llvm::errs() << "SECOND BRANCH: " << label << "\n";

												}
											}
										}
									}
								}
							}
						} else {
							llvm::errs() << "No next node\n";
						}

					}
					icmp++;

					//else
					//{
					// llvm::errs() << "Condition id is invalid \n";
					//}
					//}
					// if(llvm::isa<llvm::BranchInst>(I))
					//{
					// I.dump();
					// }
				}




			}
		}
	}


	return 0;
}

