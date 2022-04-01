/**
* @author	José Pedro Lourenço (23496)
* @email	a23496@alunos.ipca.pt
* @data     01/04/2022
* @file		StandardData.c
*
* @brief	Ficheiro com função para carregar dados iniciais em listas
*/

#include "DataManagement.h"

/**
* @brief	Carregar dados para as listas
* @param	jobs		Apontador para a lista de jobs
* @param	machines	Apontador para a lista de máquinas
* @param	operations	Apontador para a lista de operações
* @param	executions	Apontador para a lista de execuções de operações
*/

void loadData(Job** jobs, Machine** machines, Operation** operations, Execution** executions)
{
	Job* job = NULL;
	Machine* machine = NULL;
	Operation* operation = NULL;
	Execution* execution = NULL;

	// jobs (id)
	job = newJob(1);
	*jobs = InsertJobInList(*jobs, job);

	// machines (id)
	machine = newMachine(1);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(2);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(3);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(4);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(5);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(6);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(7);
	*machines = InsertMachineInList(*machines, machine);
	machine = newMachine(8);
	*machines = InsertMachineInList(*machines, machine);
	
	// operations (id/order)
	operation = newOperation(1);
	*operations = InsertOperationInList(*operations, operation);
	operation = newOperation(2);
	*operations = InsertOperationInList(*operations, operation);
	operation = newOperation(3);
	*operations = InsertOperationInList(*operations, operation);
	operation = newOperation(4);
	*operations = InsertOperationInList(*operations, operation);

	// execution (operation_id, machine_id, time)
	// operation 1
	execution = newExecution(1, 1, 4);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(1, 3, 5);
	*executions = InsertExecutionInList(*executions, execution);

	// operation 2
	execution = newExecution(2, 2, 4);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(2, 4, 5);
	*executions = InsertExecutionInList(*executions, execution);

	// operation 3
	execution = newExecution(3, 3, 5);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(3, 5, 6);
	*executions = InsertExecutionInList(*executions, execution);

	// operation 4
	execution = newExecution(4, 4, 5);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(4, 5, 5);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(4, 6, 4);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(4, 7, 5);
	*executions = InsertExecutionInList(*executions, execution);
	execution = newExecution(4, 8, 9);
	*executions = InsertExecutionInList(*executions, execution);
}

/**
* @brief	Extra para mostrar dados na consola
*/
void ShowData(Execution* executions) {
	printf("Executions:\n");
	displayExecutions(executions);
}
