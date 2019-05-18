#include <stdio.h>
#include <stdlib.h>

/* Our data structure, each structure shows one letter (with symbols -quote, double quote or ending S or s- or not) */
struct NewChars
{
    int character;
    int quote;
    int doublequotes;
    int S_or_s;
};


/*Declarations*/
void createDifthoggo(int first);

struct NewChars mapper(struct NewChars a,int c, int q, int dq, int sS);

int stateMorN(int c,int goNext, struct NewChars characters[]);

int stateAny(int c, int goNext,struct NewChars characters[]);


FILE* fpOut;
FILE* fp;


/*Definitions*/

/*This function creates a letter from a digraph based on the parameters
        VARIABLES:
        first: the first letter of the digraph, based on this we write
        the right letter that replaces the digraph
*/
void createDifthoggo(int first)
{
    if(first == 204)
    {
		fprintf(fpOut,"%c",66);
    }
    else if(first == 236)
    {
		fprintf(fpOut,"%c",98);
    }
    else if(first == 205)
    {
		fprintf(fpOut,"%c",68);
    }
    else if(first == 237)
    {
		fprintf(fpOut,"%c",100);
    }
}

/* We use this function to create a structure of each of our Latin-based letter
        VARIABLES:
        a: the structure we will modify
        c: the Greek-based character
        q: if it has a quote
        dq: if it has a double quote
        sS: if it has an ending s or S such as KS,PS,ks,ps
        Returns: the structure we modified
*/
struct NewChars mapper(struct NewChars a,int c, int q, int dq, int sS)
{
    a.character = c;
    a.quote = q;
    a.doublequotes = dq;
    a.S_or_s = sS;
    return a;
}

/*  We use this function if we catch up with a letter that may be a digraph such as m,M,n,N
    first, we save that first letter into a variable, then reads another letter and make some
    checks about the second letter we read. If it is and End Of File we print the first letter
    and returns. If it is the same as the first letter then we print the first letter (mm is not a digraph)
    and call again the function with new "first letter" as the previous stated "second letter".
    If we come along a character that would not create a digraph, then we print the first letter and pass the second
    letter we came along as an argument to another functions that checks for ANY letters.
    Last, if we meet a letter that consists a digraph, then we write the letter that replaces that one.
    In all last 3 cases, we modify the goNext argument and at the end we return it. In the first we just
    return -1.
        VARIABLES:
        c: the Greek-based character
        characters: an array with our modified structures
        goNext: a variable we use to check if we need to move on to the reading of
        the next character or not, we do not use it or check it here,
        just modifies it and returns it
*/
int stateMorN(int c,int goNext, struct NewChars characters[])
{
    int digraph = 0;

    digraph = c;
	c = fgetc(fp);

    /*we read End Of File, print the last character and return*/
    if(c==EOF)
    {
		fprintf(fpOut,"%c",characters[digraph].character);
        return -1;
    }
    else if(c==digraph)
    {
		fprintf(fpOut,"%c",characters[digraph].character);
        goNext = stateMorN(c,goNext,characters);
    }
    else if(!(
                ((digraph==204 || digraph==236) && (c==240 || c==208)) ||
                ((digraph==205 || digraph==237) && (c==244 || c==212))
            ))
    {
		fprintf(fpOut,"%c",characters[digraph].character);
        goNext = stateAny(c,goNext,characters);
    }
    else
    {
        createDifthoggo(digraph);
        goNext = 1;
    }

    return goNext;
}

/*  We use this function (called by the main) when reading Greek letters to print them as Latin one.
    First we check if the code of the letter can be the first letter of digraph, if so
    we pass it to the function that controls digraphs. Then we check the goNext so as if
    a the first case was true and a digraph has been made then we return from the function
    in order to move to the next one or if we reached End Of File.
    Then we check the cases that if a letter has a quote, a double quote or an ending s or S and replace
    that letter with the right one. Then we check if we have a new line, and print it and lastly,
    we check if the program is not ready to read the next char and if so we print the letters
    that do not meet none of the above cases, so they do not have
    a quote,double quote, ending s or S, they are not the character of new line
    and they are not the first letter of a "may be" digraph. Symbols also are in that last case.
    In the end it returns the goNext variable to the main or the function for the digraphs in each case.
        VARIABLES:
        c: the Greek-based character
        characters: an array with our modified structures
        goNext: a variable we use to check if we need to move on to the reading of
        the next character or not, here we can check this character, modify it and return it
*/
int stateAny(int c, int goNext,struct NewChars characters[])
{
    /* c = first letter of a possible digraph */
    if((c==204 || c==205 || c==236 || c==237) && goNext == 0)
    {
        goNext = stateMorN(c,goNext,characters);
    }

    /* Read next char (1) or stop the program (-1) */
    if(goNext == 1 || goNext == -1)
    {
        return goNext;
    }

    /* Ending s or S, KS,PS,ks,ps */
    if(c==206 || c==216 || c==238 || c==248)
    {
		fprintf(fpOut,"%c",characters[c].character);
		fprintf(fpOut,"%c",characters[c].S_or_s);
        goNext = 1;
    }

    /* new line */
    if(c == 92)
    {
		fprintf(fpOut,"\n");
        goNext = 1;
    }

    /* simple letters that do not meet the cases above */
    if(goNext == 0)
    {
        /* if it is a letter we already converted */
        if(characters[c].character >0){
			fprintf(fpOut,"%c",characters[c].character);
            goNext = 1;
        }
        else /* if it is a symbol */
        {
			fprintf(fpOut,"%c",c);
            goNext = 1;
        }
    }

    return goNext;
}

/*  Our main function is the controller of our program. We create the structures we need here
    we start reading the input text file and passing each character at the function and when we
    reach the end of the input file then the program stops.
    Main function "gives the coordinates" to other functions and variables in order to convert
    each Greek letter we read into a Latin one and each symbol as the right one.
        VARIABLES:
        c: the input character we read
        characters: the array with our structures that provide the replacements of the Greek letters
*/
int main(int argc, char* argv[])
{
    /*  our character any time, and our array with our structures that would consist
		the replacements for the Greek letters
    */
    int c;
    struct NewChars characters[255];

	if(argc != 3){
		printf("Wrong number of args\n");
		exit(1);
	} else {
		fp = fopen(argv[1], "r");
		fpOut = fopen(argv[2], "w");
		stdout = fpOut;
	}

    /* Replaces Greek letters with the Latin one */
    characters[182] = mapper(characters[182],65,39,0,0);
    characters[184] = mapper(characters[184],69,39,0,0);
    characters[185] = mapper(characters[185],72,39,0,0);
    characters[186] = mapper(characters[186],73,39,0,0);
    characters[188] = mapper(characters[188],79,39,0,0);
    characters[190] = mapper(characters[190],89,39,0,0);
    characters[191] = mapper(characters[191],87,39,0,0);
    characters[192] = mapper(characters[192],105,39,34,0);
    characters[193] = mapper(characters[193],65,0,0,0);
    characters[194] = mapper(characters[194],86,0,0,0);
    characters[195] = mapper(characters[195],71,0,0,0);
    characters[196] = mapper(characters[196],68,0,0,0);
    characters[197] = mapper(characters[197],69,0,0,0);
    characters[198] = mapper(characters[198],90,0,0,0);
    characters[199] = mapper(characters[199],72,0,0,0);
    characters[200] = mapper(characters[200],56,0,0,0);
    characters[201] = mapper(characters[201],73,0,0,0);
    characters[202] = mapper(characters[202],75,0,0,0);
    characters[203] = mapper(characters[203],76,0,0,0);
    characters[204] = mapper(characters[204],77,0,0,0);
    characters[205] = mapper(characters[205],78,0,0,0);
    characters[206] = mapper(characters[206],75,0,0,83);
    characters[207] = mapper(characters[207],79,0,0,0);
    characters[208] = mapper(characters[208],80,0,0,0);
    characters[209] = mapper(characters[209],82,0,0,0);
    characters[211] = mapper(characters[211],83,0,0,0);
    characters[212] = mapper(characters[212],84,0,0,0);
    characters[213] = mapper(characters[213],89,0,0,0);
    characters[214] = mapper(characters[214],70,0,0,0);
    characters[215] = mapper(characters[215],88,0,0,0);
    characters[216] = mapper(characters[216],80,0,0,83);
    characters[217] = mapper(characters[217],87,0,0,0);
    characters[218] = mapper(characters[218],73,0,34,0);
    characters[219] = mapper(characters[219],89,0,34,0);
    characters[220] = mapper(characters[220],97,39,0,0);
    characters[221] = mapper(characters[221],101,39,0,0);
    characters[222] = mapper(characters[222],104,39,0,0);
    characters[223] = mapper(characters[223],105,39,0,0);
    characters[224] = mapper(characters[224],121,39,34,0);
    characters[225] = mapper(characters[225],97,0,0,0);
    characters[226] = mapper(characters[226],118,0,0,0);
    characters[227] = mapper(characters[227],103,0,0,0);
    characters[228] = mapper(characters[228],100,0,0,0);
    characters[229] = mapper(characters[229],101,0,0,0);
    characters[230] = mapper(characters[230],122,0,0,0);
    characters[231] = mapper(characters[231],104,0,0,0);
    characters[232] = mapper(characters[232],56,0,0,0);
    characters[233] = mapper(characters[233],105,0,0,0);
    characters[234] = mapper(characters[234],107,0,0,0);
    characters[235] = mapper(characters[235],108,0,0,0);
    characters[236] = mapper(characters[236],109,0,0,0);
    characters[237] = mapper(characters[237],110,0,0,0);
    characters[238] = mapper(characters[238],107,0,0,115);
    characters[239] = mapper(characters[239],111,0,0,0);
    characters[240] = mapper(characters[240],112,0,0,0);
    characters[241] = mapper(characters[241],114,0,0,0);
    characters[242] = mapper(characters[242],115,0,0,0);
    characters[243] = mapper(characters[243],115,0,0,0);
    characters[244] = mapper(characters[244],116,0,0,0);
    characters[245] = mapper(characters[245],121,0,0,0);
    characters[246] = mapper(characters[246],102,0,0,0);
    characters[247] = mapper(characters[247],120,0,0,0);
    characters[248] = mapper(characters[248],112,0,0,115);
    characters[249] = mapper(characters[249],119,0,0,0);
    characters[250] = mapper(characters[250],105,0,34,0);
    characters[251] = mapper(characters[251],121,0,34,0);
    characters[252] = mapper(characters[252],111,39,0,0);
    characters[253] = mapper(characters[253],121,39,0,0);
    characters[254] = mapper(characters[254],119,39,0,0);

    /*  As long as we do not reach End Of File, we keep reading and passing the
		input we read as an argument to the function that controls all letters.
        VARIABLES:
        goNext: if we move on to the next character or we stop the program
        c: the input character
    */
    while((c = fgetc(fp)) != EOF)
    {

        int goNext = 0;

        goNext = stateAny(c,goNext,characters);

        /*We reached End Of File inside the other functions (not main)*/
        if(goNext == -1)
        {
            break;
        }
    }
	
	fclose(fp);
	fclose(fpOut);

    return 0;
}

