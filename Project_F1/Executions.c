/**
* Autor:                José Pedro Lourenço (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descrição:            Ficheiro com funções relacionadas com operações de "Execuções"
*/

#include "DataManagement.h"

/**
* @brief	Criar nova execução
* @param	operationPosition	Identificador da operação
* @param	machineID			Identificador da máquina
* @param	runtime				Unidades de tempo necessárias para a execução da operação
* @return	Nova execução
*/
Execution* newExecution(int operationPosition, int machineID, int runtime) {

	Execution* new = (Execution*)malloc(sizeof(Execution));
	if (new == NULL) // se não houver memória para alocar
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
* @brief	Inserir nova execução no início da lista de execuções
* @param	head	Lista de execuções
* @param	new		Nova execução
* @return	Lista de execuções atualizada
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
* @brief	Armazenar lista de execuções em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeExecutions(char fileName[], Execution* head){
	if (head == NULL) return false; // se lista está vazia

	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se não foi possível abrir o ficheiro

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
* @brief	Ler lista de execuções de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de execuçõess
*/
Execution* readExecutions(char fileName[]) {
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se não foi possível abrir o ficheiro

	Execution* head = NULL;
	Execution* current = NULL;
	FileExecution currentExecution; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentExecution, sizeof(FileExecution), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newExecution(currentExecution.operationPosition, currentExecution.machineID, currentExecution.runtime);
		head = InsertExecutionInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de operações da memória
* @param	head	Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
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
* @brief	Procurar por uma execução através do identificador da operação, na lista de execuções
* @param	head			Lista de execuções
* @param	operationID		Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
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
* @brief	Remover execução pelo identificador da operação
* @param	head				Apontador para a lista de execuções
* @param	operationPosition	Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteExecutionByOperation(Execution** head, int operationPosition){
	if (*head == NULL) return false; // se a lista estiver vazia

	Execution* current = *head;
	Execution* previous = NULL;

	if (current && current->operationPosition == operationPosition) { // se o elemento que será apagado é o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current&& current->operationPosition != operationPosition) // procurar o elemento a ser apagado
	{
		previous = current;
		current = current->next;
	}

	if (current == NULL) return false; // se o elemento não foi encontrado

	previous->next = current->next; // desassociar o elemento da lista
	free(current);

	return true;
}

/**
* @brief	Atualizar as unidades de tempo necessárias para a execução de uma operação
* @param	head			Apontador para a lista de execuções
* @param	operationPOsition		Identificador da operação
* @param	machineID		Identificador da máquina
* @param	runtime			Unidades de tempo
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool updateRuntime(Execution** head, int operationPosition, int machineID, int runtime){
	if (*head == NULL) return false; // se lista está vazia

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
* @brief	Inserir execução ordenada pelo ID da operação na lista de execuções
* @param	head	Lista de execuções
* @param	new		Nova execução
* @return	Lista de execuções atualizada
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
* @brief	Procurar por uma execução na lista de execuções
* @param	head			Lista de execuções
* @param	operationID		Identificador da operação
* @param	machineID		Identificador da máquina
* @return	Booleano para o resultado da função (se funcionou ou não)
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
* @brief	Mostrar a lista de execuções na consola
* @param	head	Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
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