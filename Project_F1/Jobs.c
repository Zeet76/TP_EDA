/**
* Autor:                Jos� Pedro Louren�o (23496)
* Email:                a23496@alunos.ipca.pt
* Data:                 01/04/2022
*
* Descri��o:            Ficheiro com fun��es relacionadas com opera��es de "Trabalhos"
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
* @brief	Inserir novo trabalho no in�cio da lista de trabalhos
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
* @brief	Armazenar lista de trabalhos em ficheiro bin�rio
* @param	fileName	Nome do ficheiro para armazenar a lista
* @param	head		Lista de trabalhos
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool writeJobs(char fileName[], Job* head){
	if (head == NULL) return false; // se lista est� vazia
	
	FILE* file = NULL;
	if ((file = fopen(fileName, "wb")) == NULL) return false; // se n�o foi poss�vel abrir o ficheiro
	
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
* @brief	Ler lista de trabalhos de ficheiro bin�rio
* @param	fileName	Nome do ficheiro para ler a lista
* @return	Lista de trabalhos
*/
Job* readJobs(char fileName[]){
	FILE* file;

	if ((file = fopen(fileName, "rb")) == NULL) return NULL; // se n�o foi poss�vel abrir o ficheiro

	Job* head = NULL;
	Job* current = NULL;
	FileJob currentJob; // � a mesma estrutura mas sem o campo *next, uma vez que esse campo n�o � armazenado no ficheiro

	while (fread(&currentJob, sizeof(FileJob), 1, file)) // l� todos os registos do ficheiro e guarda na lista
	{
		current = newJob(currentJob.id);
		head = InsertJobInList(head, current);
	}

	fclose(file);
	return head;
}

/**
* @brief	Libertar a lista de trabalhos da mem�ria
* @param	head	Lista de trabalhos
* @return	Booleano para o resultado da fun��o (se funcionou ou n�o)
*/
bool freeJobs(Job* head){
	if (head == NULL) return false; // se lista est� vazia

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
* @brief	Obter o m�nimo de tempo necess�rio para completo um trabalho e as respetivas execu��es
* @param	operations		Lista de opera��es
* @param	executions		Lista de execu��es
* @param	minExecutions	Apontador para a lista de execu��es a ser devolvida, relativamente ao tempo m�nimo
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

	while (currentOperation) // percorrer lista de opera��es
	{
			while (currentExecution) // percorrer lista de execu��o de opera��es
			{
				if (currentExecution->operationPosition == currentOperation->position) // se encontrar a execuca��o de opera��o relativa � opera��o
				{
					// guardar execu��o de opera��o com menor tempo de utiliza��o
					if (currentExecution->runtime < time)
					{
						time = currentExecution->runtime;
						minExecution = newExecution(currentExecution->operationPosition, currentExecution->machineID, currentExecution->runtime);
					}
				}
				currentExecution = currentExecution->next;
			}

		*minExecutions = InsertExecutionInList(*minExecutions, minExecution);

		// repor lista percorrida (currentExecution), para que se for necess�rio voltar a percorrer o while da execu��o de opera��es de novo
		freeExecutions(currentExecution);
		currentExecution = NULL;
		currentExecution = executions;
		counter += time; // acumular o tempo de utiliza��o de cada execu��o de opera��o
		time = 99999; // resetar tempo para a pr�xima itera��o
		
		currentOperation = currentOperation->next;
	}

	return counter;
}

/**
* @brief	Obter o m�ximo de tempo necess�rio para completo um trabalho e as respetivas execu��es
* @param	operations		Lista de opera��es
* @param	executions		Lista de execu��es
* @param	maxExecutions	Apontador para a lista de execu��es a ser devolvida, relativamente ao tempo m�ximo
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

	while (currentOperation != NULL) // percorrer lista de opera��es
	{
			while (currentExecution != NULL) // percorrer lista de execu��o de opera��es
			{
				if (currentExecution->operationPosition == currentOperation->position) // se encontrar a execuca��o de opera��o relativa � opera��o
				{
					// guardar execu��o de opera��o com maior tempo de utiliza��o
					if (currentExecution->runtime > time)
					{
						time = currentExecution->runtime;
						maxExecution = newExecution(currentExecution->operationPosition, currentExecution->machineID, currentExecution->runtime);
					}
				}

				currentExecution = currentExecution->next;
			}

		*maxExecutions = InsertExecutionInList(*maxExecutions, maxExecution);

		// repor lista percorrida (currentExecution), para que se for necess�rio voltar a percorrer o while da execu��o de opera��es de novo
		freeExecutions(currentExecution);
		currentExecution = NULL;
		currentExecution = executions;
		counter += time; // acumular o tempo de utiliza��o de cada execu��o de opera��o
		time = 0; // resetar tempo de utiliza��o para a pr�xima itera��o

		currentOperation = currentOperation->next;
	}

	return counter;
}