/**
* @author	José Pedro Lourenço (23496)
* @email	a23496@alunos.ipca.pt
* @data		01/04/2022
* @file		MainProgram.c
* 
* @brief	Ficheiro principal da aplicação, com menu e chamada de funções
* 
* ************************************************************************************************************************
*
* Software desenvolvido para a proposta de escalonamento para a produção de um produto que envolve várias operações
* e a utilização de várias máquinas, para minimizar o tempo necessário na sua produção.
* @see https://github.com/Zeet76/TP_EDA.git
*/

#include"DataManagement.h"

/**
* @brief	Função principal do programa
*/
void main() {
	setlocale(LC_ALL, "Portuguese"); // permitir caracteres especiais na consola
	
	Job* jobs = NULL;
	Operation* operations = NULL;
	Machine* machines = NULL;
	Execution* executions = NULL;

#pragma region Funcionalidade 1: Definição de uma estrutura de dados dinâmica para a representação de um job com um conjunto finito de n operações;
	printf("FUNCIONALIDADE 1: Definição de uma estrutura de dados dinâmica para a representação de um job com um conjunto finito de n operações;\n\n");

	loadData(&jobs, &machines, &operations, &executions);
	
	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 2: Armazenamento/leitura de ficheiro de texto com representação de um job;
	printf("\nFUNCIONALIDADE 2: Armazenamento/leitura de ficheiro de texto com representação de um job;\n\n");

	// guardar os dados em ficheiros
	writeJobs("jobs", jobs);
	writeMachines("machines", machines);
	writeOperations("operations", operations);
	writeExecutions("executions", executions);
	printf("Dados exportados com sucesso!\n");

	// libertar memória das listas anteriores, para serem lidas dos ficheiros
	freeJobs(jobs);
	freeMachines(machines);
	freeOperations(operations);
	freeExecutions(executions);

	// depois de libertar memória, definir listas como NULL para evitar possíveis erros
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


#pragma region Funcionalidade 3: Inserção de uma nova operação;
	printf("\nFUNCIONALIDADE 3: Inserção de uma nova operação;\n\n");

	//inserir nova operação
	Operation* operation = NULL;
	operation = newOperation(5);
	operations = InsertOperationInList(operations, operation);

	//inserir nova execução de uma operação
	Execution* execution = NULL;
	execution = newExecution(5, 10, 10);
	executions = InsertExecutionInList(executions, execution);

	//guardar as novas inserções em ficheiros
	writeOperations("operations", operations);
	writeExecutions("executions", executions);
	
	//mostrar dados na consola
	printf("Inserção de uma nova operação com sucesso!\n\n");

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 4: Remoção de uma determinada operação;
	printf("\nFUNCIONALIDADE 4: Remoção de uma determinada operação;\n\n");

	// remover operação
	deleteOperation(&operations, 5);
	printf("Operação removida com sucesso!\n");

	bool allFound = false;
	while (allFound == false) // enquanto que encontrar operações, remover as execuções de operações associadas
	{
		if (searchExecutionByOperation(executions, 5))
		{
			// remover execução
			deleteExecutionByOperation(&executions, 5);
			printf("Execução associada à operação removida com sucesso!\n\n\n");
		}
		else allFound = true;
	}

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 5: Alteração de uma determinada operação;
	printf("\nFUNCIONALIDADE 5: Alteração de uma determinada operação;\n\n");
	
	// alterar a posição da operação num determinado job (...; posição antiga; posição nova
	updatePosition(&operations, 4, 2);
	printf("Posição da operação alterada com sucesso!\n");

	// alterar o tempo necessário para uma execução da operação(...; operation_position; machine_id; new_runtime)
	updateRuntime(&executions, 2, 2, 10);
	printf("Tempo necessário para a execução da operação alterada com sucesso!\n\n");

	ShowData(executions);
#pragma endregion


#pragma region Funcionalidade 6: Determinação da quantidade mínima de unidades de tempo necessárias para completar o job e listagem das respetivas operações;
	printf("\nFUNCIONALIDADE 6: Determinação da quantidade mínima de unidades de tempo necessárias para completar o job e listagem das respetivas operações;\n\n");
	
	// obter o tempo mínimo para completar um job e as respetivas operações
	Execution* minExecutions = NULL;
	int minTime = getMinTimeToCompleteJob(operations, executions, &minExecutions);

	// mostrar resultados
	printf("Menor tempo necessário para completar o trabalho é %d!\n", minTime);
	printf("Operações com menor tempo:\n");
	minExecutions = SortExecutionsByOperation(minExecutions);
	displayExecutions(minExecutions);
#pragma endregion


#pragma region Funcionalidade 7: Determinação da quantidade máxima de unidades de tempo necessárias para completar o job e listagem das respetivas operações;
	printf("\nFUNCIONALIDADE 7: Determinação da quantidade máxima de unidades de tempo necessárias para completar o job e listagem das respetivas operações;\n\n");
	
	// obter o tempo máximo para completar um job
	Execution* maxExecutions = NULL;
	int maxTime = getMaxTimeToCompleteJob(operations, executions, 1, &maxExecutions);

	// mostrar resultados
	printf("Maior tempo necessário para completar o trabalho(ID: %d) é %d!\n", 1, maxTime);
	printf("Operações com maior tempo:\n");
	maxExecutions = SortExecutionsByOperation(maxExecutions);
	displayExecutions(maxExecutions);
#pragma endregion


#pragma region Funcionalidade 8: Determinação da quantidade média de unidades de tempo necessárias para completar uma operação, considerando todas as alternativas possíveis;
	printf("\nFUNCIONALIDADE 8: Determinação da quantidade média de unidades de tempo necessárias para completar uma operação, considerando todas as alternativas possíveis;\n\n");

	// obter o tempo médio para completar uma operação
	float average = getAverageTimeToCompleteOperation(executions, 1);

	// mostrar resultado
	printf("Média de tempo necessário para completar a operação(ID: %d) é %.2f!\n", 1, average);
#pragma endregion

}



