/*
 * scan.c
 *
 *  Created on: 2018/10/26
 *      Author: centos
 */
#include "token-list.h"

int init_scan(char *filename) {
	fp = fopen(filename, "r");
	if (fp == NULL) {
		linenum = 0;
		return -1;
	}
	cbuf = fgetc(fp);
	linenum = 1;
	return 0;

}

void end_scan() {
	fclose(fp);
}

int get_linenum() {
	return linenum;
}

void output(char *tokenstr[NUMOFTOKEN + 1], int numtoken[NUMOFTOKEN + 1]) {
	int i = 0;
	for (i = 0; i < NUMOFTOKEN + 1; i++) {
		if (numtoken[i] > 0) {
			printf("\t\"Identifier\" \"%s\"\t%d\n", tokenstr[i], numtoken[i]);
		}
	}

}

int scan() {

	char read_number[1024] = {};
	char read_name[1024] = {};
	char read_string[1024] = {};
	int number_array_idx = 0;
	int name_array_idx = 0;
	int string_array_idx = 0;
	int flag = 0;

	do {
		switch (cbuf) {
//number count//
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
			number_array_idx = 1;
			read_number[0] = cbuf;
			int number_string = 1;

			while (1) {
				cbuf = fgetc(fp);
				if (!(cbuf <= 57 && cbuf >= 48)) {

					num_attr = atoi(read_number);
					if (num_attr < 0 || num_attr > 32767) {
						error("over limited number");
						return -1;
					}
					if (number_string > 1024) {
						error("over limited size");
						return -1;
					}
					read_number[number_array_idx++] = '\0';
					//henko//
					strcpy(string_attr, read_number);
					memset(read_number, 0, sizeof(read_number));
					return TNUMBER;
				}
				read_number[number_array_idx++] = cbuf;
				number_string++;

			}

			break;

//keyword and name//
			//A~Z//
		case 65:
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
		case 76:
		case 77:
		case 78:
		case 79:
		case 80:
		case 81:
		case 82:
		case 83:
		case 84:
		case 85:
		case 86:
		case 87:
		case 88:
		case 89:
		case 90:
			//a~z//
		case 97:
		case 98:
		case 99:
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122:
			name_array_idx = 1;
			read_name[0] = cbuf;
			int name_length = 1;
			while (1) {
				cbuf = fgetc(fp);
//!(number or alphabet or alphabet)//
				if (!((cbuf <= 57 && cbuf >= 48) || (cbuf <= 90 && cbuf >= 65)
						|| (cbuf <= 122 && cbuf >= 97))) {
					if (name_length > MAXSTRSIZE) {
						error("Too long name");
						return -1;
					}
					//keyword or not keyword?//
					int j = 0;
					while (j < 29) {
						if ((strcmp(read_name, key[j].keyword)) == 0) {
							read_name[name_array_idx++] = '\0';
							memset(read_name, 0, sizeof(read_name));
							return key[j].keytoken;
						}
						j++;
					}
					//name//
					strcpy(string_attr, read_name);
					//printf("%s",string_attr);
					string_attr[name_length++] = '\0';
					return TNAME;
				}
				read_name[name_array_idx++] = cbuf;
				name_length++;
			}

			break;
//symbol//
			//\n,\r,'',\//
		case 10:
		case 13:
		case 9:
		case 32:
			if (cbuf == 10 || cbuf == 13) {
				linenum += 1;
			}
			cbuf = fgetc(fp);
			break;
			//+//
		case 43:
			cbuf = fgetc(fp);
			return TPLUS;
			//-//
		case 45:
			cbuf = fgetc(fp);
			return TMINUS;
			//*//
		case 42:
			cbuf = fgetc(fp);
			return TSTAR;
			//=//
		case 61:
			cbuf = fgetc(fp);
			return TEQUAL;
			//<=,<>,<//
		case 60:
			cbuf = fgetc(fp);
			if (cbuf == 61) {
				cbuf = fgetc(fp);
				return TLEEQ;
			} else if (cbuf == 62) {
				cbuf = fgetc(fp);
				return TNOTEQ;
			} else
				return TLE;
			//>=,>//
		case 62:
			cbuf = fgetc(fp);
			if (cbuf == 61) {
				cbuf = fgetc(fp);
				return TGREQ;
			} else
				return TGR;
			//( //
		case 40:
			cbuf = fgetc(fp);
			return TLPAREN;
			//) //
		case 41:
			cbuf = fgetc(fp);
			return TRPAREN;
			//[ //
		case 91:
			cbuf = fgetc(fp);
			return TLSQPAREN;
			//] //
		case 93:
			cbuf = fgetc(fp);
			return TRSQPAREN;
			//:= ,://
		case 58:
			cbuf = fgetc(fp);
			if (cbuf == 61) {
				cbuf = fgetc(fp);
				return TASSIGN;
			} else
				return TCOLON;
			//. //
		case 46:
			cbuf = fgetc(fp);
			return TDOT;
			//, //
		case 44:
			cbuf = fgetc(fp);
			return TCOMMA;
			//; //
		case 59:
			cbuf = fgetc(fp);
			return TSEMI;
//string//
			//' //
		case 39:
			string_array_idx = 0;
			read_string[0] = cbuf;
			int string_length = 1;

			while ((cbuf = fgetc(fp)) != EOF) {
				if (cbuf == '\n' || cbuf == '\r') {
					error("your string is not closed .");
				}
				if (string_length > MAXSTRSIZE) {
					error("Too long string");
					return -1;
				}
				if (cbuf == 39) {
					if ((cbuf = fgetc(fp)) == 39) {
						read_string[string_array_idx++]='\'';
						read_string[string_array_idx++]='\'';
						continue;
					}
						strcpy(string_attr, read_string);
						string_attr[string_length++] = '\0';
						memset(read_string, 0, sizeof(read_string));
						return TSTRING;
				}
				read_string[string_array_idx++] = cbuf;
				string_length++;

			}
			error("your string is not closed.");
			return -1;
			break;

			//{ }//
		case 123:

			while ((cbuf = fgetc(fp)) != EOF) {
				if (cbuf == 125) {
					flag = 1;
					cbuf = fgetc(fp);
					break;
				}
			}
			if (flag == 1) {
				break;
			} else
				error("wrong text");
			return -1;
			break;
			// /* */  //
		case 47:
			if ((cbuf = fgetc(fp) == 42)) {
				while ((cbuf = fgetc(fp)) != EOF) {
					if (cbuf == 42 && (cbuf = fgetc(fp)) == 47) {
						flag = 1;
						cbuf = fgetc(fp);
						break;
					}
				}
			}
			if (flag == 1) {
				break;
			} else {
				error("bad comment.");
				return -1;
			}

			break;

		default:
			return -1;
		}

	} while (cbuf != EOF);

	return -1;
}
