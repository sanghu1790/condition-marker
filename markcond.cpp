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
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"

#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
using namespace std;
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "llvm-c/TargetMachine.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm-c/BitWriter.h"
//#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"

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

  // Code to open a file and read the truth values for sequences created.
  int truthvalue;
  int conditionvalue[52];
  int icmpp[52];
  // char a;
  int i = 0, j = 0;
  string line;
  ifstream myfile("seq3.txt", ios_base::in);
  // std::fstream myfile("seq.txt", std::ios_base::in);
  if (myfile.is_open()) {

    while (myfile.good()) {
      myfile >> truthvalue;
      icmpp[i] = truthvalue; // Assigning the truth values from the seq.txt
                             // files in an comparison array to check the truth
                             // and false value
      i++;
    }

    myfile.close();

  } else
    cout << "Unable to open file";

  int icmp = 0;
  llvm::GlobalVariable *kappa;
  for (auto &F : *M) {
    for (llvm::Module::global_iterator GI = M->global_begin(),
                                       GE = M->global_end();
         GI != GE; ++GI) {
      if (GI->getName() == "kappa") {
        GI->dump();
        kappa = GI;
      }
    }
  }
  for (auto &F : *M) {
    for (llvm::Function::iterator b = F.begin(); b != F.end(); ++b) {
      for (llvm::BasicBlock::iterator ins = b->begin(); ins != b->end();
           ++ins) {
        if (llvm::isa<llvm::ICmpInst>(ins)) {
          if (icmp < sizeof(icmpp)) {
            // Instruction *instruction = cmpInst->getNextNode();
            // llvm::CmpInst *cmpInst = llvm::dyn_cast<llvm::CmpInst>(&I);
            // llvm::Instruction *instruction = cmpInst->llvm::getNextNode();
            llvm::errs() << "\n \n" << icmp << "\n \n";
            llvm::errs() << *ins << "\n";
            llvm::errs() << icmpp[icmp] << "\n";

            // I.dump();
            //	 cout << "\n" << I;

            if (llvm::Instruction *i = ins->getNextNode()) {
              if (llvm::BranchInst *bi = llvm::dyn_cast<llvm::BranchInst>(i)) {
                i->dump();
                if (bi->getNumOperands() == 3) {

                  if (icmpp[icmp] != 9) // 9 is a non-boolean value for any
                                        // condition (neither true nor false)
                  {

                    if (icmpp[icmp] == 1) {
                      // llvm::errs() << "heloo sanghu \n "<<
                      // *bi->getOperand(2) << "\n \n";
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
                            llvm::errs() << "FIRST BRANCH: " << label << "\n";
                            llvm::IRBuilder<> IR(
                                llvm::dyn_cast<llvm::BasicBlock>(
                                    bi->getOperand(2)));
                            /*------Logic to insert Kappa++ instructions in
                             * destination basic blocks------*/
                            llvm::BasicBlock *tempblock = b->getNextNode();
                            IR.SetInsertPoint(tempblock->getFirstInsertionPt());
                            llvm::LoadInst *Load = IR.CreateLoad(kappa);
                            llvm::Value *Inc =
                                IR.CreateAdd(IR.getInt32(1), Load);
                            llvm::StoreInst *Store = IR.CreateStore(Inc, kappa);
                            bi->getOperand(2)->dump(); // true branch
                          }
                        }
                      }

                    } else {

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
                            llvm::errs() << "SECOND BRANCH: " << label << "\n";
                            llvm::IRBuilder<> IR(
                                llvm::dyn_cast<llvm::BasicBlock>(
                                    bi->getOperand(1)));
                            /*------Logic to insert Kappa++ instructions in
                             * destination basic blocks------*/
                            llvm::BasicBlock *tempblock1 = b->getNextNode();
                            IR.SetInsertPoint(
                                tempblock1->getFirstInsertionPt());
                            llvm::LoadInst *Load = IR.CreateLoad(kappa);
                            llvm::Value *Inc =
                                IR.CreateAdd(IR.getInt32(1), Load);
                            llvm::StoreInst *Store = IR.CreateStore(Inc, kappa);
                            bi->getOperand(1)->dump(); // false branch
                          }
                        }
                      }
                    }
                  } else {
                    llvm::errs() << "Entered condition value is don't care\n";
                  }
                }
              }
            } else {
              llvm::errs() << "No next node\n";
            }
          }
          icmp++;

          // else
          //{
          // llvm::errs() << "Condition id is
          // invalid \n";
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

  /*To generate the new .ll file to store the updated llvm code*/
  //	for (auto &F: *M)  {
  //		M->dump();
  //	freopen ("myfile9.ll","w",stdout);
  //         printf("sanghu is good boy \n \n");
  //	       llvm::outs()<< *M;
  //       fclose (stdout);

  //		}

//  FILE *f = freopen("tcaschange3.ll", "w", stdout);
//  if (f != NULL) {
//    // llvm::WriteBitcodeToFile(M,llvm::outs());
//
//    // M->dump();
    llvm::outs() << *M;
//    fflush(f);
//    fclose(f);
//  }
  /*ofstream filepredicate;
  filepredicate.open ("predicate.txt");
  //llvm::outs() << *M;
   M->dump();
  filepredicate.close();*/

  return 0;
}

