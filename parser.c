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

void compileIndexes(void) {
  // DONE - Ngoc
  assert("Parsing indexes ....");
  while ( lookAhead->tokenType == SB_LSEL ) {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
  };
  assert("Indexes parsed!");
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
	if (lookAhead->tokenType == KW_CONST) {
		compileConstDecls();
	}
	if (lookAhead->tokenType == KW_TYPE) {
		compileTypeDecls();
	}
	if (lookAhead->tokenType == KW_VAR) {
		compileVarDecls();
	}
	compileSubDecls();
	eat(KW_BEGIN);
    while (1) {
        compileStatement();
        if (lookAhead->tokenType == SB_SEMICOLON) {
            eat(SB_SEMICOLON);
        } else {
            break;
        };
    }
    eat(KW_END);
  assert("Block parsed!");
}

void compileConstDecls(void) {
  // DONE - Vuong
  assert("Parsing constant declares ....");
  eat(KW_CONST);
  do {
	  eat(TK_IDENT);
	  eat(SB_EQ);
	  compileConstant();
	  eat(SB_SEMICOLON);
  } while (lookAhead->tokenType == TK_IDENT);
  assert("Constant declares parsed ....");
}

void compileTypeDecls(void) {
  // DONE - Vuong
  eat(KW_TYPE);
  do {
	  eat(TK_IDENT);
	  eat(SB_EQ);
	  compileType();
	  eat(SB_SEMICOLON);
  } while (lookAhead->tokenType == TK_IDENT);
  assert("Type declares parsed ....");
}

void compileVarDecls(void) {
  // DONE - Vuong
  assert("Parsing variable declares ....");
  eat(KW_VAR);
  do {
	  eat(TK_IDENT);
	  eat(SB_COLON);
	  compileType();
	  eat(SB_SEMICOLON);
  } while (lookAhead->tokenType == TK_IDENT);
  assert("Variable declares parsed ....");
}

void compileSubDecls(void) {
    // DONE - Vuong
    assert("Parsing sub routines ....");
    while (lookAhead->tokenType == KW_FUNCTION || lookAhead->tokenType == KW_PROCEDURE) {
        if (lookAhead->tokenType == KW_FUNCTION) {
            compileFuncDecl();
        } else {
            compileProcDecl();
        }
    };
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
  compileBasicType();
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
  do {
      if (lookAhead->tokenType == KW_VAR) {
          eat(KW_VAR);
      }
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      if (lookAhead->tokenType != SB_SEMICOLON) {
          if (lookAhead->tokenType != SB_RPAR) {
              error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
          }
          break;
      };
      eat(SB_SEMICOLON);
  } while (lookAhead->tokenType != SB_RPAR);
  eat(SB_RPAR);
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
      switch (lookAhead->tokenType) {
          case TK_IDENT:
              eat(TK_IDENT);
              break;
          case TK_NUMBER:
              eat(TK_NUMBER);
              break;
          default:
              error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
              break;
      }
  }
  assert("Constant parsed!");
}

void compileType(void) {
  // DONE - Vuong
  assert("Parsing a type ....");
  switch (lookAhead->tokenType) {
      case TK_IDENT:
          compileIdentifier();
          break;
      case KW_ARRAY:
          eat(KW_ARRAY);
          eat(SB_LSEL);
          compileUnsignedInteger();
          eat(SB_RSEL);
          eat(KW_OF);
          compileType();
          break;
      case KW_INTEGER:
      case KW_CHAR:
          compileBasicType();
          break;
      default:
          error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
          break;
  }
}

void compileBasicType(void) {
  // DONE - Vuong
  switch (lookAhead->tokenType) {
      case KW_INTEGER:
          eat(KW_INTEGER);
          break;
      case KW_CHAR:
          eat(KW_CHAR);
          break;
      default:
          error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
          break;
  }
}
void compileStatement(void) {
  // DONE - Vuong
  assert("Parsing a statement ....");
  switch (lookAhead->tokenType) {
      case TK_IDENT:
          compileAssignSt();
          break;
      case KW_CALL:
          compileCallSt();
          break;
      case KW_BEGIN:
          compileGroupSt();
          break;
      case KW_IF:
          compileIfSt();
          break;
      case KW_WHILE:
          compileWhileSt();
          break;
      case KW_FOR:
          compileForSt();
          break;
      case SB_SEMICOLON:
      case KW_END:
      case KW_ELSE:
          break;
      default:
          error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
          break;
  }
  assert("Statement parsed!");
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // DONE - Ngoc
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL) {
      compileIndexes();
  }
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // DONE - Vuong
  eat(KW_CALL);
  compileIdentifier();
  if ( lookAhead->tokenType == SB_LPAR ) {
    compileArguments();
  }
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // DONE - Vuong
  eat(KW_BEGIN);
  compileStatement();
  while (1) {
      if (lookAhead->tokenType == SB_SEMICOLON) {
          eat(SB_SEMICOLON);
      } else {
          break;
      }
      compileStatement();
  }
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  // DONE - Vuong
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) {
      eat(KW_ELSE);
      compileStatement();
  };

  assert("If statement parsed ....");
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // DONE - Vuong
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // DONE - Vuong
  eat(KW_FOR);
  compileIdentifier();
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileVariable(void) {
  // DONE - Vuong 
  compileIdentifier();
  while (1) {
      if (lookAhead->tokenType == SB_LSEL) {
          eat(SB_LSEL);
      } else {
          break;
      }
      compileExpression();
      eat(SB_RSEL);
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  // DONE - Vuong 
  if (lookAhead->tokenType == SB_PLUS) {
      eat(SB_PLUS);
  } else if (lookAhead->tokenType == SB_MINUS) {
      eat(SB_MINUS);
  }
  do {
      compileTerm();
        if (lookAhead->tokenType == SB_PLUS) {
          eat(SB_PLUS);
        } else if (lookAhead->tokenType == SB_MINUS) {
          eat(SB_MINUS);
        } else {
          break;
        }
  } while (1);
  assert("Expression parsed");
}

void compileTerm(void) {
  // DONE - Vuong
    do {
        compileFactor();
        switch (lookAhead->tokenType) {
            case SB_TIMES:
                eat(SB_TIMES);
                break;
            case SB_SLASH:
                eat(SB_SLASH);
                break;
            case SB_PLUS:
            case SB_MINUS:
            case SB_SEMICOLON:
            case KW_END:
            case KW_THEN:
            case KW_DO:
            case SB_RSEL:
            case SB_LT:
            case SB_LE:
            case SB_GT:
            case SB_GE:
            case SB_EQ:
            case SB_NEQ:
            case SB_RPAR:
                break;
            default:
                error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
                break;
        }
    } while (1);
}

void compileFactor(void) {
  // DONE - Ngoc
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
  case TK_CHAR:
      compileUnsignedConstant();
      break;
  case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
  case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType) {
      case SB_LSEL:
          compileIndexes();
          break;
      case SB_LPAR:
          compileArguments();
          break;
      default:
          break;
      }
      break;
  default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileCondition(void) {
  // DONE - Vuong
  compileExpression();
  switch (lookAhead->tokenType) {
      case SB_EQ:
          eat(SB_EQ);
          break;
      case SB_LT:
          eat(SB_LT);
          break;
      case SB_GT:
          eat(SB_GT);
          break;
      case SB_NEQ:
          eat(SB_NEQ);
          break;
      case SB_LE:
          eat(SB_LE);
          break;
      case SB_GE:
          eat(SB_GE);
          break;
      default:
          error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
          break;
  }
  compileExpression();
}

void compileArguments(void) {
  eat(SB_LPAR);
  while (1) {
      compileExpression();
      if (lookAhead->tokenType == SB_COMMA) {
          eat(SB_COMMA);
      } else {
          break;
      }
  }
  eat(SB_RPAR);
}


void compileIdentifier(void) {
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
