# include "main.h"
const char separators[] = {" \",\t.;?!\n"};

int main()
{
	int nlines;
	char **text;
	Link root = NULL;

	scanf("%d\n", &nlines);								/* obtém o nº de linhas a serem lidas */
	text = (char**) malloc (sizeof(char*)*nlines);		/* aloca memória para o texto */
	root = insertLines(text, nlines, root);				/* insere a linha no texto e na árvore (palavra a palavra) */
	root = executeCommands(root, text, nlines);			/* recebe e executa o comando que for inserido */

	erase (root, text, nlines);							/* apaga toda a memória alocada pelo programa */
	return 0;
}

Link executeCommands (Link root, char** text, int nlines)
{
	char c;

	while (scanf("%c", &c) > 0)
		switch (c)
		{
			case 'f':
				root = F_command (root);				/* esquece uma palavra */
				break;

			case 's':
				S_command (text, nlines);				/* mostra o texto original */
				break;

			case 'l':
				L_command (root, text, nlines);			/* lista uma palavra e as linhas em que ocorre */
				break;

			case 'w':
				W_command (root, text, nlines);			/* lista as linhas em que as palavras ocorrem consecutivamente */
				break;

			case 'h':
				H_command (root);						/* devolve todas as palavras e as suas ocorrencias */
				break;

			default:
				return root;
		}
	
	return root;
}

Link F_command (Link root)									
{		
	char *line, *word;

	line = getline();
	word = strtok (line, separators);
	root = deleteWord (root, word);							/* apaga o node respectivo na árvore */

	free (line);
	return root;	
}

void L_command (Link root, char **text, int nlines) 		/* lista todas a linhas em que a palavra ocorre */
{
	int i, *printedLines, nPrintedLines = 0;
	Link node;
	char *line, *word;

	line = getline();										/* obtém a palavra a listar */
	word = strtok (line, separators);
	word  = convertLower (word);							
	node = lookUp (root, word);								/* procura o node respectivo na árvore */

	if (node == NULL)	
	{	
		free (line);
		return;											
	}

	printedLines = (int*) malloc (sizeof(int)*nlines);

	for (i = 0; i < node->occurrences; ++i)  											
		if (testLine (printedLines, nPrintedLines, node->lines[i]))					/* imprime cada linha uma vez */
		{
			printf("%d %s\n", node->lines[i], text[node->lines[i]-1]);
			printedLines[nPrintedLines++] = node->lines[i];							/* guarda o nº da linha imprimida para não a imprimir duas vezes*/
		}

	free (line);
	free (printedLines);
}

void W_command (Link root, char **text, int nlines)
{
	int i, e, *printedLines, nPrintedLines = 0;
	Link  Node1, Node2;
	char *line, *word1, *word2;

						
	line = getline();									/* obtém a linha com os argumentos */
	word1 = strtok (line, separators);
	word2 = strtok (NULL, separators);
	word1 = convertLower (word1);						/* a comparação tem de ser case-insensitive */
	word2 = convertLower (word2);

	Node1 = lookUp (root, word1);										/* obtém o node da árvore respectivo á 1ª palavra */
	Node2 = lookUp (root, word2);										/* obtém o node da árvore respectivo á 2ª palavra */


	if (Node1 == NULL || Node2 == NULL)									/* se algum dos nodes tiver sido apagado ou nunca tiver sido inserido */
	{
		free (line);
		return;
	}
	
	printedLines = (int*) malloc (sizeof(int)*nlines);						/* aloca memória para guardar n linhas */


	for (i = 0; i < Node1->occurrences; ++i)												
		for (e = 0; e < Node2->occurrences; ++e)											/* testa se as linhas são iguais e se não foi já imprimida */
			if (Node1->lines[i] == Node2->lines[e] && testLine (printedLines, nPrintedLines, Node1->lines[i])) 	/* testa se as posições são adjacentes */
				if (Node1->positions[i] == (Node2->positions[e] - 1) || Node1->positions[i] == (Node2->positions[e] + 1))
					{
						printf("%d %s\n",Node1->lines[i], text[Node1->lines[i]-1]);				/* os indices do texto começam em 0 e as linhas em 1 */
						printedLines[nPrintedLines++] = Node1->lines[i];						/* guarda o nº da linha imprimida para não a imprimir duas vezes*/
					}

	free (line);
	free (printedLines);

}

int testLine (int *printedLines, int nPrintedLines, int line)
{

	while (nPrintedLines)												/* testa se a linha a ser imprimida já se encontra no vector */
		if (printedLines[--nPrintedLines] == line)						/* de linhas préviamente imprimidas */
			return 0;

	return 1;
}

void H_command (Link root)
{
	traverse(root);										/* devolve todas as palavras e numero de ocorrencias */
	getchar();											/* apanha o '\n' */
}

void S_command (char** text, int nlines)
{
	int i;

	for (i = 0; i < nlines; ++i)								/* imprime o texto original inalterado */
		printf("%s\n", text[i]);			

	getchar();            										/* apanha o '\n' */
}

char* getline ()
{
	int i;
	char c, line[MAXCHARS], *p_line;

	for (i = 0; (c = getchar()) != '\n'; ++i)			/* lê uma linha do terminal */
		line[i] = c;

	line[i] = '\0';

	p_line = (char*) malloc (sizeof(char)*(++i)); 
	strcpy (p_line, line);								/* aloca a memória necessária para a linha */		

	return p_line;										/* retorna um char* com a linha */
}

Link insertLines (char **text, int nlines, Link root)
{	
	int i, position = 1;
	Word line, aux_word;

	for (i = 0; i < nlines; ++i)
	{		
		line = getline();										/* lê uma linha */			
		text[i] = (char*) malloc (sizeof(char)*(strlen(line)+1));	
		strcpy(text[i], line);									/* insere a linha no texto */

		aux_word = strtok (line, separators);				/* parte a linha em palavras */

		while (aux_word != NULL) 
		{
			aux_word = convertLower (aux_word);
			root = insertNode (root, aux_word, i+1, position);    /* insere as palavras na árvore */
			aux_word = strtok (NULL, separators);			
			++position;											  /* a posição da palavra na linha */
		}
		position = 1;
		free(line);
	}
	return root;											  /* devolve a raiz com a árvore actualizada */
}

Word convertLower (Word word)
{
	int i;												/* converte os caracteres alfabéticos maiúsculos para minúsculos */

	for(i = 0; word[i] != '\0'; ++i)
 		word[i] = tolower(word[i]);

 	return word;

}

void erase (Link root, char** text, int nlines)
{
	int i;

	for (i = 0; i < nlines; ++i)
		free(text[i]);										/* liberta toda a memória alocada pelo programa */

	free(text);
	deleteTree(root);
}
