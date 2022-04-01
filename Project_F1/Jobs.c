/**
* Autor:                José Pedro Lourenço (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descrição:            Ficheiro com funções relacionadas com operações de "Trabalhos"
*/

#include "DataManagement.h"

/**
* @brief	Criar novo trabalho
* @param	id	Identificador do trabalho
* @return	Novo trabalho
*/
Job* newJob(int id) {
	Job* new = (Job*)malloc(sizeof(Job));

	new->id = id;
	new->next = NULL;

	return new;
}

/**
* @brief	Inserir novo trabalho no início da lista de trabalhos
* @param	head	Lista de trabalhos
* @param	new		Novo trabalho
* @return	Lista de trabalhos atualizada
*/
Job* InsertJobInList(Job* head, Job* new) {
	
	if (head == NULL) head = new; // se a lista estiver vazia
	else // se existir algum elemento na lista
	{
		new->next = head;
		head = new;
	}

	return head;
}

/**
* @brief	Armazenar lista de trabalhos em ficheiro binário
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de trabalhos
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool writeJobs(char fileName[], Job* head){
	if (head == NULL) return false; // se lista está vazia
	
	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se não foi possível abrir o ficheiro
	
	Job* current = head;
	FileJob currentJob;	//para gravar em ficheiro
	while (current) //escrever todos os elementos da lista no ficheiro
	{
		currentJob.id = current->id;

		fwrite(&currentJob, sizeof(FileJob), 1, file);
		current = current->next;
	}

	fclose(file);

	return true;
}

/**
* @brief	Ler lista de trabalhos de ficheiro binário
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de trabalhos
*/
Job* readJobs(char fileName[]){
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se não foi possível abrir o ficheiro

	Job* head = NULL;
	Job* current = NULL;
	FileJob currentJob; // é a mesma estrutura mas sem o campo *next, uma vez que esse campo não é armazenado no ficheiro

	while (fread(&currentJob, sizeof(FileJob), 1, file)) // lê todos os registos do ficheiro e guarda na lista
	{
		current = newJob(currentJob.id);
		head = InsertJobInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de trabalhos da memória
* @param	head	Lista de trabalhos
* @return	Booleano para o resultado da função (se funcionou ou não)
*/
bool freeJobs(Job* head){
	if (head == NULL) return false; // se lista está vazia

	Job* current = NULL;

	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}

	return true;
}

/**
* @brief	Obter o mínimo de tempo necessário para completo um trabalho e as respetivas execuções
* @param	operations		Lista de operações
* @param	executions		Lista de execuções
* @param	minExecutions	Apontador para a lista de execuções a ser devolvida, relativamente ao tempo mínimo
* @return	Quantidade de tempo
*/
int getMinTimeToCompleteJob(Operation* operations, Execution* executions, Execution** minExecutions)
{
	if (operations == NULL || executions == NULL) return -1;// se as listas estiverem vazias

	int time = 99999;
	int counter = 0;

	Operation* currentOperation = operations;
	Execution* currentExecution = executions;
	Execution* minExecution = NULL;

	while (currentOperation) // percorrer lista de operações
	{
			while (currentExecution) // percorrer lista de execução de operações
			{
				if (currentExecution->operationPosition == currentOperation->position) // se encontrar a execucação de operação relativa à operação
				{
					// guardar execução de operação com menor tempo de utilização
					if (currentExecution->runtime < time)
					{
						time = currentExecution->runtime;
						minExecution = newExecution(currentExecution->operationPosition, currentExecution->machineID, currentExecution->runtime);
					}
				}
				currentExecution = currentExecution->next;
			}

		*minExecutions = InsertExecutionInList(*minExecutions, minExecution);

		// repor lista percorrida (currentExecution), para que se for necessário voltar a percorrer o while da execução de operações de novo
		freeExecutions(currentExecution);
		currentExecution = NULL;
		currentExecution = executions;
		counter += time; // acumular o tempo de utilização de cada execução de operação
		time = 99999; // resetar tempo para a próxima iteração
		
		currentOperation = currentOperation->next;
	}

	return counter;
}

/**
* @brief	Obter o máximo de tempo necessário para completo um trabalho e as respetivas execuções
* @param	operations		Lista de operações
* @param	executions		Lista de execuções
* @param	maxExecutions	Apontador para a lista de execuções a ser devolvida, relativamente ao tempo máximo
* @return	Quantidade de tempo
*/
int getMaxTimeToCompleteJob(Operation* operations, Execution* executions, Execution** maxExecutions)
{
	if (operations == NULL || executions == NULL) return -1;// se as listas estiverem vazias

	int time = 0;
	int counter = 0;

	Operation* currentOperation = operations;
	Execution* currentExecution = executions;
	Execution* maxExecution = NULL;

	while (currentOperation != NULL) // percorrer lista de operações
	{
			while (currentExecution != NULL) // percorrer lista de execução de operações
			{
				if (currentExecution->operationPosition == currentOperation->position) // se encontrar a execucação de operação relativa à operação
				{
					// guardar execução de operação com maior tempo de utilização
					if (currentExecution->runtime > time)
					{
						time = currentExecution->runtime;
						maxExecution = newExecution(currentExecution->operationPosition, currentExecution->machineID, currentExecution->runtime);
					}
				}

				currentExecution = currentExecution->next;
			}

		*maxExecutions = InsertExecutionInList(*maxExecutions, maxExecution);

		// repor lista percorrida (currentExecution), para que se for necessário voltar a percorrer o while da execução de operações de novo
		freeExecutions(currentExecution);
		currentExecution = NULL;
		currentExecution = executions;
		counter += time; // acumular o tempo de utilização de cada execução de operação
		time = 0; // resetar tempo de utilização para a próxima iteração

		currentOperation = currentOperation->next;
	}

	return counter;
}