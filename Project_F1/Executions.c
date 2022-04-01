/**
* Autor:                Jos� Pedro Louren�o (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descri��o:            Ficheiro com fun��es relacionadas com opera��es de "Execu��es"
*/

#include "DataManagement.h"

/**
* @brief	Criar nova execu��o
* @param	operationPosition	Identificador da opera��o
* @param	machineID			Identificador da m�quina
* @param	runtime				Unidades de tempo necess�rias para a execu��o da opera��o
* @return	Nova execu��o
*/
Execution* newExecution(int operationPosition, int machineID, int runtime) {

	Execution* new = (Execution*)malloc(sizeof(Execution));
	if (new == NULL) // se n�o houver mem�ria para alocar
	{
		return NULL;
	}

	new->operationPosition = operationPosition;
	new->machineID = machineID;
	new->runtime = runtime;
	new->next = NULL;

	return new;
}

/**
* @brief	Inserir nova execu��o no in�cio da lista de execu��es
* @param	head	Lista de execu��es
* @param	new		Nova execu��o
* @return	Lista de execu��es atualizada
*/
Execution* InsertExecutionInList(Execution* head, Execution* new) {
	
	if (head == NULL) head = new; // se a lista estiver vazia
	else // se existir algum elemento na lista
	{
		new->next = head;
		head = new;
	}

	return head;
}

/**
* @brief	Armazenar lista de execu��es em ficheiro bin�rio
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de execu��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool writeExecutions(char fileName[], Execution* head){
	if (head == NULL) return false; // se lista est� vazia

	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se n�o foi poss�vel abrir o ficheiro

	Execution* current = head;
	FileExecution currentExecution;	//para gravar em ficheiro
	while (current) //escrever todos os elementos da lista no ficheiro
	{
		currentExecution.operationPosition = current->operationPosition;
		currentExecution.machineID = current->machineID;
		currentExecution.runtime = current->runtime;

		fwrite(&currentExecution, sizeof(FileExecution), 1, file);
		current = current->next;
	}

	fclose(file);

	return true;
}

/**
* @brief	Ler lista de execu��es de ficheiro bin�rio
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de execu��ess
*/
Execution* readExecutions(char fileName[]) {
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se n�o foi poss�vel abrir o ficheiro

	Execution* head = NULL;
	Execution* current = NULL;
	FileExecution currentExecution; // � a mesma estrutura mas sem o campo *next, uma vez que esse campo n�o � armazenado no ficheiro

	while (fread(&currentExecution, sizeof(FileExecution), 1, file)) // l� todos os registos do ficheiro e guarda na lista
	{
		current = newExecution(currentExecution.operationPosition, currentExecution.machineID, currentExecution.runtime);
		head = InsertExecutionInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de opera��es da mem�ria
* @param	head	Lista de opera��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool freeExecutions(Execution* head){
	if (head == NULL) return false; // se a lista estiver vazia
	
	Execution* current = NULL;

	while (head)
	{
		current = head;
		head = head->next;
		free(current);
	}

	return true;
}

/**
* @brief	Procurar por uma execu��o atrav�s do identificador da opera��o, na lista de execu��es
* @param	head			Lista de execu��es
* @param	operationID		Identificador da opera��o
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool searchExecutionByOperation(Execution* head, int operationPosition) {
	if (head == NULL) return false; // se a lista estiver vazia

	Execution* current = head;

	while (current != NULL)
	{
		if (current->operationPosition == operationPosition) return true;

		current = current->next;
	}

	return false;
}

/**
* @brief	Remover execu��o pelo identificador da opera��o
* @param	head				Apontador para a lista de execu��es
* @param	operationPosition	Identificador da opera��o
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool deleteExecutionByOperation(Execution** head, int operationPosition){
	if (*head == NULL) return false; // se a lista estiver vazia

	Execution* current = *head;
	Execution* previous = NULL;

	if (current && current->operationPosition == operationPosition) { // se o elemento que ser� apagado � o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current&& current->operationPosition != operationPosition) // procurar o elemento a ser apagado
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
* @brief	Atualizar as unidades de tempo necess�rias para a execu��o de uma opera��o
* @param	head			Apontador para a lista de execu��es
* @param	operationPOsition		Identificador da opera��o
* @param	machineID		Identificador da m�quina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool updateRuntime(Execution** head, int operationPosition, int machineID, int runtime){
	if (*head == NULL) return false; // se lista est� vazia

	Execution* current = *head;

	while (current != NULL)
	{
		if (current->operationPosition == operationPosition && current->machineID == machineID)
		{
			current->runtime = runtime;
			return true;
		}
		current = current->next;
	}

	return false;
}

/**
* @brief	Inserir execu��o ordenada pelo ID da opera��o na lista de execu��es
* @param	head	Lista de execu��es
* @param	new		Nova execu��o
* @return	Lista de execu��es atualizada
*/
Execution* InsertExecutionByOperation(Execution* head, Execution* new){

	if (head == NULL) head = new;// se a lista estiver vazia

	else
	{
		Execution* current = head;
		Execution* previous = NULL;

		// enquanto que atual tem o ID menor que a nova
		while (current&& current->operationPosition < new->operationPosition)
		{
			previous = current;
			current = current->next;
		}

		if (previous == NULL)
		{
			new->next = head;
			head = new; // inserir no meio
		}
		else
		{
			previous->next = new;
			new->next = current; // inserir no fim
		}
	}

	return head;
}

/**
* @brief	Procurar por uma execu��o na lista de execu��es
* @param	head			Lista de execu��es
* @param	operationID		Identificador da opera��o
* @param	machineID		Identificador da m�quina
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool searchExecution(Execution* head, int operationPosition, int machineID){
	if (head == NULL) return false;// se a lista estiver vazia

	Execution* current = head;

	while (current != NULL)
	{
		if (current->operationPosition == operationPosition && current->machineID == machineID)
		{
			return true;
		}
		current = current->next;
	}

	return false;
}

/**
* @brief	Mostrar a lista de execu��es na consola
* @param	head	Lista de execu��es
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool displayExecutions(Execution* head) {
	if (head == NULL) return false; // se a lista estiver vazia

	Execution* current = head;

	while (current)
	{
		printf("Operation ID: %d, Machine ID: %d, Execution Time: %d;\n", current->operationPosition, current->machineID, current->runtime);
		current = current->next;
	}

	printf("\n");
	return true;
}