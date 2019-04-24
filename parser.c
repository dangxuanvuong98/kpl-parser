/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  // DONE
    compileConstDecls();
    compileTypeDecls();
    compileVarDecls();
    compileSubDecls();
    eat(KW_BEGIN);
    while (true) {
        compileStatement();
        if (lookAhead->tokenType == KW_END) {
            break;
        }
    }
    eat(KW_END);
  assert("Block parsed!");
}

void compileConstDecls(void) {
  // TODO
}

void compileTypeDecls(void) {
  // TODO
}

void compileVarDecls(void) {
  // TODO
}

void compileSubDecls(void) {
    // DONE
    assert("Parsing sub routines ....")
    while (true) {
        switch (lookAhead->tokenType) {
            case KW_FUNCTION:
                compileFuncDecl();
                break;
            case KW_PROCEDURE:
                compileProcDecl();
                break;
            default:
                return;
        }
    }
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // DONE
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParamList();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // DONE
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParamList();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileParamList(void) {
  assert("Parsing param list ....");
  // TODO
  assert("Param list parsed ....");
}

void compileUnsignedConstant(void) {
  // TODO
}

void compileConstant(void) {
  // TODO
}

void compileType(void) {
  // TODO
}

void compileBasicType(void) {
  // TODO
}
void compileStatement(void) {
  // TODO
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  // TODO
  assert("If statement parsed ....");
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  assert("For statement parsed ....");
}

void compileVariable(void) {
  // TODO
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  assert("Expression parsed");
}

void compileTerm(void) {
  // TODO
}

void compileFactor(void) {
  // TODO
}

void compileCondition(void) {
  // TODO
}

void compileIndentifier(void) {
  // TODO
}

void compileUnsignedInteger(void) {
  // TODO
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
