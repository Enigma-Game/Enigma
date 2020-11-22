/*
* lexerproxy.c
* lexer proxy for Lua parser -- implements assert removal
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 11 May 2007 11:18:57
* This code is hereby placed in the public domain.
* Add <<#include "proxy.c">> just before the definition of luaX_next in llex.c
*/

#include <string.h>
#include <stdbool.h>

bool lua_evaluate_assert = false;

static int nexttoken(LexState *ls, SemInfo *seminfo)
{
 for (;;) {
	int n;
	int t=llex(ls,seminfo);
	if (t!=TK_NAME) return t;
	if (lua_evaluate_assert || (strcmp(getstr(seminfo->ts),"assert_type")!=0
			&& strcmp(getstr(seminfo->ts),"assert_bool")!=0))
		return t;
	t=nexttoken(ls,&ls->lookahead.seminfo);
	if (t!='(') {
		ls->lookahead.token = t;
		return TK_NAME;
	}
	for (n=1; n>0; ) {
		t=llex(ls,seminfo);
		if (t==TK_EOS) return t;
		if (t=='(') n++;
		if (t==')') n--;
	}
 }
}

#define llex nexttoken

