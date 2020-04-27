#include "token-list.h"

/* keyword list */
struct KEY key[KEYWORDSIZE] = { { "and", TAND }, { "array", TARRAY }, { "begin",
TBEGIN }, { "boolean", TBOOLEAN }, { "break", TBREAK }, { "call", TCALL }, {
		"char", TCHAR }, { "div", TDIV }, { "do", TDO }, { "else", TELSE }, {
		"end", TEND }, { "false", TFALSE }, { "if",
TIF }, { "integer", TINTEGER }, { "not", TNOT }, { "of", TOF }, { "or", TOR }, {
		"procedure", TPROCEDURE }, { "program", TPROGRAM }, { "read", TREAD }, {
		"readln", TREADLN }, { "return", TRETURN }, { "then", TTHEN }, { "true",
		TTRUE }, { "var", TVAR }, { "while",
TWHILE }, { "write", TWRITE }, { "writeln", TWRITELN } };

/* Token counter */
int numtoken[NUMOFTOKEN + 1];

/* string of each token */
char *tokenstr[NUMOFTOKEN + 1] = { "", "NAME", "program", "var", "array", "of",
		"begin", "end", "if", "then", "else", "procedure", "return", "call",
		"while", "do", "not", "or", "div", "and", "char", "integer", "boolean",
		"readln", "writeln", "true", "false", "NUMBER", "STRING", "+", "-", "*",
		"=", "<>", "<", "<=", ">", ">=", "(", ")", "[", "]", ":=", ".", ",",
		":", ";", "read", "write", "break" };

int main(int nc, char *np[]) {
	if (nc < 2) {
		printf("File name id not given.\n");
		return 0;
	}
	if (init_scan(np[1]) < 0) {
		printf("File %s can not open.\n", np[1]);
		return 0;
	}
	int filelength = strlen(np[1]);
	char resultfilename[filelength];
	strcpy(resultfilename, np[1]);
	resultfilename[filelength - 3] = 'c';
	resultfilename[filelength - 2] = 's';
	resultfilename[filelength - 1] = 'l';
	if ((cslp = fopen(resultfilename, "w")) == NULL) {
		printf("File %s can not open\n",resultfilename);
		return 0;
	}

	/* 作成する部分：トークンカウント用の配列？を初期化する */
	memset(numtoken, 0, NUMOFTOKEN + 1);
	globalidroot = &global;
	if ((globalidroot->type_next = malloc(sizeof(struct TYPE))) == NULL) {
		printf("ERROR can not allotcate\n");
	}
	localidroot = &local;
	formal_flag = 0;
//	globalidroot=NULL;
//	localidroot=NULL;
	token = scan();
	parse_program();
//	PrintIDTable(&global,&local);
	//printf("finish");
	return 0;
}

int error(char *mes) {
	int line = 0;
	line = get_linenum();
	if (line == 0) {
		line = 1;
	}
	printf("ERROR: %s\n error exist at %d line", mes, line);
	end_scan();
	return ERROR;
}

