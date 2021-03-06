// see License.txt for copyright and terms of use

// Static Printer: A sample Oink tool that shows how to query the AST
// and typesystem; for now it just prints the inheritance graph.  If
// there is something you always wanted a tool to tell you about your
// raw program, implement it as feature here and send it to me.

#ifndef BULLET_H
#define BULLET_H

#include "oink.h"
#include "cc_ast_aux.h"         // LoweredASTVisitor
#include <map>

// LLVM headers need these
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

// Squelch LLVM warnings
#pragma GCC diagnostic ignored "-Wconversion"
#define INT64_C(C)  ((int64_t) C ## LL)
#define UINT64_C(C) ((uint64_t) C ## ULL)
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/PassManager.h>
#include <llvm/CallingConv.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Pass.h>
//#include <llvm/Target/TargetData.h>

class Bullet : public virtual Oink {
  // tor ****
  public:
  Bullet() {}
  
  // methods ****
//   void printIhg_stage();
  void emit_stage();
  void printASTHistogram_stage();
};

struct GenStatementInfo {
  GenStatementInfo() {
    breakTarget = continueTarget = NULL;
  }
  llvm::BasicBlock* breakTarget;
  llvm::BasicBlock* continueTarget;
};

class CodeGenASTVisitor : public ASTVisitor {
  // The LLVM context
  llvm::LLVMContext& context;
  // LLVM information about the target
  //llvm::TargetData TD;
  llvm::Function* currentFunction;
  // The alloca instruction insertion point
  llvm::Instruction* allocaInsertPt;
  std::map<Variable*, llvm::Value*> variables;

  public:
  llvm::Module *mod;
  LoweredASTVisitor loweredVisitor; // use this as the argument for traverse()

  CodeGenASTVisitor();
  virtual ~CodeGenASTVisitor() {}

  llvm::AllocaInst *createTempAlloca(const llvm::Type *ty, const char *name);
  const llvm::Type* typeToLlvmType(Type *t);

  llvm::BasicBlock* genStatement(llvm::BasicBlock* currentBlock, Statement *obj, GenStatementInfo info);
  llvm::Value* expressionToValue(llvm::BasicBlock* currentBlock, Expression *obj);
  llvm::Value* expressionToLvalue(llvm::BasicBlock* currentBlock, Expression *obj);
  llvm::Value* fullExpressionToValue(llvm::BasicBlock* currentBlock, FullExpression *obj);
  llvm::Value* intToBoolValue(llvm::BasicBlock* currentBlock, llvm::Value *intValue);
  llvm::Value* condToValue(llvm::BasicBlock* currentBlock, Condition *obj);
  void moveArgsToStack(llvm::BasicBlock* entryBlock, FunctionType *type, llvm::Function* func);

  virtual bool visitTranslationUnit(TranslationUnit *obj);
  virtual void postvisitTranslationUnit(TranslationUnit *obj);
  virtual bool visitTopForm(TopForm *obj);
  virtual void postvisitTopForm(TopForm *obj);
  virtual bool visitFunction(Function *obj);
  virtual bool visitMemberInit(MemberInit *obj);
  virtual void postvisitMemberInit(MemberInit *obj);
  virtual bool visitDeclaration(Declaration *obj);
  virtual void postvisitDeclaration(Declaration *obj);
  virtual bool visitASTTypeId(ASTTypeId *obj);
  virtual void postvisitASTTypeId(ASTTypeId *obj);
  virtual bool visitPQName(PQName *obj);
  virtual void postvisitPQName(PQName *obj);
  virtual bool visitTypeSpecifier(TypeSpecifier *obj);
  virtual void postvisitTypeSpecifier(TypeSpecifier *obj);
  virtual bool visitBaseClassSpec(BaseClassSpec *obj);
  virtual void postvisitBaseClassSpec(BaseClassSpec *obj);
  virtual bool visitEnumerator(Enumerator *obj);
  virtual void postvisitEnumerator(Enumerator *obj);
  virtual bool visitMemberList(MemberList *obj);
  virtual void postvisitMemberList(MemberList *obj);
  virtual bool visitMember(Member *obj);
  virtual void postvisitMember(Member *obj);
  virtual bool visitDeclarator(Declarator *obj);
  virtual void postvisitDeclarator(Declarator *obj);
  virtual bool visitIDeclarator(IDeclarator *obj);
  virtual void postvisitIDeclarator(IDeclarator *obj);
  virtual bool visitExceptionSpec(ExceptionSpec *obj);
  virtual void postvisitExceptionSpec(ExceptionSpec *obj);
  virtual bool visitOperatorName(OperatorName *obj);
  virtual void postvisitOperatorName(OperatorName *obj);
  virtual bool visitCondition(Condition *obj);
  virtual void postvisitCondition(Condition *obj);
  virtual bool visitHandler(Handler *obj);
  virtual void postvisitHandler(Handler *obj);
  virtual bool visitArgExpression(ArgExpression *obj);
  virtual void postvisitArgExpression(ArgExpression *obj);
  virtual bool visitArgExpressionListOpt(ArgExpressionListOpt *obj);
  virtual void postvisitArgExpressionListOpt(ArgExpressionListOpt *obj);
  virtual bool visitInitializer(Initializer *obj);
  virtual void postvisitInitializer(Initializer *obj);
  virtual bool visitTemplateDeclaration(TemplateDeclaration *obj);
  virtual void postvisitTemplateDeclaration(TemplateDeclaration *obj);
  virtual bool visitTemplateParameter(TemplateParameter *obj);
  virtual void postvisitTemplateParameter(TemplateParameter *obj);
  virtual bool visitTemplateArgument(TemplateArgument *obj);
  virtual void postvisitTemplateArgument(TemplateArgument *obj);
  virtual bool visitNamespaceDecl(NamespaceDecl *obj);
  virtual void postvisitNamespaceDecl(NamespaceDecl *obj);
  virtual bool visitFullExpressionAnnot(FullExpressionAnnot *obj);
  virtual void postvisitFullExpressionAnnot(FullExpressionAnnot *obj);
  virtual bool visitASTTypeof(ASTTypeof *obj);
  virtual void postvisitASTTypeof(ASTTypeof *obj);
  virtual bool visitDesignator(Designator *obj);
  virtual void postvisitDesignator(Designator *obj);
  virtual bool visitAttributeSpecifierList(AttributeSpecifierList *obj);
  virtual void postvisitAttributeSpecifierList(AttributeSpecifierList *obj);
  virtual bool visitAttributeSpecifier(AttributeSpecifier *obj);
  virtual void postvisitAttributeSpecifier(AttributeSpecifier *obj);
  virtual bool visitAttribute(Attribute *obj);
  virtual void postvisitAttribute(Attribute *obj);
};

#endif // BULLET_H
