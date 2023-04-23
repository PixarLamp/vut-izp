//
//FILE NAME: proj1.c
//
//NAME: Hrklova Zuzana
//
//DATE:11/13/2019
//
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void name_to_number_conversion(char *name, char *converted_name) //konvertuje meno kontaktu na cislice
{	
	int name_length = (int)strlen(name);
	int l = 0;
	for(int i = 0; i < name_length; i++)
	{
		char c = tolower(name[i]); 
		//kazdemu pismenu priradi zodpovedajucu cislicu a ulozi ju do noveho stringu
		if (('a'<=c) && (c<'d'))
			converted_name[l]='2'; 
		else if (('c'<c) && (c<'g'))
			converted_name[l]='3';
		else if (('f'<c) && (c<'j'))
			converted_name[l]='4';
		else if (('i'<c) && (c<'m'))
			converted_name[l]='5';
		else if (('l'<c) && (c<'p'))
			converted_name[l]='6';
		else if (('o'<c) && (c<'t'))
			converted_name[l]='7';
		else if (('s'<c) && (c<'w'))
			converted_name[l]='8';
		else if (('v'<c) && (c<='z'))
			converted_name[l]='9';
		else if(c == '+')
			converted_name[l]='0';
		else 
			continue;
		l++; //ak sa podari nahradit inedx l konvertovaneho mena posunie sa na dalsi v poradi 
	}
	converted_name[l] = '\0'; //ukoncenie stringu
}

void print_contact_containing_entered_no(char *first, char *second)
{//funkcia v mene kontaktu prepisuje \n na \0, aby meno bolo v rovnakom riadku ako cislo
	int i = strlen(first) - 1;
	first[i] = '\0';
	printf("%s, %s", first, second);	
}

bool find_sequence(char *text, char *sequence, int len_text)
{//hlada zadane cislo zo vstupu v mene/telefonnom cisle
	bool sequence_found = false;
	int sequence_length = (int)strlen(sequence);
	int k = 0;
	for (int j=0; j < len_text; j++)
	{
		if(text[j] == sequence[k]) //ak sa znak textu rovna prvej cislici vstupu posunie sa v cislici o index dalej a pokracuje v hladani
		{
			k +=1;
			if (k >= sequence_length)
				sequence_found = true; //ak sa podari najst aj cislicu z posledneho indexu vstupu, vstup sa nachadza v texte 
		}
	}
	if (sequence_found == true)
		return true; //kontakt obsahuje zadane cislo
	else
		return false; //kontakt neobsahuje zadane cislo
}

bool line_length_check(char *text)
{
	int text_length = (int)strlen(text);
	if (text[text_length - 1] == '\n')
		return true; //dlzka mena/tel.cisla je mensia ako 100 znakov
	else
		return false; //kontakt presahuje povoleny limit znakov
}

int main(int argc, char *argv[])
{
	char *CISLO = argv[1]; //hladane cislo
	char JMENO[102] = " ";
	char TELCISLO[102];	
	if (argc<2)
	{
		//nie je zadane hladane cislo pri spusteni
		while(1) //prechadza vsetky kontakty az do konca tel.zoznamu
		{
			char* a = fgets(JMENO, 102, stdin);
			if (a == NULL)
				break; //nacita prazdny riadok => koniec tel. zoznamu
			if (line_length_check(JMENO) == false)
			{	
				fprintf(stderr, "\nInvalid data input\n");
				return 1; //riadok prekracuje limit 100 znakov, koniec programu
			}
			fgets(TELCISLO, 102, stdin);//tel. cislo
			if (line_length_check(TELCISLO) == false)
			{
				fprintf(stderr, "\nInvalid data input\n");
				return 1; //prekroceny limit 100 znakov 
			}
			print_contact_containing_entered_no(JMENO, TELCISLO); //vytlaci tel. kontakt 	
		}
	}
	else //pri spusteni je zadane hladane cislo 
	{
		char converted_name[102];
		bool sequence_not_found = true;
		bool found_in_tel = false;
		bool found_in_name = false;
		while(1) //kym nedojde na koniec tel.zoznamu
		{
			char *b = fgets(JMENO, 102, stdin);
			if (b == NULL)
				break; //koniec tel.zoznamu
			if (line_length_check(JMENO) == false) //kontrola dlzky tel. kontaktu
			{
				fprintf(stderr, "\nInvalid data input\n");
				return 1; //meno kontaktu dlhsie ako 100 znakov
			}
			fgets(TELCISLO, 102, stdin); 
			if (line_length_check(TELCISLO) == false) //kontrola dlzky tel. cisla
			{
				fprintf(stderr, "\nInvalid data input\n");
				return 1; //tel. cislo dlhsie ako 100 znakov
			}	
			name_to_number_conversion(JMENO, converted_name); //konvercia znakov v tel. kontakte na ciselnu sekvenciu
			int len_converted_name = (int)strlen(converted_name);
			int len_tel = (int)strlen(TELCISLO);
			find_sequence(TELCISLO, CISLO, len_tel); //porovnavanie tel.cisla a hladaneho cisla
			if (find_sequence(TELCISLO, CISLO, len_tel) == true)
			{
				found_in_tel = true;	
				sequence_not_found = false;
			}
			find_sequence(converted_name, CISLO, len_converted_name); //porovnavanie mena v tel. kontakte a hladaneho cisla 
			if (find_sequence(converted_name, CISLO, len_converted_name) == true)
				{
					found_in_name = true;
					sequence_not_found = false;
				}
			if ((found_in_tel == true)||(found_in_name == true)) //ak je najdena zhoda s hladanym cislom v mene alebo cisle tel. kontaktu vypise kontakt
				print_contact_containing_entered_no(JMENO, TELCISLO);
			found_in_tel = false;
			found_in_name = false;
		}
		if (sequence_not_found == true)//ak sa v ziadnom riadku tel. zoznamu nenachadza hladane cislo vypise "not found"
			printf("Not found\n");
	}
	return 0;
}
