/**
* @author	Jos� Pedro Louren�o (23496)
* @email	a23496@alunos.ipca.pt
* @date		01/04/2022
* @file		DataManagement.h
*
* @brief	Ficheiro com todas as declara��es necess�rias para a execu��o do projeto
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#pragma once

#pragma region structs
/**
* @brief	Estrutura de dados para a lista (em mem�ria) de Trabalhos
*/
typedef struct Job{
	int id;
	struct Job* next;
	struct Job* jobs;
} Job;

/**
 * @brief	Estrutura de dados para armazenar em ficheiro a lista de trabalhos
 */
typedef struct FileJob{
	int id;
} FileJob;

/**
* @brief	Estrutura de dados para a lista (em mem�ria) de M�quinas
*/
typedef struct Machine{
	int id;
	bool isBusy;
	struct Machine* next;
	struct Machine* machines;
} Machine;

/**
* @brief	Estrutura de dados para armazenar em ficheiro a lista de m�quinas
*/
typedef struct FileMachine{
	int id;
	bool isBusy; // se a m�quina est� ou n�o em utiliza��o
} FileMachine;

/**
* @brief	Estrutura de dados para a lista (em mem�ria) de Executions das Opera��es das M�quinas
*/
typedef struct Execution{ 
	int operationPosition;
	int machineID;
	int runtime; // unidades de tempo necess�ria para a execu��o da opera��o
	struct Execution* next;
	struct Execution* executions;
} Execution;

/**
* @brief	Estrutura de dados para armazenar em ficheiro a lista de execu��es das opera��es em m�quinas
*/
typedef struct FileExecution{
	int operationPosition;
	int machineID;
	int runtime;
} FileExecution;

/**
* @brief	Estrutura de dados para a lista (em mem�ria) de opera��es
*/
typedef struct Operation{
	int position; // posi��o da opera��o (se � a 1�, 2�, 3�... a ser executada)
	struct Operation* next;
	struct Operation* operations;
} Operation;

/**
* @brief	Estrutura de dados para armazenar em ficheiro a lista de opera��es
*/
typedef struct FileOperation{
	int position;
} FileOperation;
#pragma endregion

#pragma region Assinaturas de Fun��es

/**
* @brief	Sobre trabalhos
*/
Job* newJob(int id);
Job* insertJobAtStart(Job* head, Job* new);
bool writeJobs(char fileName[], Job* head);
Job* readJobs(char fileName[]);
bool freeJobs(Job* head);
int getMinTimeToCompleteJob(Operation* operations, Execution* executions, Execution** minexecutions);
int getMaxTimeToCompleteJob(Operation* operations, Execution* executions, Execution** maxexecutions);

/**
* @brief	Sobre m�quinas
*/
Machine* newMachine(int id);
Machine* InsertMachineInList(Machine* head, Machine* new);
bool writeMachines(char fileName[], Machine* head);
Machine* readMachines(char fileName[]);
bool freeMachines(Machine* head);
bool displayMachines(Machine* head);

/**
* @brief	Sobre opera��es
*/
Operation* newOperation(int position);
Operation* InsertOperationInList(Operation* head, Operation* new);
bool updatePosition(Operation** operations, int oldPosition, int newPosition);
bool deleteOperation(Operation** head, int id);
bool writeOperations(char fileName[], Operation* head);
Operation* readOperations(char fileName[]);
bool freeOperations(Operation* head);
bool displayOperations(Operation* head);
float getAverageTimeToCompleteOperation(Execution* head, int operationID);
bool updatePosition(Operation** operations, int oldPosition, int newPosition);
Execution* SortExecutionsByOperation(Execution* head);

/**
* @brief	Sobre execu��es
*/
Execution* newExecution(int operationPosition, int machineID, int runtime);
Execution* InsertExecutionInList(Execution* head, Execution* new);
Execution* InsertExecutionByOperation(Execution* head, Execution* new);
bool updateRuntime(Execution** head, int operationID, int machineID, int runtime);
bool deleteExecutionByOperation(Execution** head, int operationID);
bool writeExecutions(char fileName[], Execution* head);
Execution* readExecutions(char fileName[]);
bool freeExecutions(Execution* head);
bool displayExecutions(Execution* head);
bool searchExecution(Execution* head, int operationID, int machineID);
bool searchExecutionByOperation(Execution* head, int operationID);

/**
 * @brief	Carregar dados para listas
 */
void loadData(Job** jobs, Machine** machines, Operation** operations, Execution** executions);

/**
 * @brief	Carregar dados para a consola
 */
void ShowData(Execution* executions);

#pragma endregion
