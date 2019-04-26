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
  // DONE - Vuong
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
  // DONE - Vuong
  assert("Parsing constant declares ....");
  if (lookAhead->tokenType == KW_CONST) {
      eat(KW_CONST);
      while (true) {
          eat(TK_IDENT);
          eat(SB_EQ);
          compileConstant();
          eat(SB_SEMICOLON);
          if (lookAhead->tokenType != TK_IDENT) {
              break;
          }
      }
  }
  assert("Constant declares parsed ....");
}

void compileTypeDecls(void) {
  // DONE - Vuong
  if (lookAhead->tokenType == KW_TYPE) {
      eat(KW_TYPE);
      while (true) {
          eat(TK_IDENT);
          eat(SB_EQ);
          compileType();
          eat(SB_SEMICOLON);
          if (lookAhead->tokenType != TK_IDENT) {
              break;
          }
      }
  }
  assert("Type declares parsed ....");
}

void compileVarDecls(void) {
  // DONE - Vuong
  assert("Parsing variable declares ....")
  if (lookAhead->tokenType == KW_VAR) {
      eat(KW_VAR);
      while (true) {
          eat(TK_IDENT);
          eat(SB_COLON);
          compileType();
          eat(SB_SEMICOLON);
          if (lookAhead->tokenType != TK_IDENT) {
              break;
          }
      }
  }
  assert("Variable declares parsed ....");
}

void compileSubDecls(void) {
    // DONE - Vuong
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
  // DONE - Vuong
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LPAR) {
    compileParamList();
  }
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // DONE - Vuong
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LPAR) {
    compileParamList();
  }
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileParamList(void) {
  assert("Parsing param list ....");
  // DONE - Vuong
  eat(SB_LPAR);
  while (true) {
      if (lookAhead->tokenType = KW_VAR) {
          eat(KW_VAR);
      }
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      if (lookAhead->tokenType != SB_SEMICOLON) {
        break;
      }
      eat(SB_SEMICOLON);
  }
  assert("Param list parsed ....");
}

void compileUnsignedConstant(void) {
  // DONE - Vuong
  switch(lookAhead->tokenType) {
      case TK_NUMBER:
          eat(TK_NUMBER);
          break;
      case TK_IDENT:
          eat(TK_IDENT);
          break;
      case TK_CHAR:
          eat(TK_CHAR);
          break;
      default:
          error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
          break;
  }
}

void compileConstant(void) {
  // DONE - Vuong
  assert("Parsing a constant ....");
  if (lookAhead->tokenType == TK_CHAR) {
      eat(TK_CHAR);
  } else {
      switch (lookAhead->tokenType) {
          case SB_MINUS:
              eat(SB_MINUS);
              break;
          case SB_PLUS:
              eat(SB_PLUS);
              break;
          default:
              break;
      }
      compileIndentifier();
  }
  assert("Constant parsed!");
}

void compileType(void) {
  // DOING - Vuong
  assert("Parsing a type ....");
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
  // DONE - Vuong
  eat(TK_IDENT);
}

void compileUnsignedInteger(void) {
  // DONE - Vuong
  eat(TK_NUMBER);
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
