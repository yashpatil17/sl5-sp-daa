#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct OPTAB
{
	char mnemonic_code[10];
	int machine_code;
	char class[2];
} OPTAB;

OPTAB optab[] = {
	{"ADD", 1, "IS"},
	{"BC", 7, "IS"},
	{"COMP", 6, "IS"},
	{"DIV", 8, "IS"},
	{"END", 2, "AD"},
	{"EQU", 5, "AD"},
	{
		"DC",
		1,
		"DL",
	},
	{"DS", 2, "DL"},
	{"MOVER", 4, "IS"},
	{"MOVEM", 5, "IS"},
	{"LTORG", 3, "AD"},
	{"MULT", 3, "IS"},
	{"ORIGIN", 4, "AD"},
	{"PRINT", 10, "IS"},
	{"READ", 9, "IS"},
	{"STOP", 0, "IS"},
	{"SUB", 2, "IS"},
	{"START", 1, "AD"},
};

typedef union length_or_value {
	int length;
	char value[5];
} lorv;

typedef struct SYMTAB
{
	char sym_name[10];
	int address;
	lorv var;
	int flag;
} SYMTAB;
SYMTAB symtab[20];

typedef struct LITTAB
{
	char lit_name[10];
	int address;
} LITTAB;
LITTAB littab[20];

int pooltab[10];
int lc, ptp, ltp, stp;
FILE *ic;

void initialise()
{
	int i;
	lc = 0;
	pooltab[0] = 1;
	ptp = ltp = stp = 1;
	for (i = 1; i < 10; i++)
		pooltab[i] = -1;
	for (i = 1; i < 20; i++)
		symtab[i].address = -1;
	for (i = 1; i < 20; i++)
		littab[i].address = -1;
}

int search(char *token)
{
	for (int i = 0; i < sizeof(optab) / sizeof(optab[0]); i++)
	{
		if (strcmp(token, optab[i].mnemonic_code) == 0)
			return i;
	}

	printf("\n!Invalid mnemonic code %s", token);
	return -1;
}

void add_to_symtab(char symbol[], int lc, int length, char value[], int flag)
{
	strcpy(symtab[stp].sym_name, symbol);
	if (lc != -1)
		symtab[stp].address = lc;
	else
		symtab[stp].address = -1;
	if (flag == 1)
	{
		symtab[stp].flag = 1;
		strcpy(symtab[stp].var.value, value);
	}
	else if (flag == 0)
	{
		symtab[stp].flag = 0;
		symtab[stp].var.length = length;
	}
	else
		symtab[stp].flag = -1;
	stp++;
}

void add_to_littab(char literal[], int addr)
{
	strcpy(littab[ltp].lit_name, literal);
	if (addr != -1)
		littab[ltp].address = addr;
	else
		littab[ltp].address = -1;
	ltp++;
}

int search_in_symtab(char symbol[])
{
	for (int i = 1; i < stp; i++)
	{
		if (strcmp(symtab[i].sym_name, symbol) == 0)
			return i;
	}
	return -1;
}

int search_in_littab(char literal[], int start)
{
	for (int i = start; i < ltp; i++)
	{
		if (strcmp(littab[i].lit_name, literal) == 0)
			return i;
	}
	return -1;
}

void process_tokens(char **tokens, int no_of_tokens)
{
	int i, ind, ind1, inds, indl, lc_old, last_token;
	char *arr[3];
	char *copy;
	char delimiter[] = "+-";
	char delimiter_used;
	int lit_flag = 0;
	if (strcmp(tokens[0], "-") != 0)
	{
		if (search_in_symtab(tokens[0]) == -1)
			add_to_symtab(tokens[0], lc, 0, "", -1);
	}
	if (strcmp(tokens[1], "START") == 0 || strcmp(tokens[1], "ORIGIN") == 0)
	{
		if (strcmp(tokens[1], "START") == 0)
		{
			ic = fopen("ic.text", "w");
			lc_old = lc;
			lc = atoi(tokens[2]);
			fprintf(ic, "%d\t(AD 01) (C %s)\n", lc_old, tokens[2]);
			fclose(ic);
		}
		else
		{
			ic = fopen("ic.text", "a");
			lc_old = lc;
			if (isalpha(*tokens[2]))
			{
				copy = strdup(tokens[2]);
				i = 0;
				arr[i] = strtok(tokens[2], delimiter);
				while (arr[i] != NULL)
				{
					i++;
					arr[i] = strtok(NULL, delimiter);
				}
				delimiter_used = copy[strlen(arr[0])];
				last_token = i - 1;
				arr[last_token][strlen(arr[last_token]) - 2] = '\0';
				ind = search_in_symtab(arr[0]);
				if (ind != -1)
				{
					lc = symtab[ind].address;
					if (delimiter_used == '+')
						lc += atoi(arr[1]);
					else
						lc -= atoi(arr[1]);
				}
				else
				{
					fprintf(ic, "Symbol %s not found\n", tokens[2]);
					printf("\nSymbol %s not found\n", arr[0]);
				}
			}
			else
			{
				lc = atoi(tokens[2]);
			}
			fprintf(ic, "%d\t(AD 04) (C %d)\n", lc_old, lc);
			lc_old = lc;
			fclose(ic);
		}
	}
	else if (strcmp(tokens[1], "LTORG") == 0)
	{
		ic = fopen("ic.text", "a");
		fprintf(ic, "%d\t(AD 03)\n", lc);
		for (int i = pooltab[ptp - 1]; i < ltp; i++)
		{

			littab[i].address = lc;
			lc++;
		}
		pooltab[ptp++] = ltp;
		fclose(ic);
	}
	else if (strcmp(tokens[1], "END") == 0)
	{
		ic = fopen("ic.text", "a");
		fprintf(ic, "%d\t(AD 02)\n", lc);
		for (int i = pooltab[ptp - 1]; i < ltp; i++)
		{

			littab[i].address = lc;
			lc++;
		}
		fclose(ic);
		// return 0;
	}
	else if (strcmp(tokens[1], "EQU") == 0)
	{
		ic = fopen("ic.text", "a");
		fprintf(ic, "%d\t(AD 05)\n", lc);
		ind = search_in_symtab(tokens[0]);
		if (ind != -1)
		{
			ind1 = search_in_symtab(tokens[2]);
			if (ind1 != -1)
				symtab[ind].address = symtab[ind1].address;
		}
		fclose(ic);
	}
	else if (strcmp(tokens[1], "DS") == 0 || strcmp(tokens[1], "DC") == 0)
	{
		ic = fopen("ic.text", "a");
		if (strcmp(tokens[1], "DS") == 0)
		{
			fprintf(ic, "%d\t(DL 02) (C %s)\n", lc, tokens[2]);
			ind = search_in_symtab(tokens[0]);
			if (ind != -1)
			{
				symtab[ind].address = lc;
				symtab[ind].var.length = atoi(tokens[2]);
				symtab[ind].flag = 0;
			}
			lc = lc + atoi(tokens[2]);
		}
		else
		{
			fprintf(ic, "%d\t(DL 01) (C %s)\n", lc, tokens[2]);
			ind = search_in_symtab(tokens[0]);
			if (ind != -1)
			{
				symtab[ind].address = lc;
				strcpy(symtab[ind].var.value, tokens[2]);
				symtab[ind].flag = 1;
			}
			lc++;
		}
		fclose(ic);
	}
	else if (strcmp(tokens[1], "STOP") != 0)
	{
		ic = fopen("ic.text", "a");
		ind = search(tokens[1]);
		if (ind == -1)
		{
			printf("\nerror! mnemonic %s not found ", tokens[1]);
			exit(0);
		}
		else
		{
			if (tokens[3] != NULL)
			{
				if (isalpha(*tokens[3]))
				{
					inds = search_in_symtab(tokens[3]);
					if (inds == -1)
						add_to_symtab(tokens[3], -1, -1, "-1", -1);
					lit_flag = 0;
				}
				else
				{
					lit_flag = 1;
					indl = search_in_littab(tokens[3], pooltab[ptp - 1]);
					if (indl == -1)
						add_to_littab(tokens[3], -1);
				}
			}
		}
		if (strcmp(tokens[2], "AREG") == 0)
		{
			if (lit_flag)
			{
				indl = search_in_littab(tokens[3], pooltab[ptp - 1]);
				fprintf(ic, "%d\t(IS %d) (R 01) (L %d)\n", lc, optab[ind].machine_code, indl);
			}
			else
			{
				inds = search_in_symtab(tokens[3]);
				fprintf(ic, "%d\t(IS %d) (R 01) (S %d)\n", lc, optab[ind].machine_code, inds);
			}
		}
		else if (strcmp(tokens[2], "BREG") == 0)
		{
			if (lit_flag)
			{
				indl = search_in_littab(tokens[3], pooltab[ptp - 1]);
				fprintf(ic, "%d\t(IS %d) (R 02) (L %d)\n", lc, optab[ind].machine_code, indl);
			}
			else
			{
				inds = search_in_symtab(tokens[3]);
				fprintf(ic, "%d\t(IS %d) (R 02) (S %d)\n", lc, optab[ind].machine_code, inds);
			}
		}
		else if (strcmp(tokens[2], "CREG") == 0)
		{
			if (lit_flag)
			{
				indl = search_in_littab(tokens[3], pooltab[ptp - 1]);
				fprintf(ic, "%d\t(IS %d) (R 03) (L %d)\n", lc, optab[ind].machine_code, indl);
			}
			else
			{
				inds = search_in_symtab(tokens[3]);
				fprintf(ic, "%d\t(IS %d) (R 03) (S %d)\n", lc, optab[ind].machine_code, inds);
			}
		}
		else if (strcmp(tokens[2], "DREG") == 0)
		{
			if (lit_flag)
			{
				indl = search_in_littab(tokens[3], pooltab[ptp - 1]);
				fprintf(ic, "%d\t(IS %d) (R 04) (L %d)\n", lc, optab[ind].machine_code, indl);
			}
			else
			{
				inds = search_in_symtab(tokens[3]);
				fprintf(ic, "%d\t(IS %d) (R 04) (S %d)\n", lc, optab[ind].machine_code, inds);
			}
		}
		lc++;
		fclose(ic);
	}
	else
	{
		ic = fopen("ic.text", "a");
		fprintf(ic, "%d\t(IS 00)\n", lc);
		lc++;
		fclose(ic);
	}
}

int main()
{
	initialise();
	FILE *fp;
	int line_count = 0, no_of_tokens, i;
	char *line_buf = NULL, *token = NULL, ch;
	char **tokens = NULL;
	int last_token;
	size_t line_buf_size = 0;
	fp = fopen("input.txt", "r");
	while (getline(&line_buf, &line_buf_size, fp) > 0)
	{
		tokens = (char **)malloc(4 * sizeof(char *));
		for (i = 0; i < 4; i++)
		{
			tokens[i] = (char *)malloc(10 * sizeof(char));
		}
		no_of_tokens = 0;
		i = 0;
		token = strtok(line_buf, " ");
		while (token != NULL)
		{
			no_of_tokens++;
			strcpy(tokens[i++], token);
			token = strtok(NULL, " ");
		}
		last_token = no_of_tokens - 1;
		tokens[last_token][strlen(tokens[last_token]) - 2] = '\0';
		process_tokens(tokens, no_of_tokens);
		free(tokens);
	}
	line_buf = NULL;
	fclose(fp);
	fp = fopen("symtab.txt", "w");
	printf("\n--------------SYMBOL TABLE----------------");
	fprintf(fp, "--------------SYMBOL TABLE----------------");
	for (int i = 1; i < stp; i++)
	{
		if (symtab[i].flag == 1)
		{
			printf("\n%d %s\t %d %s", i, symtab[i].sym_name, symtab[i].address, symtab[i].var.value);
			fprintf(fp, "\n%d %s\t %d %s", i, symtab[i].sym_name, symtab[i].address, symtab[i].var.value);
		}
		else if (symtab[i].flag == 0)
		{
			printf("\n%d %s\t %d %d", i, symtab[i].sym_name, symtab[i].address, symtab[i].var.length);
			fprintf(fp, "\n%d %s\t %d %d", i, symtab[i].sym_name, symtab[i].address, symtab[i].var.length);
		}
		else
		{
			printf("\n%d %s\t %d -", i, symtab[i].sym_name, symtab[i].address);
			fprintf(fp, "\n%d %s\t %d -", i, symtab[i].sym_name, symtab[i].address);
		}
	}
	printf("\n------------------------------\n");
	fprintf(fp, "\n------------------------------\n");
	fclose(fp);

	fp = fopen("littab.txt", "w");
	fprintf(fp, "--------------LITERAL TABLE----------------");
	printf("\n--------------LITERAL TABLE----------------");
	for (int i = 1; i < ltp; i++)
	{
		printf("\n%d %s\t %d", i, littab[i].lit_name, littab[i].address);
		fprintf(fp, "\n%d %s\t %d", i, littab[i].lit_name, littab[i].address);
	}
	printf("\n------------------------------\n");
	fprintf(fp, "\n------------------------------\n");
	fclose(fp);

	//	printf("POOLTAB: \n");
	//	for(int i=0;i<ptp;i++)
	//		printf("%d\n",pooltab[i]);
}
