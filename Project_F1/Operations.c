/**
* Autor:                José Pedro Lourenço (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descrição:            Ficheiro com funções relacionadas com operações de "Operações"
*/

#include "DataManagement.h"

/**
* @brief	Criar nova operação
* @param	position	Posição da operação relativamente ao trabalho
* @return	Nova operação
*/
Operation* newOperation(int position) {
	Operation* new = (Operation*)malloc(sizeof(Operation));

	new->position = position;
	new->next = NULL;

	return new;
}

/**
* @brief	Inserir nova operação no início da lista de operações
* @param	head	Lista de operações
* @param	new		Nova operação
* @return	Lista de operações atualizada
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
* @brief	Armazenar lista de operações em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeOperations(char fileName[], Operation* head) {
	if (head == NULL) return false; // se lista está vazia

	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se não foi possível abrir o ficheiro

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
* @brief	Ler lista de operações de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de operações
*/
Operation* readOperations(char fileName[]){
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se não foi possível abrir o ficheiro

	Operation* head = NULL;
	Operation* current = NULL;
	FileOperation currentOperation; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentOperation, sizeof(FileOperation), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newOperation(currentOperation.position);
		head = InsertOperationInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de operações da memória
* @param	head	Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool freeOperations(Operation* head){
	if (head == NULL) return false; // se lista está vazia
	
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
* @brief	Remover uma operação da lista de operações
* @param	head		Apontador para a lista de operações
* @param	position	Identificador da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool deleteOperation(Operation** head, int position)
{
	if (*head == NULL) return false;

	Operation* current = *head;
	Operation* previous = NULL;

	if (current && current->position == position) { // se o elemento que será apagado é o primeiro da lista
		*head = current->next;
		free(current);
		return true;
	}

	while (current && current->position != position) // procurar o elemento a ser apagado
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
* @brief	Trocar a posição de uma operação para outra e vice-versa, num determinado trabalho
* @param	operations		Apontador para a lista de operações
* @param	oldPosition		Posição antiga da operação
* @param	newPosition		Nova posição da operação
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool updatePosition(Operation** operations, int oldPosition, int newPosition)
{
	if (*operations == NULL) return false;// se as listas estiverem vazias

	if (oldPosition == newPosition) return false;// se as posições forem as mesmas


	Operation* currentOperation1 = *operations;
	Operation* previousOperation1 = NULL; // para a antiga
	Operation* currentOperation2 = *operations;
	Operation* previousOperation2 = NULL; // para a nova

	// procurar pela antiga posição
	while (currentOperation1 && currentOperation1->position != oldPosition)
	{
		previousOperation1 = currentOperation1;
		currentOperation1 = currentOperation1->next;
	}

	// procurar pela nova posição
	while (currentOperation2 && currentOperation2->position != newPosition)
	{
		previousOperation2 = currentOperation2;
		currentOperation2 = currentOperation2->next;
	}

	// se as operações relativas à antiga ou nova posição não foram encontradas
	if (currentOperation1 == NULL || currentOperation2 == NULL) return false;

	if (previousOperation1) previousOperation1->next = currentOperation2; // se a operação anterior não for o head da lista
	else *operations = currentOperation2; // senão fazer que operação anterior seja o head

	if (previousOperation2) previousOperation2->next = currentOperation1; // se a operação nova não for o head da lista
	else *operations = currentOperation1;// senão fazer com que operação anterior seja o head

	// trocar elementos
	Operation* tempOperation = currentOperation2->next;
	currentOperation2->next = currentOperation1->next;
	currentOperation1->next = tempOperation;

	return true;
}

/**
* @brief	Ordenar lista de execuções por ordem crescente do identificador da operação
* @param	head		Lista de execuções
* @return	Booleano para o resultado da função (se funcionou ou não)
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
* @brief	Obter a média de tempo necessário para completar uma operação, considerando todas as alternativas possíveis
* @param	head					Lista de execuções
* @param	operationPosition		Identificador da operação
* @return	Valor da média de tempo
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
		if (current->operationPosition == operationPosition) // se encontrar a execucação de operação relativa à operação
		{
			sum += current->runtime;
			numberOfExecutions++;
		}
		current = current->next;
	}

	if (numberOfExecutions > 0) average = sum / numberOfExecutions; // para não permitir divisão por 0

	return average;
}

/**
* @brief	Mostrar a lista de operações na consola
* @param	head	Lista de operações
* @return	Booleano para o resultado da função (se funcionou ou não)
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