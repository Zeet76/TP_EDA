/**
* @author	Jos� Pedro Louren�o (23496)
* @email	a23496@alunos.ipca.pt
* @data		01/04/2022
* @file		MainProgram.c
* 
* @brief	Ficheiro principal da aplica��o, com menu e chamada de fun��es
* 
* ************************************************************************************************************************
*
* Software desenvolvido para a proposta de escalonamento para a produ��o de um produto que envolve v�rias opera��es
* e a utiliza��o de v�rias m�quinas, para minimizar o tempo necess�rio na sua produ��o.
* @see https://github.com/Zeet76/TP_EDA.git
*/

#include"DataManagement.h"

/**
* @brief	Fun��o principal do programa
*/
void main() {
	setlocale(LC_ALL, "Portuguese"); // permitir caracteres especiais na consola
	
	Job* jobs = NULL;
	Operation* operations = NULL;
	Machine* machines = NULL;
	Execution* executions = NULL;

#pragma region Funcionalidade 1: Defini��o de uma estrutura de dados din�mica para a representa��o de um job com um conjunto finito de n opera��es;
	printf("FUNCIONALIDADE 1: Defini��o de uma estrutura de dados din�mica para a representa��o de um job com um conjunto finito de n opera��es;\n\n");

	loadData(&jobs, &machines, &operations, &executions);
	
	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 2: Armazenamento/leitura de ficheiro de texto com representa��o de um job;
	printf("\nFUNCIONALIDADE 2: Armazenamento/leitura de ficheiro de texto com representa��o de um job;\n\n");

	// guardar os dados em ficheiros
	writeJobs("jobs", jobs);
	writeMachines("machines", machines);
	writeOperations("operations", operations);
	writeExecutions("executions", executions);
	printf("Dados exportados com sucesso!\n");

	// libertar mem�ria das listas anteriores, para serem lidas dos ficheiros
	freeJobs(jobs);
	freeMachines(machines);
	freeOperations(operations);
	freeExecutions(executions);

	// depois de libertar mem�ria, definir listas como NULL para evitar poss�veis erros
	jobs = NULL;
	machines = NULL;
	operations = NULL;
	executions = NULL;

	// ler dados de ficheiros
	jobs = readJobs("jobs");
	machines = readMachines("machines");
	operations = readOperations("operations");
	executions = readExecutions("executions");
	
	printf("Dados importados com sucesso!\n\n");
#pragma endregion


#pragma region Funcionalidade 3: Inser��o de uma nova opera��o;
	printf("\nFUNCIONALIDADE 3: Inser��o de uma nova opera��o;\n\n");

	//inserir nova opera��o
	Operation* operation = NULL;
	operation = newOperation(5);
	operations = InsertOperationInList(operations, operation);

	//inserir nova execu��o de uma opera��o
	Execution* execution = NULL;
	execution = newExecution(5, 10, 10);
	executions = InsertExecutionInList(executions, execution);

	//guardar as novas inser��es em ficheiros
	writeOperations("operations", operations);
	writeExecutions("executions", executions);
	
	//mostrar dados na consola
	printf("Inser��o de uma nova opera��o com sucesso!\n\n");

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 4: Remo��o de uma determinada opera��o;
	printf("\nFUNCIONALIDADE 4: Remo��o de uma determinada opera��o;\n\n");

	// remover opera��o
	deleteOperation(&operations, 5);
	printf("Opera��o removida com sucesso!\n");

	bool allFound = false;
	while (allFound == false) // enquanto que encontrar opera��es, remover as execu��es de opera��es associadas
	{
		if (searchExecutionByOperation(executions, 5))
		{
			// remover execu��o
			deleteExecutionByOperation(&executions, 5);
			printf("Execu��o associada � opera��o removida com sucesso!\n\n\n");
		}
		else allFound = true;
	}

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 5: Altera��o de uma determinada opera��o;
	printf("\nFUNCIONALIDADE 5: Altera��o de uma determinada opera��o;\n\n");
	
	// alterar a posi��o da opera��o num determinado job (...; posi��o antiga; posi��o nova
	updatePosition(&operations, 4, 2);
	printf("Posi��o da opera��o alterada com sucesso!\n");

	// alterar o tempo necess�rio para uma execu��o da opera��o(...; operation_position; machine_id; new_runtime)
	updateRuntime(&executions, 2, 2, 10);
	printf("Tempo necess�rio para a execu��o da opera��o alterada com sucesso!\n\n");

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 6: Determina��o da quantidade m�nima de unidades de tempo necess�rias para completar o job e listagem das respetivas opera��es;
	printf("\nFUNCIONALIDADE 6: Determina��o da quantidade m�nima de unidades de tempo necess�rias para completar o job e listagem das respetivas opera��es;\n\n");
	
	// obter o tempo m�nimo para completar um job e as respetivas opera��es
	Execution* minExecutions = NULL;
	int minTime = getMinTimeToCompleteJob(operations, executions, &minExecutions);

	// mostrar resultados
	printf("Menor tempo necess�rio para completar o trabalho � %d!\n", minTime);
	printf("Opera��es com menor tempo:\n");
	minExecutions = SortExecutionsByOperation(minExecutions);
	displayExecutions(minExecutions);
#pragma endregion


#pragma region Funcionalidade 7: Determina��o da quantidade m�xima de unidades de tempo necess�rias para completar o job e listagem das respetivas opera��es;
	printf("\nFUNCIONALIDADE 7: Determina��o da quantidade m�xima de unidades de tempo necess�rias para completar o job e listagem das respetivas opera��es;\n\n");
	
	// obter o tempo m�ximo para completar um job
	Execution* maxExecutions = NULL;
	int maxTime = getMaxTimeToCompleteJob(operations, executions, 1, &maxExecutions);

	// mostrar resultados
	printf("Maior tempo necess�rio para completar o trabalho(ID: %d) � %d!\n", 1, maxTime);
	printf("Opera��es com maior tempo:\n");
	maxExecutions = SortExecutionsByOperation(maxExecutions);
	displayExecutions(maxExecutions);
#pragma endregion


#pragma region Funcionalidade 8: Determina��o da quantidade m�dia de unidades de tempo necess�rias para completar uma opera��o, considerando todas as alternativas poss�veis;
	printf("\nFUNCIONALIDADE 8: Determina��o da quantidade m�dia de unidades de tempo necess�rias para completar uma opera��o, considerando todas as alternativas poss�veis;\n\n");

	// obter o tempo m�dio para completar uma opera��o
	float average = getAverageTimeToCompleteOperation(executions, 1);

	// mostrar resultado
	printf("M�dia de tempo necess�rio para completar a opera��o(ID: %d) � %.2f!\n", 1, average);
#pragma endregion

}



