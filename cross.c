#include "token-list.h"

void check_scope(char *scope_keyword) {
	if (strcmp(scope_keyword, "block") == 0)
		subpro_flag = LOCAL;
	else if (strcmp(scope_keyword, "procedure") == 0)
		subpro_flag = PROC;
}
void TypeRegister(struct ID *id, int type_def, int arraysize, int array_type_def) {
	id = id->next;
	while (id != NULL) {
		if ((id->ttype == TPROCEDURE)) {
			if ((id->type_next->t_type == 0)) {
				if ((id->type_next->t_next = malloc(sizeof(struct TYPE)))
						== NULL) {
					printf("ERROR can not malloc\n");
				}
				id->type_next->t_type = type_def;
				id = id->next;
				continue;
			} else if (id->type_next->t_type != 0) {
				id->type_next->t_next->t_type = type_def;
				id = id->next;
				continue;
			}
		}
		if (id->ttype != 0) {
			id = id->next;
			continue;
		}
		id->ttype = type_def;
		if (type_def == TARRAY) {
			id->arraysize = arraysize;
			id->arraytype = array_type_def;
		}
		id = id->next;
	}
}
void Set_various_Type(struct ID *id) {
	if (id->ttype == TCHAR || id->ttype == TINTEGER || id->ttype == TBOOLEAN) {
		variabble_type = id->ttype;
		factor_type = id->ttype;

	} else if (id->ttype == TARRAY) {
		variabble_type = id->ttype;
		factor_type = id->ttype;
		variabble_array_type = id->arraytype;
	} else if (id->ttype == TPROCEDURE) {
		variabble_type = id->type_next->t_type;
		factor_type = id->ttype;
	}
}
int RefRegister(struct ID *id) {
	static char *comp_string;
	comp_string = string_attr;
//	int number=1;
	int i = 0;
	id = id->next;
	while (id != NULL) {
		if (strcmp(id->procedure_name, "NULL") != 0) {
			if (strcmp(id->procedure_name, tmp_procedure_name) != 0) {
				id = id->next;
//				number++;
				continue;
			}
		}
		if (strcmp(id->name, comp_string) == 0) {
			if ((id->ttype == TPROCEDURE)
					&& (strcmp(id->name, tmp_procedure_name) == 0)) {

				printf("ERRORR recursive call(saikiyobidasi)\n");
				return ERROR;
			}
			while (id->reflinenum[i] != 0) {
				i++;
			}
			id->reflinenum[i] = get_linenum();
			comp_string++;
			Set_various_Type(id);
//			return number+5;
			return NORMAL;
		}
		id = id->next;
//		number++;
	}
	return ERROR;
}
int Dual_definision_check(struct ID *id) {
	int check_count, count_loop, comp_count;
	check_count = 0;
	count_loop = 0;
	comp_count = 0;
	id = id->next;
	while (id != NULL) {
		count_loop++;
		check_scope(scope_keyword);
		if (subpro_flag == 1) {
			if (strcmp(id->name, globalidroot->name) == 0) {
				check_count = count_loop;
				comp_count++;
			}
			id = id->next;
		} else if (subpro_flag == 2) {
			if (strcmp(id->name, localidroot->name) == 0) {
				if (strcmp(id->procedure_name, "NULL") != 0) {
					if (strcmp(id->procedure_name, localidroot->procedure_name)
							!= 0) {
						id = id->next;
						continue;
					}
				}
				check_count = count_loop;
				comp_count++;
			}
			id = id->next;
		}
	}
	if ((check_count == count_loop) && (comp_count <= 1)) {
		return NORMAL;
	} else {
		printf("ERROR dual definition at %d\n",get_linenum());
		return ERROR;
	}
	return ERROR;
}
void PrintIDTable_globalandlocal(struct ID *id) {
	id = id->next;
	while (id != NULL) {

		printf("%-13s", id->name);
		if (strcmp(id->procedure_name, "NULL") != 0) {
			printf(":%-14s|", id->procedure_name);
		} else {
			printf("               |");
		}
		if (id->ttype == TPROCEDURE) {
			printf("%-15s(%-8s)|", tokenstr[id->ttype],
					tokenstr[id->type_next->t_type]);
		} else if (id->ttype == TARRAY)
			printf("array[%-5d] of %-11s|", id->arraysize,
					tokenstr[id->arraytype]);
		else
			printf("%-15s          |", tokenstr[id->ttype]);

		printf("%-8d|", id->defineline);
		int i = 0;
		while (id->reflinenum[i] != 0) {
			if (i == 0) {
				printf("%d", id->reflinenum[i]);
				i++;
			}else {
			printf(",%d", id->reflinenum[i]);
			i++;
			}
		}
		printf("\n");

		id = id->next;
	}
}
void PrintIDTable(struct ID *id, struct ID *id2) {
	printf(
			"Name                        |Type                     |Def.    |Ref.\n");
	PrintIDTable_globalandlocal(id);
	PrintIDTable_globalandlocal(id2);
}
