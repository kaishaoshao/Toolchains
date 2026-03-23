#include <iostream>
#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <string>
#include <z3++.h>

using namespace llvm;

struct ExecutionState {
  z3::context &ctx;
  std::map<Value *, z3::expr> symbolics;
  std::map<Value *, z3::expr> memory;

  ExecutionState(z3::context &c) : ctx(c) {}

  z3::expr getExpr(Value *v) {
    auto it = symbolics.find(v);
    if (it != symbolics.end())
      return it->second;

    if (auto *CI = dyn_cast<ConstantInt>(v))
      return ctx.bv_val(CI->getSExtValue(), 32);

    std::string name = v->hasName() ? v->getName().str()
                                    : "v_" + std::to_string(symbolics.size());
    z3::expr s = ctx.bv_const(name.c_str(), 32);
    // 使用 insert 而不是 []
    symbolics.insert({v, s});
    return s;
  }
};

class MiniInterpreter : public InstVisitor<MiniInterpreter> {
public:
  ExecutionState &state;
  MiniInterpreter(ExecutionState &s) : state(s) {}

  void visitStoreInst(StoreInst &I) {
    // 使用 insert_or_assign 替代 []
    state.memory.insert_or_assign(I.getPointerOperand(),
                                  state.getExpr(I.getValueOperand()));
  }

  void visitLoadInst(LoadInst &I) {
    auto it = state.memory.find(I.getPointerOperand());
    if (it != state.memory.end()) {
      state.symbolics.insert_or_assign(&I, it->second);
    }
  }

  void visitICmpInst(ICmpInst &I) {
    z3::expr lhs = state.getExpr(I.getOperand(0));
    z3::expr rhs = state.getExpr(I.getOperand(1));
    z3::expr res(state.ctx);

    if (I.getPredicate() == ICmpInst::ICMP_SGT)
      res = (lhs > rhs);
    else if (I.getPredicate() == ICmpInst::ICMP_SLE)
      res = (lhs <= rhs);
    else if (I.getPredicate() == ICmpInst::ICMP_SLT)
      res = (lhs < rhs);
    else
      res = (lhs == rhs);

    state.symbolics.insert_or_assign(&I, res);
  }

  void visitBinaryOperator(BinaryOperator &I) {
    z3::expr lhs = state.getExpr(I.getOperand(0));
    z3::expr rhs = state.getExpr(I.getOperand(1));

    if (I.getOpcode() == Instruction::Sub) {
      state.symbolics.insert_or_assign(&I, lhs - rhs);
    } else if (I.getOpcode() == Instruction::Add) {
      state.symbolics.insert_or_assign(&I, lhs + rhs);
    }
  }
};
int main(int argc, char **argv) {
  if (argc < 2)
    return 1;
  LLVMContext llvm_ctx;
  SMDiagnostic err;
  z3::context z3_ctx;
  auto mod = parseIRFile(argv[1], err, llvm_ctx);
  if (!mod)
    return 1;

  for (auto &F : *mod) {
    if (F.isDeclaration())
      continue;
    std::cout << "\n[Analyze] 函数: " << F.getName().str() << std::endl;

    // 1. 初始化输入参数
    ExecutionState baseState(z3_ctx);
    for (auto &Arg : F.args())
      baseState.getExpr(&Arg);

    // 2. 遍历 BasicBlocks
    for (BasicBlock &BB : F) {
      MiniInterpreter(baseState).visit(BB);

      if (auto *BI = dyn_cast<BranchInst>(BB.getTerminator())) {
        if (BI->isConditional()) {
          z3::expr cond = baseState.getExpr(BI->getCondition());

          // 分别尝试 True 和 False 路径
          bool dirs[] = {true, false};
          for (bool d : dirs) {
            z3::solver s(z3_ctx);
            s.add(d ? cond : !cond);
            std::cout << "--- 路径 (" << (d ? "a > b" : "a <= b") << "): ";
            // 在 runSolver 或 main 的打印循环中修改：
            if (s.check() == z3::sat) {
              z3::model m = s.get_model();
              std::cout << "可达 ---" << std::endl;

              for (auto &Arg : F.args()) {
                // 1. 获取参数名，如果没有则显示索引
                std::string name =
                    Arg.hasName() ? Arg.getName().str()
                                  : "arg_" + std::to_string(Arg.getArgNo());

                // 2. 获取 Z3 表达式的值
                z3::expr res = m.eval(baseState.getExpr(&Arg), true);

                // 3. 将位向量转为 64 位整数，再转为 int32 以处理负数
                int64_t val = res.as_int64();
                int32_t signed_val = (int32_t)val;

                std::cout << "  " << name << " = " << signed_val << std::endl;
              }
            } else {
              std::cout << "不可达 ---" << std::endl;
            }
          }
        }
      }
    }
  }
  return 0;
}