#include "token-list.h"

int block();
int variable_declaration();
int variable_names();
int type();
int standard_type();
int array_type();
int subprogram_declaration();
int procedure_name();
int formal_parameters();
int compound_statement();
int statement();
int condition_statement();
int iteration_statement();
int exit_statement();
int call_statement();
int expressions();
int return_statement();
int assignment_statement();
int left_part();
int variabble();
int expression();
int simple_expression();
int term();
int factor();
int constant();
int multiplicative_operator();
int additive_operator();
int relational_operator();
int input_statement();
int output_statement();
int output_format();
int empty_statement();
void pp_token();

void add_newNODE(char *name, char *procedure_name, int defineline) {
	check_scope(scope_keyword);
	if (subpro_flag == LOCAL) {
		if ((globalidroot->next = malloc(sizeof(struct ID))) == NULL) {
			printf("ERROR can not malloc\n");
		} else {
			globalidroot = globalidroot->next;
			strcpy(globalidroot->name, name);
			strcpy(globalidroot->procedure_name, procedure_name);
			globalidroot->defineline = defineline;
		}
		if ((globalidroot->type_next = malloc(sizeof(struct TYPE))) == NULL) {
			printf("ERROR can not malloc\n");
		}
	} else if (subpro_flag == PROC) {
		if ((localidroot->next = malloc(sizeof(struct ID))) == NULL) {
			printf("ERROR can not malloc\n");
		} else {
			localidroot = localidroot->next;
			strcpy(localidroot->name, name);
			strcpy(localidroot->procedure_name, procedure_name);
			localidroot->defineline = defineline;
			if (formal_flag) {
				localidroot->ispara = 1;
			} else {
				localidroot->ispara = 0;
			}
		}
	}
}
int expflag=0;
int adrflag = 0;
static int num_while = 0;

int parse_program() {
	if (token != TPROGRAM) {
		return (error("Keyword'program'is not found"));
	}
	token = scan();
	if (token != TNAME)
		return (error("Program name is not found"));
	token = scan();
	if (token != TSEMI)
		return (error("Semicolon is not found"));
	printstart();
	token = scan();
	if (block() == ERROR)
		return (ERROR);
	if (token != TDOT) {
		return (error("Period is not at the end of program"));
	}
	token = scan();
	printret();
	printexception();
	printlibrary();
	return (NORMAL);
}

int block() {
	strcpy(tmp_procedure_name, "NULL");

	while (token == TVAR || token == TPROCEDURE) {
		if (token == TVAR) {
			scope_keyword = "block";
			if (variable_declaration() == ERROR) {
				return ERROR;
			}
		} else {
			if (subprogram_declaration() == ERROR) {
				return ERROR;
			}
			strcpy(tmp_procedure_name, "NULL");
		}
	}
	scope_keyword = "block";
	printstartlabel();
	if (compound_statement() == ERROR) {
		return ERROR;
	}

	return NORMAL;
}

int variable_declaration() {
	if (token != TVAR) {
		return (error("Var is not found"));
	}
	token = scan();
	if (variable_names() == ERROR) {
		return ERROR;
	}
	if (token != TCOLON) {
		return error("Colon is not found");
	}
	token = scan();
	int arraysize = 0;
	check_scope(scope_keyword);
	if (subpro_flag == LOCAL) {
		if ((arraysize = type()) == 0) {
			return ERROR;
		}
		printdefine(&global, arraysize);
		TypeRegister(&global, type_def, arraysize, array_type_def);
		type_def = 0;
	} else if (subpro_flag == PROC) {
		if ((arraysize = type()) == 0)
			return (ERROR);
		printdefine(&local, arraysize);
		TypeRegister(&local, type_def, arraysize, array_type_def);
		type_def = 0;
	}
	if (token != TSEMI) {
		return error("Semicolon is not found at variable_declaration");
	}
	token = scan();
	while (token == TNAME) {
		if (variable_names() == ERROR) {
			return ERROR;
		}
		if (token != TCOLON) {
			return error("Colon is not found at variable_declaration(second)");
		}
		token = scan();

		if ((arraysize = type()) == 0) {
			return (ERROR);
		}
		check_scope(scope_keyword);
		if (subpro_flag == LOCAL) {
			printdefine(&global, arraysize);
			TypeRegister(&global, type_def, arraysize, array_type_def);
			type_def = 0;
		} else if (subpro_flag == PROC) {
			printdefine(&local, arraysize);
			TypeRegister(&local, type_def, arraysize, array_type_def);

			type_def = 0;
		}

		if (token != TSEMI) {
			return error("Semicolon is not found at variable_declaration");
		}
		token = scan();
	}
	return NORMAL;
}

int variable_names() {
	if (token != TNAME) {
		return error("Name is not found at variable_names");
	}
	check_scope(scope_keyword);
	if (subpro_flag == LOCAL) {
		add_newNODE(string_attr, "NULL", get_linenum());
		if (Dual_definision_check(&global) == ERROR)
			return ERROR;
	} else if (subpro_flag == PROC) {
		add_newNODE(string_attr, tmp_procedure_name, get_linenum());
		if (Dual_definision_check(&local) == ERROR)
			return ERROR;
	}

	token = scan();
	while (token == TCOMMA) {
		if (token != TCOMMA) {
			return error("Comama is not found at variable_names");
		}
		token = scan();
		if (token != TNAME) {
			return error("Name is not found at variable_names_second");
		}
		check_scope(scope_keyword);
		if (subpro_flag == LOCAL) {
			add_newNODE(string_attr, "NULL", get_linenum());
			if (Dual_definision_check(&global) == ERROR)
				return ERROR;
		} else if (subpro_flag == PROC) {
			add_newNODE(string_attr, tmp_procedure_name, get_linenum());
			if (Dual_definision_check(&local) == ERROR)
				return ERROR;
		}

		token = scan();
	}
	return NORMAL;
}

int type() {

	if (formal_flag == 1) {
		if ((token != TINTEGER) && (token != TBOOLEAN) && (token != TCHAR)) {

			printf("Error formula_parameters are not standard_type at%d\n",
					get_linenum());
			return ERROR;
		}
	}

	if (token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
		type_def = token;
		if ((type_def = standard_type()) == ERROR)
			return ERROR;

		return type_def;
	} else if (token == TARRAY) {
		if (array_type() == ERROR) {
			return ERROR;
		}
	}
	return ERROR;
}

int standard_type() {
	if (token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
		switch (token) {
		case TINTEGER:
			token = scan();
			return TINTEGER;
		case TBOOLEAN:
			token = scan();
			return TBOOLEAN;
		case TCHAR:
			token = scan();
			return TCHAR;
		default:
			break;
		}
	} else {
		return ERROR;
	}
	return NORMAL;
}
int array_type() {
	if (token != TARRAY) {
		return error("Array is not found");
	}
	token = scan();
	if (token != TLSQPAREN) {
		return error("[ is not found");
	}
	token = scan();
	if (token != TNUMBER) {
		return error("NUMBER is not found");
	}
	token = scan();
	if (token != TRSQPAREN) {
		return error("] is not found");
	}
	token = scan();
	if (token != TOF) {
		return error("of is not found");
	}
	token = scan();
	//
	if ((array_type_def = standard_type()) == ERROR) {
		return ERROR;
	}
	type_def = TARRAY;
	return num_attr;
}

int subprogram_declaration() {
	int paraflag = 0;
	scope_keyword = "procedure";
	if (token != TPROCEDURE) {
		return error("procedure is not found at subprogram_declaration");
	}
	token = scan();
	if (token != TNAME) {
		return error("Name is not found at subprogram_declaration");
	}
	check_scope(scope_keyword);
	if (subpro_flag == PROC) {

		strcpy(tmp_procedure_name, string_attr);
		strcpy(comp_procedure, string_attr);

		scope_keyword = "block";
		add_newNODE(string_attr, "NULL", get_linenum());
		if (Dual_definision_check(&global) == ERROR)
			return ERROR;
		type_def = TPROCEDURE;
		TypeRegister(&global, type_def, 0, 0);
		type_def = 0;
		scope_keyword = "procedure";
	}
	token = scan();
	if (token == TLPAREN) {
		paraflag = 1;
		//formal_parameter
		if (formal_parameters() == ERROR) {
			return ERROR;
		}
	}
//	else if (token != TLPAREN) {
//
//		TypeRegister(&global, 16, 0, 0);
//		type_def = 0;
//		//
//	}
	if (token != TSEMI)
		return (error("; of subprogram_declaration"));

	token = scan();
	if (token == TVAR) {
		if (variable_declaration() == ERROR) {
			return ERROR;
		}
	}
	printsubprostart(paraflag);
	paraflag = 0;
	if (compound_statement() == ERROR) {
		return ERROR;
	}
	if (token != TSEMI) {
		return error("Semicolon is not found at subprogram_declaration");
	}
	token = scan();
	printret();
	return NORMAL;
}
int procedure_name() {
	if (token != TNAME) {
		return error("Name is not found");
	}
	token = scan();
	return NORMAL;
}
int formal_parameters() {
	if (token != TLPAREN) {
		return error("( is not found at formal_parameters");
	}
	token = scan();
	formal_flag = 1;
	if (variable_names() == ERROR) {
		return ERROR;
	}
	if (token != TCOLON) {
		return error("Colon is not found at formal_parameters");
	}
	token = scan();
	int arraysize = 0;

	if ((arraysize = type()) == ERROR) {
		return ERROR;
	}
	printdefine(&local, arraysize);
	TypeRegister(&local, type_def, arraysize, array_type_def);
	type_def = 0;
	while (token == TSEMI) {
		token = scan();
		if (variable_names() == ERROR) {
			return ERROR;
		}
		if (token != TCOLON) {
			return error("colon is not found at formal_parameters");
		}
		token = scan();

		int arraysize = 0;

		if ((arraysize = type()) == ERROR)
			return (ERROR);
		printdefine(&local, arraysize);
		TypeRegister(&local, type_def, arraysize, array_type_def);

		type_def = 0;
		//
	}
	if (token != TRPAREN) {
		return error(") is not found at formal_parameters");
	}
	token = scan();
	formal_flag = 0;
	return NORMAL;
}

int compound_statement() {
	if (token != TBEGIN) {
		return error("begin is not found at compound_statement");
	}

	token = scan();
	if (statement() == ERROR) {
		return ERROR;
	}
	while (token == TSEMI) {
		token = scan();
		if (statement() == ERROR) {
			return ERROR;
		}
	}
	if (token != TEND) {
		return error("end is not found (compound_statement)");
	}
	token = scan();
	return NORMAL;
}

int statement() {

	if (token == TNAME) {
		//assignment_statement//
		if (assignment_statement() == ERROR) {
			return ERROR;
		}
		printassingnment();
	} else if (token == TIF) {
		//condition_statement//
		if (condition_statement() == ERROR) {
			return ERROR;
		}
	} else if (token == TBREAK) {
		if (num_while == 0) {
			return error("break must require over one while");
		}
		token = scan();
		num_while = 0;
	} else if (token == TWHILE) {
		num_while = 1;
		//iteration_statement//
		if (iteration_statement() == ERROR) {
			return ERROR;
		}
	} else if (token == TCALL) {
		//call_statement//
		if (call_statement() == ERROR) {
			return ERROR;
		}

	} else if (token == TRETURN) {
		//return_statement//
		printret();
		token = scan();
		return NORMAL;
	} else if (token == TREAD || token == TREADLN) {
		//input_statement//
		if (input_statement() == ERROR) {
			return ERROR;
		}
	} else if (token == TWRITE || token == TWRITELN) {
		//output_statement//
		if (output_statement() == ERROR) {
			return ERROR;
		}
	} else if (token == TBEGIN) {
		//compound_statement//
		if (compound_statement() == ERROR) {
			return ERROR;
		}
	} else if (token == TEND) {
		return NORMAL;
	} else {
		printf("ERROR statement is bad deffine  at %d\n", get_linenum());
		return ERROR;
	}
	return NORMAL;
}
int condition_statement() {
	if (token != TIF) {
		return error("if is not found at condition_statement");
	}
	token = scan();
	if (expression() != TBOOLEAN) {
		printf("ERROR expression is not BOLEAN at %d\n", get_linenum());
		return (ERROR);
	}
	if (token != TTHEN) {
		return error("then is not found at condition_statement");
	}
	token = scan();
	if (statement() == ERROR) {
		return ERROR;
	}
	if (token == TELSE) {
		token = scan();

		if (statement() == ERROR) {
			return ERROR;
		}
	}
	return NORMAL;
}
int iteration_statement() {
	int label1, label2;
	if (token != TWHILE) {
		return error("while is not found at iteration_statement");
	}
	label1 = printwhilelabel();
	token = scan();

	if (expression() != TBOOLEAN) {
		printf("while is not BOOLEAN at %d\n", get_linenum());
		return (ERROR);
	}
	label2 = printwhile();
	if (token != TDO) {
		return error("do is not found at iteration_statement");
	}
	token = scan();
	if (statement() == ERROR) {
		return ERROR;
	}
	printwhileend(label1, label2);
	return NORMAL;
}
int call_statement() {
	if (token != TCALL) {
		return error("call is not found at call_statement");
	}
	token = scan();
	char call[20];
	strcpy(call, string_attr);
	if (token != TNAME) {
		return error("Name is not found at call_statement");
	}

	if (RefRegister(&global) == ERROR) {
		printf("ERROR used but no definition at %d\n", get_linenum());
		return ERROR;
	}

	token = scan();
	if (token == TLPAREN) {
		token = scan();
		expflag=1;
		if (expressions() == ERROR) {
			return ERROR;
		}
		printpush();
		if (token != TRPAREN) {
			return error(") is not found at call_statement");
		}
		token = scan();
	}
	expflag=0;
	printcall(call);
	return NORMAL;
}
int expressions() {
	int exptype;
	adrflag = 1;
	if ((exptype = expression()) == ERROR)
		return (ERROR);
	if (adrflag == 0) {
	} else {
		adrflag = 0;
		printpush();
	}
	while (token == TCOMMA) {
		token = scan();
		adrflag = 1;
		if ((exptype = expression()) == ERROR)
			return (ERROR);
	}
	if (adrflag == 0) {
	} else {
		adrflag = 0;
		printpush();
	}
	return NORMAL;
}
int assignment_statement() {
	adrflag = 1;
	int assign_check, comp_assign_check;
	if ((assign_check = variabble()) == ERROR) {
		return ERROR;
	}
	adrflag = 0;
	printpush();
	if (token != TASSIGN) {
		return ERROR;
	}
	token = scan();
	if ((comp_assign_check = expression()) == ERROR) {
		return ERROR;
	}

	if (assign_check != comp_assign_check) {
		printf("ERROR different assign type at %d\n", get_linenum());
		return ERROR;
	}
	if ((assign_check != TINTEGER) && (assign_check != TBOOLEAN)
			&& (assign_check != TCHAR)) {
		printf("ERROR left_part is not standard_type at %d\n", get_linenum());
		return ERROR;
	}
	return NORMAL;
}
int variabble() {
	int tempflag = 0;
	int numflag = 0;
	if (token != TNAME) {
		return error("Name is not found at variabble");
	}
	strcpy(tmp_name, string_attr);
	check_scope(scope_keyword);
	if (subpro_flag == PROC) {

		if ((RefRegister(&local)) == ERROR) {

			if ((RefRegister(&global)) == ERROR) {
				printf("ERROR used but no definition at %d\n", get_linenum());
				return ERROR;
			}
		}

	} else if (subpro_flag == LOCAL) {
		if ((RefRegister(&global)) == ERROR) {
			printf("ERROR used but no definition at %d\n", get_linenum());
			return ERROR;
		}
	}
	token = scan();
	if (token == TLSQPAREN) {
		tempflag = adrflag;
		adrflag = 0;
		numflag = 1;
		token = scan();

		if (variabble_type != TARRAY) {
			printf("Error array_type is not integer at %d", get_linenum());
			return (ERROR);
		}

		if (expression() != TINTEGER) {
			printf("Error expression_type is not integer at %d", get_linenum());
			return (ERROR);
		}
		adrflag = tempflag;
		if (token != TRSQPAREN) {
			return error("] is not found at variabble");
		}
		token = scan();
		return variabble_array_type;
	}
	if (subpro_flag == PROC) {
		printloadvarlocal(&local, numflag, adrflag, tmp_name, writeflag);
	} else if (subpro_flag == LOCAL) {
		printloadvarglobal(&global, numflag, adrflag, tmp_name, writeflag);
	}
	return variabble_type;
}

int expression() {
	int expression_type;
	int comp_expression_type;
	int opr;
	if ((expression_type = simple_expression()) == ERROR) {
		return ERROR;
	}
	while (token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ
			|| token == TGR || token == TGREQ) {
		opr = token;
		if (relational_operator() == ERROR) {
			return ERROR;
		}
		if (adrflag == 1) {
			printconvert();
			adrflag = 0;
		}
		printpush();
		if ((comp_expression_type = simple_expression()) == ERROR) {
			return ERROR;
		}
		printexpression(opr);
		if (expression_type != comp_expression_type) {
			printf("ERROR expression type is different at %d\n", get_linenum());
			return ERROR;
		} else
			return TBOOLEAN;

	}
	return expression_type;
}

int simple_expression() {
	int simple_expression_type;
	int type_simple_expression_flag = 0;
	int opr;
	if (token == TPLUS || token == TMINUS) {
		if(token==TMINUS){
			printminus();
		}
		token = scan();
		type_simple_expression_flag = 1;
	}

	if ((simple_expression_type = term()) == ERROR)
		return ERROR;
	if (type_simple_expression_flag == 1) {
		if (simple_expression_type != TINTEGER) {
			printf("ERROR +- operated is not INTEGER at %d\n", get_linenum());
			return ERROR;
		}
	}
	opr = token;
	while (token == TPLUS || token == TMINUS || token == TOR) {
		if (adrflag == 1) {
			printconvert();
			adrflag = 0;
		}
		printpush();

		if (token == TPLUS || token == TMINUS) {
			if (additive_operator() == ERROR) {
				return ERROR;
			}
			if ((simple_expression_type = term()) != TINTEGER) {
				printf("ERROR (+,-) added is not INTEGER at %d\n",
						get_linenum());
				return (ERROR);
			}
		} else if (token == TOR) {
			if (additive_operator() == ERROR) {
				return ERROR;
			}
			if ((simple_expression_type = term()) != TBOOLEAN) {
				printf("ERROR or operated is not BOOLEAN at %d\n",
						get_linenum());
				return (ERROR);
			}
		}
		printsimpleexpression(opr);
	}
	return simple_expression_type;

}

int term() {
	int term_type = 0;
	int opr;
	if ((term_type = factor()) == ERROR) {
		return ERROR;
	}
	opr = token;
	while (token == TSTAR || token == TDIV || token == TAND) {
		if (token == TSTAR || token == TDIV) {
			if (multiplicative_operator() == ERROR) {
				return ERROR;
			}
			printconvert();
			adrflag = 0;

			printpush();
			if ((term_type = factor()) != TINTEGER) {
				printf("ERROR operated (*,div) is not INTEGER at %d\n",
						get_linenum());
				return ERROR;
			}
		} else if (token == TAND) {
			if (multiplicative_operator() == ERROR) {
				return ERROR;
			}
			if ((term_type = factor()) != TBOOLEAN) {
				printf("ERROR operated (and) is not BOOLEAN %d\n",
						get_linenum());
				return ERROR;
			}

		}
		printterm(opr);
		if(expflag==1){
			printpart(term_type);
		}
	}
	return term_type;
}

int factor() {
	factor_type = 0;
	if (token == TNAME) {
		adrflag = 0;
		if ((factor_type = variabble()) == ERROR)
			return ERROR;

		return factor_type;
	} else if (token == TNUMBER || token == TFALSE || token == TTRUE
			|| token == TSTRING) {
		if (token == TNUMBER) {
			printnum();
			if (constant() == ERROR) {
				return ERROR;
			}
			return TINTEGER;
		} else if (token == TFALSE || token == TTRUE) {
			printbool();
			if (constant() == ERROR) {
				return ERROR;
			}
			return TBOOLEAN;
		} else if (token == TSTRING) {
			if (constant() == ERROR) {
				return ERROR;
			}
			printstr();
			return TCHAR;
		}
	}
	else if (token == TLPAREN) {
		int factor_tmp_type;
		token = scan();
		if ((factor_tmp_type = expression()) == ERROR) {
			return ERROR;
		}
		if (token != TRPAREN) {
			return error(") is not found at factor");
		}
		token = scan();

		return factor_tmp_type;

	} else if (token == TNOT) {
		token = scan();
		if (factor() != TBOOLEAN) {
			printf("operated not is not type of BOOLEAN at %d\n",
					get_linenum());
			return ERROR;
		}
		printnot();
		return TBOOLEAN;
	} else if (token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
		int factor_type_standard = token;
		int comp_factor_type_standard;
		if (standard_type() == ERROR) {
			return ERROR;
		}
		if (token != TLPAREN)
			return (error("( is not found at factor"));

		token = scan();
		if ((comp_factor_type_standard = expression()) == ERROR)
			return ERROR;
		if ((comp_factor_type_standard != TINTEGER)
				&& (comp_factor_type_standard != TBOOLEAN)
				&& (comp_factor_type_standard != TCHAR)) {
			printf("ERROR factor type is not standard_type at %d\n",
					get_linenum());
			return ERROR;
		}

		if (token != TRPAREN)
			return (error(") is not found at factor"));
		printcast(factor_type_standard);
		token = scan();

		return factor_type_standard;
	} else
		return ERROR;
	return NORMAL;
}
int constant() {
	if (token == TNUMBER || token == TFALSE || token == TTRUE) {
		token = scan();
		switch (token) {
		case TNUMBER:
			return TINTEGER;
		case TFALSE:
			return TBOOLEAN;
		case TTRUE:
			return TBOOLEAN;
		default:
			break;
		}
	} else if (token == TSTRING) {
		token = scan();
		return TCHAR;
	} else {
		return ERROR;
	}
	return NORMAL;
}
int multiplicative_operator() {
	if (token == TSTAR || token == TDIV || token == TAND) {
		token = scan();
	} else {
		return ERROR;
	}
	return NORMAL;
}
int additive_operator() {
	if (token == TPLUS || token == TMINUS || token == TOR) {
		token = scan();
	} else {
		return ERROR;
	}
	return NORMAL;
}
int relational_operator() {
	if (token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ
			|| token == TGR || token == TGREQ) {
		token = scan();
	} else {
		return ERROR;
	}
	return NORMAL;
}
int input_statement() {
	int input_statement_type;
	int opr;
	adrflag = 1;
	if (token != TREAD && token != TREADLN) {
		return error("read/ln is not found at input_statement");
	}
	writeflag = 1;
	opr = token;
	token = scan();
	if (token == TLPAREN) {

		token = scan();
		if ((input_statement_type = variabble()) == ERROR)
			return (ERROR);
		if ((input_statement_type != TINTEGER)
				&& (input_statement_type != TCHAR)) {
			printf(
					"ERROR input_statement variabble type is not INTEGER and CHAR at %d\n",
					get_linenum());
			return ERROR;
		}
		printread(input_statement_type);
		while (token == TCOMMA) {

			token = scan();
			if ((input_statement_type = variabble()) == ERROR)
				return (ERROR);
			if ((input_statement_type != TINTEGER)
					&& (input_statement_type != TCHAR)) {
				printf(
						"ERROR input_statement variabble type is not INTEGER and CHAR at %d\n",
						get_linenum());
				return ERROR;
			}
			printread(input_statement_type);
		}
		if (token != TRPAREN) {
			return (error(")"));
		}
		printreadend(opr);
		token = scan();

	}
	adrflag = 0;
	writeflag = 0;
	return NORMAL;
}
int output_statement() {
	int opr;
	if (token != TWRITE && token != TWRITELN) {
		return error("write/ln is not found at output_statement");
	}
	opr = token;
	token = scan();
	if (token == TLPAREN) {
		token = scan();
		if (output_format() == ERROR) {
			return ERROR;
		}
		while (token == TCOMMA) {
			token = scan();
			if (output_format() == ERROR) {
				return ERROR;
			}
		}
		if (token != TRPAREN) {
			return error(") is not found at output statement");
		}
		token = scan();
	}
	printwriteend(opr);
	return NORMAL;
}
int output_format() {
	int output_format_type;
	int flag = 0;
	if (token == TSTRING) {
		printrefstring();
		token = scan();
		return NORMAL;
	} else if ((output_format_type = expression()) != ERROR) {
		if ((output_format_type != TINTEGER) && (output_format_type != TBOOLEAN)
				&& (output_format_type != TCHAR)) {
			printf("ERROR output_format is not standard_type at %d\n",
					get_linenum());
			return ERROR;
		}
		writeflag = 1;
		flag = checkvar(&global, writeflag);
		writeflag = 0;
		if (flag == 1) {
			fprintf(cslp, "\t\tLD\tgr2,gr0\n");
		}

		printwrite(output_format_type);
		if (token == TCOLON) {
			token = scan();
			if (token != TNUMBER)
				return ERROR;
			token = scan();
		}
		return NORMAL;
	} else
		return ERROR;
}

void pp_token() {
	if (token == TNUMBER || token == TNAME) {
		printf("%s", string_attr);
	} else {
		printf("%s", tokenstr[token]);
	}
}
