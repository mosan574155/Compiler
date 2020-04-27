/*
 * comet.c
 *
 *  Created on: 2019/01/08
 *      Author: centos
 */
#include "token-list.h"

struct LLIST {
	char *name;
	struct LLIST *next;
};
struct LLIST *labellistroot = NULL;
void printlibrary() {
	fprintf(cslp,
			"EOVF\n\t\tCALL\tWRITELINE\n\t\tLAD\tgr1,EOVF1\n\t\tLD\tgr2,gr0\n\t\tCALL\tWRITESTR\n\t\tCALL\tWRITELINE\n\t\tSVC\t1\nEOVF1\tDC\t'***** Run-Time Error : Overflow *****'\n");
	fprintf(cslp,
			"E0DIV\n\t\tJNZ\tEOVF\n\t\tCALL\tWRITELINE\n\t\tLAD\tgr1,E0DIV1\n\t\tLD\tgr2,gr0\n\t\tCALL\tWRITESTR\n\t\tCALL\tWRITELINE\n\t\tSVC\t2\nE0DIV1\tDC\t'***** Run-Time Error : Zero-Divide *****'\n");
	fprintf(cslp,
			"EROV\n\t\tCALL\tWRITELINE\n\t\tLAD\tgr1,EROV1\n\t\tLD\tgr2,gr0\n\t\tCALL\tWRITESTR\n\t\tCALL\tWRITELINE\n\t\tSVC\t3\nEROV1\tDC\t'***** Run-Time Error : Range-Over in Array index *****'\n");
	fprintf(cslp,
			"WRITECHAR\n\t\tRPUSH\n\t\tLD\tgr6,SPACE\n\t\tLD\tgr7,OBUFSIZE\nWC1\n\t\tSUBA\tgr2,ONE\n\t\tJZE\tWC2\n\t\tJMI\tWC2\n\t\tST\tgr6,OBUF,gr7\n\t\tCALL\tBOVFCHECK\n\t\tJUMP\tWC1\nWC2\n\t\tST\tgr1,OBUF,gr7\n\t\tCALL\tBOVFCHECK\n\t\tST\tgr7,OBUFSIZE\n\t\tRPOP\n\t\tRET\n");
	fprintf(cslp,
			"WRITESTR\n\t\tRPUSH\n\t\tLD\tgr6,gr1\nWS1\n\t\tLD\tgr4,0,gr6\n\t\tJZE\tWS2\n\t\tADDA\tgr6,ONE\n\t\tSUBA\tgr2,ONE\n\t\tJUMP\tWS1\nWS2\n\t\tLD\tgr7,OBUFSIZE\n\t\tLD\tgr5,SPACE\nWS3\n\t\tSUBA\tgr2,ONE\n\t\tJMI\tWS4\n\t\tST\tgr5,OBUF,gr7\n\t\tCALL\tBOVFCHECK\n\t\tJUMP\tWS3\nWS4\n\t\tLD\tgr4,0,gr1\n\t\tJZE\tWS5\n\t\tST\tgr4,OBUF,gr7\n\t\tADDA\tgr1,ONE\n\t\tCALL\tBOVFCHECK\n\t\tJUMP\tWS4\nWS5\n\t\tST\tgr7,OBUFSIZE\n\t\tRPOP\n\t\tRET\n");
	fprintf(cslp,
			"BOVFCHECK\n\t\tADDA\tgr7,ONE\n\t\tCPA\tgr7,BOVFLEVEL\n\t\tJMI\tBOVF1\n\t\tCALL\tWRITELINE\n\t\tLD\tgr7,OBUFSIZE\nBOVF1\n\t\tRET\nBOVFLEVEL\tDC\t256\n");
	fprintf(cslp,
			"WRITEINT\n\t\tRPUSH\n\t\tLD\tgr7,gr0\n\t\tCPA\tgr1,gr0\n\t\tJPL\tWI1\n\t\tJZE\tWI1\n\t\tLD\tgr4,gr0\n\t\tSUBA\tgr4,gr1\n\t\tCPA\tgr4,gr1\n\t\tJZE\tWI6\n\t\tLD\tgr1,gr4\n\t\tLD\tgr7,ONE\nWI1\n\t\tLD\tgr6,SIX\n\t\tST\tgr0,INTBUF,gr6\n\t\tSUBA\tgr6,ONE\n\t\tCPA\tgr1,gr0\n\t\tJNZ\tWI2\n\t\tLD\tgr4,ZERO\n\t\tST\tgr4,INTBUF,gr6\n\t\tJUMP\tWI5\nWI2\n\t\tCPA\tgr1,gr0\n\t\tJZE\tWI3\n\t\tLD\tgr5,gr1\n\t\tDIVA\tgr1,TEN\n\t\tLD\tgr4,gr1\n\t\tMULA\tgr4,TEN\n\t\tSUBA\tgr5,gr4\n\t\tADDA\tgr5,ZERO\n\t\tST\tgr5,INTBUF,gr6\n\t\tSUBA\tgr6,ONE\n\t\tJUMP\tWI2\nWI3\n\t\tCPA\tgr7,gr0\n\t\tJZE\tWI4\n\t\tLD\tgr4,MINUS\n\t\tST\tgr4,INTBUF,gr6\n\t\tJUMP\tWI5\nWI4\n\t\tADDA\tgr6,ONE\nWI5\n\t\tLAD\tgr1,INTBUF,gr6\n\t\tCALL\tWRITESTR\n\t\tRPOP\n\t\tRET\nWI6\n\t\tLAD\tgr1,MMINT\n\t\tCALL\tWRITESTR\n\t\tRPOP\n\t\tRET\nMMINT\tDC\t'-32768'\n");
	fprintf(cslp,
			"WRITEBOOL\n\t\tRPUSH\n\t\tCPA\tgr1,gr0\n\t\tJZE\tWB1\n\t\tLAD\tgr1,WBTRUE\n\t\tJUMP\tWB2\nWB1\n\t\tLAD\tgr1,WBFALSE\nWB2\n\t\tCALL\tWRITESTR\n\t\tRPOP\n\t\tRET\nWBTRUE\tDC\t'TRUE'\nWBFALSE\tDC\t'FALSE'\n");
	fprintf(cslp,
			"WRITELINE\n\t\tRPUSH\n\t\tLD\tgr7,OBUFSIZE\n\t\tLD\tgr6,NEWLINE\n\t\tST\tgr6,OBUF,gr7\n\t\tADDA\tgr7,ONE\n\t\tST\tgr7,OBUFSIZE\n\t\tOUT\tOBUF,OBUFSIZE\n\t\tST\tgr0,OBUFSIZE\n\t\tRPOP\n\t\tRET\nFLUSH\n\t\tRPUSH\n\t\tLD\tgr7,OBUFSIZE\n\t\tJZE\tFL1\n\t\tCALL\tWRITELINE\nFL1\n\t\tRPOP\n\t\tRET\n");
	fprintf(cslp,
			"READCHAR\n\t\tRPUSH\n\t\tLD\tgr5,RPBBUF\n\t\tJZE\tRC0\n\t\tST\tgr5,0,gr1\n\t\tST\tgr0,RPBBUF\n\t\tJUMP\tRC3\nRC0\n\t\tLD\tgr7,INP\n\t\tLD\tgr6,IBUFSIZE\n\t\tJNZ\tRC1\n\t\tIN\tIBUF,IBUFSIZE\n\t\tLD\tgr7,gr0\nRC1\n\t\tCPA\tgr7,IBUFSIZE\n\t\tJNZ\tRC2\n\t\tLD\tgr5,NEWLINE\n\t\tST\tgr5,0,gr1\n\t\tST\tgr0,IBUFSIZE\n\t\tST\tgr0,INP\n\t\tJUMP\tRC3\nRC2\n\t\tLD\tgr5,IBUF,gr7\n\t\tADDA\tgr7,ONE\n\t\tST\tgr5,0,gr1\n\t\tST\tgr7,INP\nRC3\n\t\tRPOP\n\t\tRET\n");
	fprintf(cslp,
			"READINT\n\t\tRPUSH\nRI1\n\t\tCALL\tREADCHAR\n\t\tLD\tgr7,0,gr1\n\t\tCPA\tgr7,SPACE\n\t\tJZE\tRI1\n\t\tCPA\tgr7,TAB\n\t\tJZE\tRI1\n\t\tCPA\tgr7,NEWLINE\n\t\tJZE\tRI1\n\t\tLD\tgr5,ONE\n\t\tCPA\tgr7,MINUS\n\t\tJNZ\tRI4\n\t\tLD\tgr5,gr0\n\t\tCALL\tREADCHAR\n\t\tLD\tgr7,0,gr1\nRI4\n\t\tLD\tgr6,gr0\nRI2\n\t\tCPA\tgr7,ZERO\n\t\tJMI\tRI3\n\t\tCPA\tgr7,NINE\n\t\tJPL\tRI3\n\t\tMULA\tgr6,TEN\n\t\tADDA\tgr6,gr7\n\t\tSUBA\tgr6,ZERO\n\t\tCALL\tREADCHAR\n\t\tLD\tgr7,0,gr1\n\t\tJUMP\tRI2\nRI3\n\t\tST\tgr7,RPBBUF\n\t\tST\tgr6,0,gr1\n\t\tCPA\tgr5,gr0\n\t\tJNZ\tRI5\n\t\tSUBA\tgr5,gr6\n\t\tST\tgr5,0,gr1\nRI5\n\t\tRPOP\n\t\tRET\n");
	fprintf(cslp,
			"READLINE\n\t\tST\tgr0,IBUFSIZE\n\t\tST\tgr0,INP\n\t\tST\tgr0,RPBBUF\n\t\tRET\n");
	fprintf(cslp,
			"ONE\tDC\t1\nSIX\tDC\t6\nTEN\tDC\t10\nSPACE\tDC\t#0020\nMINUS\tDC\t#002D\nTAB\tDC\t#0009\nZERO\tDC\t#0030\nNINE\tDC\t#0039\nNEWLINE\tDC\t#000A\nINTBUF\tDS\t8\nOBUFSIZE\tDC\t0\nIBUFSIZE\tDC\t0\nINP\tDC\t0\nOBUF\tDS\t257\nIBUF\tDS\t257\nRPBBUF\tDC\t0\n\t\t\tEND\n");
}
void printstart() {
	fprintf(cslp,
			"$$%s\tSTART\n\t\tLAD\tgr0,0\n\t\tCALL\tL%04d\n\t\tCALL\tFLUSH\n\t\tSVC\t0\n",
			string_attr, ++label_number);
}
void printdefine(struct ID *id, int arraysize) {
	id = id->next;
	while (id != NULL) {
		if (id->ttype == 0) {
			if ((type_def == 4)) {
				check_scope(scope_keyword);
				if (subpro_flag == 1)
					fprintf(cslp, "$%s\tDC\t%d\n", id->name, arraysize);
				if (subpro_flag == 2)
					fprintf(cslp, "$%s%%%s\tDC\t%d\n", id->name,
							id->procedure_name, arraysize);
			}
			if ((type_def != 4)) {
				check_scope(scope_keyword);
				if (subpro_flag == 1)
					fprintf(cslp, "$%s\tDC\t0\n", id->name);
				if (subpro_flag == 2)
					fprintf(cslp, "$%s%%%s\tDC\t0\n", id->name,
							id->procedure_name);
			}

		}
		id = id->next;
	}

}
void count_para(struct ID *id) {
	if (id == NULL) {
		return;
	}
	count_para(id->next);
	if (strcmp(id->procedure_name, tmp_procedure_name) == 0
			&& id->ispara == 1) {

		fprintf(cslp, "\t\tPOP\tgr1\n");
		fprintf(cslp, "\t\tST\tgr1,\t$%s%%%s\n", id->name, tmp_procedure_name);

	}

}
void printsubprostart(int flag) {
	fprintf(cslp, "$%s\n", tmp_procedure_name);
	if (flag == 0) {
		return;
	}
	fprintf(cslp, "\t\tPOP\tgr2\n");

	count_para(&local);

	fprintf(cslp, "\t\tPUSH\t0,gr2\n");
}
void printloadvarlocal(struct ID *id, int numflag, int adrflag, char *name,
		int writeflag) {
	while (id != NULL) {
		if ((strcmp(id->name, name) == 0)
				&& (strcmp(id->procedure_name, tmp_procedure_name) == 0)) {
			if (id->ispara == 0) {
				if (writeflag == 1) {
					fprintf(cslp, "\t\tLAD\tgr1,\t$%s", id->name);
				} else
					fprintf(cslp, "\t\tLD\tgr1,\t$%s", id->name);

				fprintf(cslp, "%%%s\n", id->procedure_name);
			} else {
				fprintf(cslp, "\t\tLD\tgr1,\t$%s", id->name);

				fprintf(cslp, "%%%s\n", id->procedure_name);
			}
			if (!adrflag && id->ispara) {
				fprintf(cslp, "\t\tLD\tgr1,0,gr1\n");
			}
		}
		id = id->next;

	}
}
void printloadvarglobal(struct ID *id, int numflag, int adrflag, char *name,
		int writeflag) {
	while (id != NULL) {
		if (strcmp(id->name, name) == 0) {
			if (id->ispara == 0) {
				if (writeflag == 1) {
					fprintf(cslp, "\t\tLD\tgr1,\t$%s\n", id->name);
				} else
					fprintf(cslp, "\t\tLAD\tgr1,\t$%s\n", id->name);
			} else {
				fprintf(cslp, "\t\tLD\tgr1,\t$%s\n", id->name);
			}
			if (numflag) {
				fprintf(cslp, "\t\tLD\tgr1,0,gr1\n");
			}
		}
		id = id->next;
	}
}

void printret() {
	fprintf(cslp, "\t\tRET\n");
}
void printassingnment() {
	fprintf(cslp, "\t\tPOP\tgr2\n\t\tST\tgr1,0,gr2\n");
}
int printif() {
	fprintf(cslp, "\t\tCPA\tgr1,gr0\n\t\tJZE\tL%04d\n", ++label_number);
	return label_number;
}
int printelse(int num) {
	fprintf(cslp, "\t\tJUMP\tL%04d\nL%04d\n", ++label_number, num);
	return label_number;
}
void printelseend(int num) {
	fprintf(cslp, "L%04d\n", num);
}
int printwhilelabel() {
	fprintf(cslp, "L%04d\n", ++label_number);
	return label_number;
}
int printwhile() {
	fprintf(cslp, "\t\tCPA\tgr1,gr0\n\t\tJZE\t\tL%04d\n", ++label_number);
	return label_number;
}
void printwhileend(int num1, int num2) {
	fprintf(cslp, "\t\tJUMP\tL%04d\nL%04d\n", num1, num2);
}
void printcall(char *proname) {
	fprintf(cslp, "\t\tCALL\t$%s\n", proname);
}


void printread(int type) {
	if (type == TINTEGER) {
		fprintf(cslp, "\t\tCALL\tREADINT\n");
	} else {
		fprintf(cslp, "\t\tCALL\tREADCHAR\n");
	}
}
void printreadend(int opr) {
	if (opr == TREADLN) {
		fprintf(cslp, "\t\tCALL\tREADLINE\n");
	}
}
void printwrite(int type) {
	if (type == TINTEGER)
		fprintf(cslp, "\t\tCALL\tWRITEINT\n");
	else if (type == TBOOLEAN)
		fprintf(cslp, "\t\tCALL\tWRITEBOOL\n");
	else if (type == TCHAR)
		fprintf(cslp, "\t\tCALL\tWRITECHAR\n");
}
void printwriteend(int opr) {
	if (opr == TWRITELN) {
		fprintf(cslp, "\t\tCALL\tWRITELINE\n");
	}
}

void printterm(int opr) {
	switch (opr) {
	case TSTAR:
		fprintf(cslp, "\t\tPOP\tgr2\n\t\tMULA\tgr1,gr2\n\t\tJOV\tEOVF\n");
		break;
	case TDIV:
		fprintf(cslp,
				"\t\tPOP\tgr2\n\t\tDIVA\tgr2,gr1\n\t\tLD\tgr1,gr2\n\t\tJOV\tE0DIV\n");
		break;
	case TAND:
		fprintf(cslp, "\t\tPOP\tgr2\n\t\tAND\tgr1,gr2\n");
		break;
	default:
		break;
	}
}
void printminus() {
	fprintf(cslp,
			"\t\tLD\tgr2,gr0\n\t\tSUBA\tgr2,gr1\n\t\tJOV\tEOVF\n\t\tLD\tgr1,gr2\n");
}
void printsimpleexpression(int opr) {
	fprintf(cslp, "\t\tPOP\tgr2\n");
	switch (opr) {
	case TPLUS:
		fprintf(cslp, "\t\tADDA\tgr1,\tgr2\n\t\tJOV\tEOVF\n");
		break;
	case TMINUS:
		fprintf(cslp, "\t\tSUBA\tgr2,\tgr1\n\t\tJOV\tEOVF\n\t\tLD\tgr1,gr2\n");
		break;
	case TOR:
		fprintf(cslp, "\t\tOR\tgr1,\tgr2\n");
		break;
	default:
		break;
	}
}
void printexpression(int opr) {
	fprintf(cslp, "\t\tPOP\tgr2\n\t\tCPA\tgr2,\tgr1\n");
	if (opr == TEQUAL)
		fprintf(cslp, "\t\tJZE\tL%04d\n", ++label_number);
	else if (opr == TNOTEQ)
		fprintf(cslp, "\t\tJNZ\tL%04d\n", ++label_number);
	else if (opr == TLE)
		fprintf(cslp, "\t\tJMI\tL%04d\n", ++label_number);
	else if (opr == TLEEQ) {
		fprintf(cslp, "\t\tJMI\tL%04d\n", ++label_number);
		fprintf(cslp, "\t\tJZE\tL%04d\n", label_number);
	} else if (opr == TGR)
		fprintf(cslp, "\t\tJPL\tL%04d\n", ++label_number);
	else if (opr == TGREQ) {
		fprintf(cslp, "\t\tJPL\tL%04d\n", ++label_number);
		fprintf(cslp, "\t\tJZE\tL%04d\n", label_number);
	}
	int tmp = label_number;
	fprintf(cslp, "\t\tLD\tgr1,\tgr0\n\t\tJUMP\tL%04d\nL%04d\n\t\tLAD\tgr1,\t1\n",
			++label_number, tmp);
	fprintf(cslp, "L%04d\n", label_number);
}
void printexception() {
	while (labellistroot != NULL) {
		fprintf(cslp, "%s", labellistroot->name);
		labellistroot = labellistroot->next;
	}
}

int checkvar(struct ID *id, int writeflag) {
	while (id != NULL) {
		if (strcmp(id->name, string_attr) == 0) {

			if (id->ispara == 0) {

				if (writeflag == 1) {
					fprintf(cslp, "\t\tLD\tgr1,\t$%s\n", id->name);
				} else
					fprintf(cslp, "\t\tLD\tgr1,\t$%s\n", id->name);
				return 1;
			} else {
				fprintf(cslp, "\t\tLD\tgr1,\t$%s/n", id->name);
				return 1;
			}
		}
		id = id->next;
	}
	return 2;
}

void setlabellist(int type, int lnumber, char* str) {
	struct LLIST* list;
	struct LLIST* i;
	char tmp[2048];
	char *string;
	if (type == TINTEGER || type == TCHAR) {
		if (str != NULL) {
			sprintf(tmp, "L%04d\tDC\t\'%s'\n", lnumber, str);
		} else {
			sprintf(tmp, "L%04d\tDC\t0\n", lnumber);
		}
	} else if (type == TBOOLEAN) {
		sprintf(tmp, "L%04d\tDC\t0\n", lnumber);
	} else if (type == TSTRING) {
		sprintf(tmp, "L%04d\tDC\t'%s'\n", lnumber, str);
	}
	if ((list = (struct LLIST *) malloc(sizeof(struct LLIST))) == NULL) {
		error("can not malloc\n");
	}
	if ((string = (char *) malloc(sizeof(char) * strlen(tmp))) == NULL) {
		error("can not malloc\n");
	}
	strcpy(string, tmp);
	list->name = string;
	list->next = NULL;
	if (labellistroot == NULL) {
		labellistroot = list;
	} else {
		for (i = labellistroot; i->next != NULL; i = i->next)
			;
		i->next = list;
	}
}

void printcast(int type) {
	if (type == TBOOLEAN) {
		fprintf(cslp, "\t\tPOP\tgr1\n\t\tCPA\tgr1,gr0\n");
		fprintf(cslp, "\t\tJZE\tL000%d", ++label_number);
		fprintf(cslp, "\t\tPUSH\t0,gr1\n");
	} else if (type == TCHAR) {
		fprintf(cslp, "\t\tPOP\tgr1\n\t\tLAD\tgr2,127\n");
		fprintf(cslp, "\t\tAND\tgr1,gr2\n\t\tPUSH\t0,gr1\n");
	}
}

void printrefstring(){
	fprintf(cslp, "\t\tLAD\tgr1,\tL%04d\n", ++label_number);
			fprintf(cslp, "\t\tLD\tgr2,gr0\n");
			fprintf(cslp, "\t\tCALL\tWRITESTR\n");
			setlabellist(TSTRING, label_number, string_attr);
}
void printnum() {
	fprintf(cslp, "\t\tLAD\tgr1,%d\n", num_attr);
}
void printbool() {
	if (token == TTRUE) {
		fprintf(cslp, "\t\tLAD\tgr1,\t1\n");
	} else {
		fprintf(cslp, "\t\tLAD\tgr1,\t0\n");
	}
}
void printstr() {
	fprintf(cslp, "\t\tLAD\tgr1,\t%d\n", (int) string_attr[0]);
}
void printnot() {
	fprintf(cslp, "\t\tLAD\tgr2,\t1\n\t\tXOR\tgr1,gr2\n");
}
void printstartlabel() {
	fprintf(cslp, "L0001\n");
}

void printpart(int type) {
	fprintf(cslp, "\t\tLAD\tgr2,\tL%04d\n\t\tST\tgr1,0,gr2\n\t\tPUSH\t0,gr2\n",
			++label_number);
	setlabellist(type, label_number, NULL);
}
void printpush() {
	fprintf(cslp, "\t\tPUSH\t0,gr1\n");
}
void printconvert() {
	fprintf(cslp, "\t\tLD\tgr1,0,gr1\n");
}

