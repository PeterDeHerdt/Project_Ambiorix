%option reentrant
%option prefix="amx_parser_object_"
%option bison-bridge
%option bison-locations
%option noyywrap
%option yylineno

%{

	#include <stdio.h>
	#include <string.h>
	#include <sys/types.h>
	#include "../include/amx_parser_object_context.h"
        #include "amx_parser_object.tab.h"

        #define YY_EXTRA_TYPE amx_parser_object_t*
        #define YY_USER_ACTION yylloc->first_line = yylineno;

        #define YY_INPUT(buf,result,max_size) \
        { \
                errno=0; \
		while ( (result = read(yyextra->fd,buf, max_size)!=0)) \
                        { \
			if( errno != EAGAIN) \
                                { \
                                YY_FATAL_ERROR( "input in flex scanner failed" ); \
                                break; \
                                } \
			errno=0; \
                        } \
        }
%}

%x MULTI_LINE_COMMENT SINGLE_LINE_COMMENT

NEWLINE  \r?\n

%%

"/*"                                   { BEGIN( MULTI_LINE_COMMENT ); }
<MULTI_LINE_COMMENT>"*"                { yymore(); }
<MULTI_LINE_COMMENT>[^*\n]+            { yymore(); }
<MULTI_LINE_COMMENT>[^*\n]*{NEWLINE}   { yymore(); }
<MULTI_LINE_COMMENT>"*/"               { yylval->cptr = calloc(1,yyleng+1); memcpy(yylval->cptr ,yytext,yyleng-2);; BEGIN(INITIAL); return MULTILINECOMMENT;}

"//"                                   { BEGIN( SINGLE_LINE_COMMENT ); }
<SINGLE_LINE_COMMENT>{NEWLINE}         { BEGIN(INITIAL); }
<SINGLE_LINE_COMMENT>"*"               {}
<SINGLE_LINE_COMMENT>[^*\n]+           {}

[ \t]                     {}
[\n]                      {}

<<EOF>>                   { yylval->integer = token_eof;  yyterminate(); return EOF_TOKEN; }

object                    { yylval->integer = token_object;  return OBJECT; }
signals                   { yylval->integer = token_signals; return SIGNALS; }
slots                     { yylval->integer = token_slots;   return SLOTS; }
[[:digit:]]+              { yylval->cptr  = calloc(1,yyleng+1); memcpy(yylval->cptr ,yytext,yyleng); return DIGIT; }
-[[:digit:]]+             { yylval->cptr  = calloc(1,yyleng+1); memcpy(yylval->cptr ,yytext,yyleng); return DIGIT; }
[a-zA-Z0-9\-_]+           { yylval->cptr  = calloc(1,yyleng+1); memcpy(yylval->cptr ,yytext,yyleng); return STRING; }

\n|.                      { return yytext[0]; }

%%

void amx_parser_object_create_lex(amx_parser_object_t *object) 
{
	yylex_init(&object->scanner);
	yyset_extra(object, object->scanner);
}

void amx_parser_object_destroy_lex(void *scanner) 
{
	amx_parser_object_lex_destroy(scanner);
}


