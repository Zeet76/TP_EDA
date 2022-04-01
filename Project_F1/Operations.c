/**
* Autor:                Jos� Pedro Louren�o (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descri��o:            Ficheiro com fun��es relacionadas com opera��es de "Opera��es"
*/

#include "DataManagement.h"

/**
* @brief	Criar nova opera��o
* @param	position	Posi��o da opera��o relativamente ao trabalho
* @return	Nova opera��o
*/
Operation* newOperation(int position) {
	Operation* new = (Operation*)malloc(sizeof(Operation));

	new->position = position;
	new->next = NULL;

	return new;
}

/**
* @brief	Inserir nova opera��o no in�cio da lista de opera��es
* @param	head	Lista de opera��es
* @param	new		Nova opera��o
* @return	Lista de opera��es atualizada
*/
Operation* InsertOperationInList(Operation* head, Operation* new) {
	
	if (head == NULL) head = new; // se a lista estiver vazia
	else // se existir algum elemento na lista
	{
		new->next = head;
		head = new;
	}

	return head;
}

/**
* @brief	Armazenar lista de opera��es em ficheiro bin�rio
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de opera��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool writeOperations(char fileName[], Operation* head) {
	if (head == NULL) return false; // se lista est� vazia

	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se n�o foi poss�vel abrir o ficheiro

	Operation* current = head;
	FileOperation currentOperation;	//para gravar em ficheiro
	while (current) //escrever todos os elementos da lista no ficheiro
	{
		currentOperation.position = current->position;

		fwrite(&currentOperation, sizeof(FileOperation), 1, file);
		current = current->next;
	}

	fclose(file);
	return true;
}

/**
* @brief	Ler lista de opera��es de ficheiro bin�rio
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de opera��es
*/
Operation* readOperations(char fileName[]){
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se n�o foi poss�vel abrir o ficheiro

	Operation* head = NULL;
	Operation* current = NULL;
	FileOperation currentOperation; // � a mesma estrutura mas sem o campo *next, uma vez que esse campo n�o � armazenado no ficheiro

	while (fread(&currentOperation, sizeof(FileOperation), 1, file)) // l� todos os registos do ficheiro e guarda na lista
	{
		current = newOperation(currentOperation.position);
		head = InsertOperationInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de opera��es da mem�ria
* @param	head	Lista de opera��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool freeOperations(Operation* head){
	if (head == NULL) return false; // se lista est� vazia
	
	Operation* current = NULL;

	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}

	return true;
}

/**
* @brief	Remover uma opera��o da lista de opera��es
* @param	head		Apontador para a lista de opera��es
* @param	position	Identificador da opera��o
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool deleteOperation(Operation** head, int position)
{
	if (*head == NULL) return false;

	Operation* current = *head;
	Operation* previous = NULL;

	if (current && current->position == position) { // se o elemento que ser� apagado � o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current && current->position != position) // procurar o elemento a ser apagado
	{
		previous = current;
		current = current->next;
	}

	if (current == NULL) return false; // se o elemento n�o foi encontrado

	previous->next = current->next; // desassociar o elemento da lista
	free(current);

	return true;
}

/**
* @brief	Trocar a posi��o de uma opera��o para outra e vice-versa, num determinado trabalho
* @param	operations		Apontador para a lista de opera��es
* @param	oldPosition		Posi��o antiga da opera��o
* @param	newPosition		Nova posi��o da opera��o
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool updatePosition(Operation** operations, int oldPosition, int newPosition)
{
	if (*operations == NULL) return false;// se as listas estiverem vazias

	if (oldPosition == newPosition) return false;// se as posi��es forem as mesmas


	Operation* currentOperation1 = *operations;
	Operation* previousOperation1 = NULL; // para a antiga
	Operation* currentOperation2 = *operations;
	Operation* previousOperation2 = NULL; // para a nova

	// procurar pela antiga posi��o
	while (currentOperation1 && currentOperation1->position != oldPosition)
	{
		previousOperation1 = currentOperation1;
		currentOperation1 = currentOperation1->next;
	}

	// procurar pela nova posi��o
	while (currentOperation2 && currentOperation2->position != newPosition)
	{
		previousOperation2 = currentOperation2;
		currentOperation2 = currentOperation2->next;
	}

	// se as opera��es relativas � antiga ou nova posi��o n�o foram encontradas
	if (currentOperation1 == NULL || currentOperation2 == NULL) return false;

	if (previousOperation1) previousOperation1->next = currentOperation2; // se a opera��o anterior n�o for o head da lista
	else *operations = currentOperation2; // sen�o fazer que opera��o anterior seja o head

	if (previousOperation2) previousOperation2->next = currentOperation1; // se a opera��o nova n�o for o head da lista
	else *operations = currentOperation1;// sen�o fazer com que opera��o anterior seja o head

	// trocar elementos
	Operation* tempOperation = currentOperation2->next;
	currentOperation2->next = currentOperation1->next;
	currentOperation1->next = tempOperation;

	return true;
}

/**
* @brief	Ordenar lista de execu��es por ordem crescente do identificador da opera��o
* @param	head		Lista de execu��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
Execution* SortExecutionsByOperation(Execution* head){
	//if (head == NULL) return head;// se a lista estiver vazia

	Execution* current = head;
	Execution* sorted = NULL;
	Execution* new = NULL;

	while (current != NULL)
	{
		new = newExecution(current->operationPosition, current->machineID, current->runtime);
		sorted = InsertExecutionByOperation(&sorted, new);
		current = current->next;
	}

	return sorted;
}

/**
* @brief	Obter a m�dia de tempo necess�rio para completar uma opera��o, considerando todas as alternativas poss�veis
* @param	head					Lista de execu��es
* @param	operationPosition		Identificador da opera��o
* @return	Valor da m�dia de tempo
*/
float getAverageTimeToCompleteOperation(Execution* head, int operationPosition)
{
	if (head == NULL) return -1.0f;// se a lista estiver vazia

	int sum = 0;
	float average = 0;
	float numberOfExecutions = 0;

	Execution* current = head;

	while (current)
	{
		if (current->operationPosition == operationPosition) // se encontrar a execuca��o de opera��o relativa � opera��o
		{
			sum += current->runtime;
			numberOfExecutions++;
		}
		current = current->next;
	}

	if (numberOfExecutions > 0) average = sum / numberOfExecutions; // para n�o permitir divis�o por 0

	return average;
}

/**
* @brief	Mostrar a lista de opera��es na consola
* @param	head	Lista de opera��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool displayOperations(Operation* head) {
	if (head == NULL) return false;// se a lista estiver vazia

	Operation* current = head;

	while (current != NULL)
	{
		printf("ID/Placement: %d\n", current->position);
		current = current->next;
	}

	printf("\n");
	return true;
}