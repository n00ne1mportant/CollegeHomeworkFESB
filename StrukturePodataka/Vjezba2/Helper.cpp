#include "Helper.h"
optr CreateElement(char * n, char * l, int god)
{
	optr p = (optr)malloc(sizeof(_osoba));
	if (!p)
		return p;
	if ((n == NULL || strlen(n) <= 2) && (l == NULL || strlen(l) <= 2))
	{
		p->name = NULL;
		p->lname = NULL;
		p->next = NULL;
		p->born = 0;
		return p;
	}
	p->name = (char*)malloc(sizeof(char) * strlen(n) + 1);
	p->lname = (char*)malloc(sizeof(char) * strlen(l) + 1);
	if (!p->name || !p->lname)
		return NULL;
	strcpy(p->name, n);
	strcpy(p->lname, l);
	p->born = god;
	p->next = NULL;
	return p;
}
//--------------------------------------

int AddToBegining(optr root, char * name, char * lname, int god)
{
	if (!root)
		return ERROR_NULL_ROOT;
	optr p = CreateElement(name, lname, god);
	if (!p)
		return ERROR_ALLOCATING_MEMORY;
	p->next = root->next;
	root->next = p;
	return OK;
}
//------------------------

int AddToEnd(optr root, char* name, char* lname, int god)
{
	if (!root)
		return ERROR_NULL_ROOT;
	optr p = root;
	while (p->next)
	{
		p = p->next;
	}
	p->next = CreateElement(name, lname, god);
	p->next->next = NULL;
	return OK;
}
//-------------------------------------------

int ShowList(optr root)
{
	int i = 1;
	optr p = root->next;
	printf("Indx\tIme\tPrezime\t\tGod\n");
	while (p)
	{
		printf("[%d] %s\t%s\t\t%d\n", i++, p->name, p->lname, p->born);
		p = p->next;
	}
	if (i == 1)
		return ERROR_EMPTY_LIST;
	return OK;
}
//--------------------------------------------

int ShowElement(optr p)
{
	if (!p)
		return ERROR_EMPTY_LIST;
	printf("%s\t%s\t\t%d\n", p->name, p->lname, p->born);
	return OK;
}
//--------------------------------------------

int Delete(optr root, optr element)
{
	if (!element || !root)
		return ERROR_UNESPECTED_NULL_PARAMETER;
	if (!root->next)
		return ERROR_EMPTY_LIST;
	optr p = root;
	while (p->next != element && p->next != NULL)
		p = p->next;
	if (p->next == NULL)
		return ERROR_ELEMENT_NOT_FOUND;
	optr del = p->next;
	p->next = p->next->next;
	FreeE(del);
	return OK;
}
//---------------------------------------------

int FindElements(optr root, optr** found, char* lastName, int* FindAll)
{
	int count = 1;
	optr p = root->next;
	optr* temp = NULL;
	if (*FindAll > 0)
	{
		while (p)
		{
			if (strcmp(p->lname,lastName) == 0)
			{
				temp = (optr*)realloc(temp,sizeof(optr) * count);
				//*found = (optr*)realloc(*found,sizeof(_osoba) * count);
				//if (*found == NULL)
					//return ERROR_ALLOCATING_MEMORY;
				//*found[count] = (optr)malloc(sizeof(optr));
				//*found[count] = p;
				//count++;
				temp[count-1] = p;
				count++;
			}
			p = p->next;
		}
		if (count == 0)
			return ERROR_ELEMENT_NOT_FOUND;
		*found = temp;
		*FindAll = count-1;
		return OK;
	}
	else
	{
		while (p)
		{
			if (strcmp(p->lname, lastName) == 0)
			{
				*found = (optr*)malloc(sizeof(optr));
				if (*found == NULL)
					return ERROR_ALLOCATING_MEMORY;
				*found[0] = p;
				*FindAll = 1;
				return OK;
			}
			p = p->next;
		}
		return ERROR_ELEMENT_NOT_FOUND;
	}
}
//---------------------------------------------

char * Catch(int ErrorCode, int* ShuldExit)
{
	*ShuldExit = 0;
	switch (ErrorCode)
	{
	case ERROR_OPENING_FILE:
		*ShuldExit = ERROR_OPENING_FILE;
		return "Greska prilikom otvaranja datoteke. Jeste li unijeli tocan path?\n";
	case ERROR_WRITTING_TO_FILE:
		*ShuldExit = ERROR_WRITTING_TO_FILE;
		return "Greska prilikom pisanja u datoteku. Je li datoteka u uporabi?\n";
	case ERROR_ELEMENT_NOT_FOUND:
		*ShuldExit = ERROR_ELEMENT_NOT_FOUND;
		return "Element niza nije pronadjen\n";
	case ERROR_READING_FILE:
		*ShuldExit = ERROR_READING_FILE;
		return "Greska prilikom citanja iz datoteke\n";
	case ERROR_NULL_ROOT:
		*ShuldExit = ERROR_NULL_ROOT;
		return "Root je NULL\n";
	case ERROR_EMPTY_LIST:
		*ShuldExit = ERROR_EMPTY_LIST;
		return "Lista je prazna\n";
	case ERROR_ALLOCATING_MEMORY:
		*ShuldExit = ERROR_ALLOCATING_MEMORY;
		return "Malloc failed... fek\n";
	case ERROR_UNESPECTED_NULL_PARAMETER:
		*ShuldExit = ERROR_UNESPECTED_NULL_PARAMETER;
		return "Pogresan parametar";
	case OK:
		return "[OK]\n";
	default:
		break;
	}
}
//---------------------------------------------

int WriteToFIle(optr root, char * filepath)
{
	FILE* file_ = fopen(filepath, "w");
	if (!file_)
		return ERROR_OPENING_FILE;
	if (!root || !root->next)
		return ERROR_EMPTY_LIST;
	fprintf(file_,"#Ime\tPrezime\t\tGod\n");// -->To Do: Add comment support <-- DONE
	optr p = root->next;
	while (p)
	{
		fprintf(file_, "%s\t%s\t\t%d\n", p->name, p->lname, p->born);
		p = p->next;
	}
	if (p)
		return ERROR_WRITTING_TO_FILE;
	fclose(file_);
	return OK;
}
//---------------------------------------------

int ReadFile(optr root, char * path, int append)
{
	if (!root)
		return ERROR_NULL_ROOT;
	FILE* file_ = fopen(path, "r");
	if (!file_)
		return ERROR_OPENING_FILE;
	optr p = root;
	if (append)
	{
		while (p->next)
			p = p->next;
		int g = 0;
		char buffer[2 * NAME_LENGHT + 6]; // 2x space + 4 digits and 2 names
		while (fgets(buffer,2*NAME_LENGHT +6,file_)) //Hello fgets and GetSubstrings :)
		{
			if (strlen(buffer) < 10 || buffer[0] == '#')
				continue;
			char** data = GetSubstrings(buffer, '\t');
			if (!data)
				continue;
			optr n = CreateElement(data[0], data[1], atoi(data[2]));
			if (!n)
				return ERROR_ALLOCATING_MEMORY;
			n->next = p->next;
			p->next = n;
			p = p->next;
		}
		if (!feof(file_))
			return ERROR_READING_FILE;
		fclose(file_);
		return OK; 
	}//---<<< below, append = false
		int error = 0;
		error = ClearList(root);
		if (error != ERROR_EMPTY_LIST && error != 0)
			return error;
		int g = 0;
		char buffer[2 * NAME_LENGHT + 6]; // 2x space + 4 digits and 2 names
		while (fgets(buffer, 2 * NAME_LENGHT + 6, file_))
		{
			if (strlen(buffer) < 10 || buffer[0] == '#')
				continue;
			char** data = GetSubstrings(buffer, '\t');
			if (!data)
				continue;
			p->next = CreateElement(data[0], data[1], atoi(data[2]));
			p = p->next;
		}
		if (!feof(file_))
			return ERROR_READING_FILE;
		fclose(file_);
		return OK;
}
//---------------------------------------------

int ClearList(optr root)
{
	if (!root)
		return ERROR_NULL_ROOT;
	if (!root->next)
		return ERROR_EMPTY_LIST;
	optr p = root->next;
	optr q = p;
	while (p->next)
	{
		q = p;
		p = p->next;
		FreeE(q);
	}
	FreeE(p);
	root->next = NULL;
	return OK;
}
//--------------------------------------------

int FreeE(optr el)
{
	if (!el)
		return ERROR_UNESPECTED_NULL_PARAMETER;
	free(el->lname);
	free(el->name);
	free(el);
	return OK;
}
//--------------------------------------------

char** GetSubstrings(char* string, char termChar)  
{
	int lengt = strlen(string);
	if (lengt < 10)
		return NULL;
	int i, previ = 0, count = 0;
	for (i = 0; i<lengt; i++)
	{
		if (string[i] == termChar )
		{
			if (string[i - 1] == termChar)
				continue;
			count++;
			previ = i;
		}
	}
	count++;
	char** substrings = (char**)malloc(sizeof(char*)*count);
	if (!substrings)
		return NULL;
	int tempCount = 0;
	previ = 0;
	for (i = 0; i<=lengt; i++)
	{
		if (string[i] == termChar || string[i] == '\0' || string[i] == '\n' || i == lengt || tempCount == count)
		{
			char* sub = (char*)malloc(sizeof(char)*(i - previ) + 1);
			if (!sub)
				return NULL;
			memcpy(sub, &string[previ], i - previ);
			sub[i - previ] = '\0'; //memset mozda?
			previ = i + 1;
			if (strlen(sub) <= 1)
			{
				free(sub);
				continue;
			}
			substrings[tempCount++] = sub; 
		}
	}
	return substrings;
}
