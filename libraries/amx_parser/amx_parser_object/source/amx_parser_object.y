
%{
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "../include/amx_parser_object_context.h"
%}

%pure-parser
%name-prefix="amx_parser_object_"
%locations
%defines
%error-verbose
%parse-param { amx_parser_object_t* context }
%lex-param { void* scanner  }

%union
{
	int integer;
	char* cptr;
}

%token <integer> OBJECT
%token <integer> SIGNALS
%token <integer> SLOTS
%token <integer> EOF_TOKEN
%token <cptr>    DIGIT
%token <cptr>    STRING
%token <cptr>    MULTILINECOMMENT

%type <integer> stream
%type <integer> object
%type <integer> object_body
%type <integer> signals
%type <integer> slots

%{
        int amx_parser_object_lex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);

        void amx_parser_object_error(YYLTYPE* locp, amx_parser_object_t* context, const char* err)
	{
		(void)context;
                printf("ERROR: line %d: %s\n", locp->first_line, err);
	}

	#define scanner context->scanner
%}

%start stream

%%



stream: object stream                           { amx_parser_object_printf("object stream detected \n");}
      | object                                  { amx_parser_object_printf("object detected \n");}
      | MULTILINECOMMENT stream                 { amx_parser_object_switch_parser(context, "MultiLineComment",$1);}
      | MULTILINECOMMENT                        { amx_parser_object_switch_parser(context, "MultiLineComment",$1);}
      | EOF_TOKEN                               { amx_parser_object_printf("EOF detected \n");}
      ;

object: OBJECT STRING '{' object_body '}'       { amx_parser_object_printf("OBJECT detected %s\n",$2);}
      ;

object_body: SIGNALS ':' signals object_body    { amx_parser_object_printf("SIGNAL DETECTED WITH BODY\n");}
	   | SLOTS ':' slots object_body        { amx_parser_object_printf("SLOTS DETECTED WITH BODY\n");}
	   | SIGNALS ':' signals                { amx_parser_object_printf("SIGNALS DETECTED\n");}
	   | SLOTS ':' slots                    { amx_parser_object_printf("SLOT DETECTED\n");}
	   | SIGNALS ':'                        { amx_parser_object_printf("EMPTY SIGNAL\n");}
	   | SLOTS ':'                          { amx_parser_object_printf("EMPTY SLOT\n");}
           ;

signals: STRING ';' signals                    { amx_parser_object_printf("signal: %s\n",$1);}
       | STRING ';'                            { amx_parser_object_printf("signal: %s\n",$1);}
       ;

slots: STRING ';' slots                        { amx_parser_object_printf("slot: %s\n",$1);}
     | STRING ';'                              { amx_parser_object_printf("slot: %s\n",$1);}
     ;

