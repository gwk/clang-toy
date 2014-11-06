
#include <iostream>
#include <string>
#include <memory>

#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "llvm/Config/config.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Host.h"

using namespace std;
using namespace llvm;

typedef char* Chars;
typedef const char* Chars_const;


#define errFL(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)

// write an error message and then exit.
#define error(fmt, ...) { \
  errFL("error: " fmt, ## __VA_ARGS__); \
  exit(1); \
}

// check that a condition is true; otherwise error.
#define check(condition, fmt, ...) { if (!(condition)) error(fmt, ## __VA_ARGS__); }


#if LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR <= 5
Module*
#else
unique_ptr<Module>
#endif
compile(LLVMContext* context, const vector<Chars_const> compileArgs) {
  vector<Chars_const> args = {
    "-Xclang",
    "-resource-dir",
    "-Xclang",
    "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/6.0",
  };
  for (auto const& s : compileArgs) {
    args.push_back(s);
  }
  const IntrusiveRefCntPtr<clang::CompilerInvocation> invocation =
    clang::createInvocationFromCommandLine(args);

  clang::CompilerInstance compiler;
  compiler.setInvocation(invocation.get());
  
  auto diagConsumer = new clang::TextDiagnosticPrinter(errs(), &compiler.getDiagnosticOpts());
  compiler.createDiagnostics(diagConsumer, true); // shouldOwnClient=true.
  
  clang::EmitLLVMOnlyAction action(context);
  check(compiler.ExecuteAction(action), "ExecuteAction failed");
  return action.takeModule();
}


int run(unique_ptr<Module> module, Chars_const fnName, const vector<string> args) {
  
  check(module, "no module");
  
  Function* entryFn = module->getFunction(fnName);
  check(entryFn, "Source file does not contain a function of the given name");
  
  // Use an EngineBuilder to configure and construct an MCJIT ExecutionEngine.
#if LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR <= 5
  EngineBuilder builder(module.release());
  builder.setUseMCJIT(true);
#else
  const EngineBuilder builder(move(module));
  const SectionMemoryManager mm;
  builder.setMCJITMemoryManager(&mm);
#endif
  string errorStr;
  builder.setErrorStr(&errorStr);
  builder.setEngineKind(EngineKind::JIT);
  builder.setOptLevel(CodeGenOpt::None);
  
  ExecutionEngine* engine = builder.create();
  
  // Call 'finalizeObject' to notify the JIT that we're ready to execute the jitted code,
  // then run the static constructors.
  engine->finalizeObject();
  engine->runStaticConstructorsDestructors(false);
  
  // Pass the args to the jitted function, and capture the result.
  const int result = engine->runFunctionAsMain(entryFn, args, nullptr);
  engine->runStaticConstructorsDestructors(true); // Run the static destructors.
  return result;
}


int compileRun(const vector<Chars_const> compileArgs, Chars_const fnName, const vector<string> args) {

  LLVMContext& context = getGlobalContext();
  unique_ptr<Module> module(compile(&context, compileArgs));
  check(module, "module compile failed");
  cout << "- run\n";
  const int result = run(move(module), fnName, args);
  cout << "- returned " << result << "\n\n";
  return result;
}


int main(int argc, Chars_const argv[]) {
  LLVMLinkInMCJIT();
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  compileRun({"-Icjit-demo", "cjit-demo/testc.c"    }, "entry", {"Hello, World from C!\n"  });
  compileRun({"-Icjit-demo", "cjit-demo/testcpp.cpp"}, "entry", {"Hello, World from C++!\n"});
  return 0;
}


char* cppCallout(char* arg) {
  return arg;
}


extern "C" {
  char* cCallout(char* arg) {
    return arg;
  }
}
