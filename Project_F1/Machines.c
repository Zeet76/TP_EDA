/**
* Autor:                José Pedro Lourenço (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descrição:            Ficheiro com funções relacionadas com operações de "Máquinas"
*/

#include "DataManagement.h"

/**
* @brief	Criar nova máquina
* @param	id		Identificador da máquina
* @return	Nova máquina
*/
Machine* newMachine(int id){
	Machine* new = (Machine*)malloc(sizeof(Machine));

	new->id = id;
	new->next = NULL;

	return new;
}

/**
* @brief	Inserir nova máquina no início da lista de máquinas
* @param	head	Lista de máquinas
* @param	new		Nova máquina
* @return	Lista de máquinas atualizada
*/
Machine* InsertMachineInList(Machine* head, Machine* new){

	if (head == NULL) head = new; // se a lista estiver vazia
	else // se existir algum elemento na lista
	{
		new->next = head;
		head = new;
	}
	return head;
}

/**
* @brief	Armazenar lista de máquinas em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de máquinas
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeMachines(char fileName[], Machine* head) {
	if (head == NULL) return false; // se lista está vazia

	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se não foi possível abrir o ficheiro

	Machine* current = head;
	FileMachine currentMachine;	//para gravar em ficheiro
	while (current) //escrever todos os elementos da lista no ficheiro
	{
		currentMachine.id = current->id;

		fwrite(&currentMachine, sizeof(FileMachine), 1, file);
		current = current->next;
	}

	fclose(file);
	return true;
}

/**
* @brief	Ler lista de máquinas de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de máquinas
*/
Machine* readMachines(char fileName[]){
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se não foi possível abrir o ficheiro

	Machine* head = NULL;
	Machine* current = NULL;
	FileMachine currentMachine; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentMachine, sizeof(FileMachine), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newMachine(currentMachine.id);
		head = InsertMachineInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de máquinas da memória
* @param	head	Lista de máquinas
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool freeMachines(Machine* head){
	if (head == NULL) return false; // se lista está vazia
	
	Machine* current = NULL;

	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}

	return true;
}

/**
* @brief	Mostrar a lista de máquinas na consola
* @param	head	Lista de máquinas
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool displayMachines(Machine* head) {
	if (head == NULL) return false; // se lista está vazia

	Machine* current = head;

	while (current != NULL)
	{
		printf("ID: %d\n", current->id);
		current = current->next;
	}

	printf("\n");
	return true;
}