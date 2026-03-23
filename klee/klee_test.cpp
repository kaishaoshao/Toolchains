#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <z3++.h>
#include <map>
#include <iostream>

using namespace llvm;

// 1. 维护 LLVM Value 到 Z3 表达式的映射
struct SymbolicState {
    z3::context &ctx;
    std::map<Value*, z3::expr> values;

    SymbolicState(z3::context &c) : ctx(c) {}

    z3::expr getExpr(Value *v) {
        if (values.count(v)) return values.at(v);
        // 如果是常量，直接转为 Z3 具体值
        if (auto *CI = dyn_cast<ConstantInt>(v)) {
            return ctx.bv_val(CI->getSExtValue(), 32);
        }
        // 否则当作一个新的符号变量处理
        z3::expr s = ctx.bv_const(v->getName().str().c_str(), 32);
        values.insert({v, s});
        return s;
    }
};

// 2. 实现解释器：拦截加法指令
class SimpleSymbolicExec : public InstVisitor<SimpleSymbolicExec> {
    SymbolicState &state;
public:
    SimpleSymbolicExec(SymbolicState &s) : state(s) {}

    void visitBinaryOperator(BinaryOperator &I) {
        if (I.getOpcode() == Instruction::Add) {
            z3::expr lhs = state.getExpr(I.getOperand(0));
            z3::expr rhs = state.getExpr(I.getOperand(1));
            
            // 核心：在 Z3 中建立等式约束
            z3::expr res = lhs + rhs;
            state.values.insert({&I, res});
            
            std::cout << "映射指令: " << I.getName().str() 
                      << " -> " << res << std::endl;
        }
    }
};

int main() {
    z3::context z3_ctx;
    LLVMContext llvm_ctx;
    SymbolicState state(z3_ctx);
    SimpleSymbolicExec exec(state);

    // 构造一个简单的 LLVM IR: %res = add i32 %a, %b
    IRBuilder<> builder(llvm_ctx);
    auto *i32 = builder.getInt32Ty();
    // 模拟两个输入参数 a 和 b
    Argument *a = new Argument(i32, "a");
    Argument *b = new Argument(i32, "b");
    auto *add_inst = cast<BinaryOperator>(builder.CreateAdd(a, b, "res"));

    // 3. 执行符号遍历
    exec.visit(add_inst);

    // 4. 调用求解器：我们想要 res == 100，求 a 和 b 是多少？
    z3::solver solver(z3_ctx);
    z3::expr res_expr = state.getExpr(add_inst);
    solver.add(res_expr == 100);

    if (solver.check() == z3::sat) {
        z3::model m = solver.get_model();
        std::cout << "解出输入: " << std::endl;
        std::cout << "a = " << m.eval(state.getExpr(a)) << std::endl;
        std::cout << "b = " << m.eval(state.getExpr(b)) << std::endl;
    }

    return 0;
}
