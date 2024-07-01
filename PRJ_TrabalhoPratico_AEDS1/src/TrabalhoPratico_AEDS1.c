/*
 ============================================================================
 Name        : TrabalhoPratico_AEDSI.c
 Author      : Kauã Agner e Sávio Rosynni
 Version     :
 Copyright   : Your copyright notice
 Description : É sistema de gestão hoteleira, projetado para facilitar o gerenciamento de reservas e registros de clientes e funcionários.
 	 	 		Utilizando estruturas de dados eficientes como Cliente, Funcionario, Quarto e Estadia, o programa permite cadastrar novos clientes com
 	 	 		detalhes completos, como nome, endereço e telefone, atribuindo automaticamente um código único para cada cliente registrado. Além disso,
 	 	 		oferece funcionalidades para registrar funcionários com informações como cargo e salário, e cadastrar quartos com especificações detalhadas,
 	 	 		como número, capacidade de hóspedes e valor da diária.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 80
#define MAX_ENDERECO 200
#define MAX_TELEFONE 20
#define MAX_CARGO 50
#define MAX_STATUS 20
#define MAX_DATA 20

typedef struct {
    int codigo;
    char nome[MAX_NOME];
    char endereco[MAX_ENDERECO];
    char telefone[MAX_TELEFONE];
} Cliente;

typedef struct {
    int codigo;
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
    char cargo[MAX_CARGO];
    float salario;
} Funcionario;

typedef struct {
    int numero;
    int quantidadeHospedes;
    float valorDiaria;
    char status[MAX_STATUS];
} Quarto;

typedef struct {
    int codigo;
    char dataEntrada[MAX_DATA];
    char dataSaida[MAX_DATA];
    int quantidadeDiarias;
    int quantidadeHospedes;
    int codigoCliente;
    int numeroQuarto;
} Estadia;

int verificarDuplicidadeCodigoCliente(FILE *file, int codigo) {
	Cliente cliente;
	rewind(file);

	// Verifica se o código já existe
	while (fread(&cliente, sizeof(Cliente), 1, file)) {
		if (cliente.codigo == codigo) {
			return 1; // Código duplicado encontrado
		}
	}
	return 0; // Código é único
}


void cadastrarCliente(FILE *file) {
	Cliente cliente;

	// Solicita dados do cliente e remove a nova linha (\n) que é adicionada pelo fgets
	printf("\nDigite o nome do cliente: ");
	fgets(cliente.nome, MAX_NOME, stdin);
	cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

	printf("Digite o endereco do cliente: ");
	fgets(cliente.endereco, MAX_ENDERECO, stdin);
	cliente.endereco[strcspn(cliente.endereco, "\n")] = '\0';

	printf("Digite o telefone do cliente: ");
	fgets(cliente.telefone, MAX_TELEFONE, stdin);
	cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';

	// Gera um código aleatório para o cliente, garantindo que ele seja único
	do {
		cliente.codigo = rand() % 1000;
	} while (verificarDuplicidadeCodigoCliente(file, cliente.codigo));

	// Move o ponteiro do arquivo para o final e escreve o novo cliente no arquivo
	fseek(file, 0, SEEK_END);
	if (fwrite(&cliente, sizeof(Cliente), 1, file) != 1) {
		printf("\nErro ao cadastrar cliente!\n");
		return;
	}
	printf("\nCliente cadastrado com sucesso!\n");
}

void listaClientes(FILE *file) {
	Cliente cliente;
	rewind(file);

	printf("--------------------------");
	printf("\nLista de Clientes:\n");

	// Lê o arquivo e imprime a lista
	while (fread(&cliente, sizeof(Cliente), 1, file)) {
		printf("\nCodigo: %d\n", cliente.codigo);
		printf("Nome: %s\n", cliente.nome);
		printf("Endereco: %s\n", cliente.endereco);
		printf("Telefone: %s\n", cliente.telefone);
	}
	printf("--------------------------\n");
}

int verificarDuplicidadeCodigoFuncionario(FILE *file, int codigo) {
    Funcionario funcionario;
    rewind(file);

    // Verifica se o código já existe
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.codigo == codigo) {
            return 1; // Código duplicado encontrado
        }
    }
    return 0; // Código é único
}

void cadastrarFuncionario(FILE *file) {
    Funcionario funcionario;

    // Solicita o dados do funcionário e remove a nova linha (\n) que é adicionada pelo fgets
    printf("Digite o nome do funcionario: ");
    fgets(funcionario.nome, MAX_NOME, stdin);
    funcionario.nome[strcspn(funcionario.nome, "\n")] = '\0';

    printf("Digite o telefone do funcionario: ");
    fgets(funcionario.telefone, MAX_TELEFONE, stdin);
    funcionario.telefone[strcspn(funcionario.telefone, "\n")] = '\0';

    printf("Digite o cargo do funcionario: ");
    fgets(funcionario.cargo, MAX_CARGO, stdin);
    funcionario.cargo[strcspn(funcionario.cargo, "\n")] = '\0';

    printf("Digite o salario do funcionario: ");
    scanf("%f", &funcionario.salario);
    getchar(); // Consumir newline deixado pelo scanf

    // Gera um código aleatório para o funcionário, garantindo que ele seja único
    do {
        funcionario.codigo = rand() % 1000;
    } while (verificarDuplicidadeCodigoFuncionario(file, funcionario.codigo));

    // Move o ponteiro do arquivo para o final e escreve o novo funcionário no arquivo
    fseek(file, 0, SEEK_END);
    if (fwrite(&funcionario, sizeof(Funcionario), 1, file) != 1) {
        printf("Erro ao cadastrar funcionario!\n");
        return;
    }
    printf("Funcionario cadastrado com sucesso!\n");
}


void listaFuncionarios(FILE *file) {
    Funcionario funcionario;
    rewind(file);

    printf("--------------------------");
    printf("\nLista de Funcionarios:\n");

    // Lê o arquivo e imprime a lista
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        printf("\nCodigo: %d\n", funcionario.codigo);
        printf("Nome: %s\n", funcionario.nome);
        printf("Telefone: %s\n", funcionario.telefone);
        printf("Cargo: %s\n", funcionario.cargo);
        printf("Salario: %.2f\n", funcionario.salario);
    }
    printf("--------------------------\n");
}

int verificarDuplicidadeQuarto(FILE *file, int numero) {
    Quarto quarto;
    rewind(file);

    // Verifica se o número já existe
    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        if (quarto.numero == numero) {
            return 1; // Numero duplicado encontrado
        }
    }
    return 0; // Número é único
}

void cadastrarQuarto(FILE *file) {
    Quarto quarto;

    // Solicita o dados do quarto
    printf("Digite o numero do quarto: ");
    scanf("%d", &quarto.numero);

    // Chama a função que verifica se o número já existe
    if (verificarDuplicidadeQuarto(file, quarto.numero)) {
            printf("Numero de quarto já existe!\n");
            return;
        }

    printf("Digite a quantidade de hospedes: ");
    scanf("%d", &quarto.quantidadeHospedes);
    printf("Digite o valor da diaria: ");
    scanf("%f", &quarto.valorDiaria);
    getchar();

    // Define o status como "desocupado"
    strcpy(quarto.status, "desocupado");

    // Move o ponteiro do arquivo para o final e escreve o novo quarto no arquivo
    fseek(file, 0, SEEK_END);
    if (fwrite(&quarto, sizeof(Quarto), 1, file) != 1) {
        printf("Erro ao cadastrar quarto!\n");
        return;
    }
    printf("Quarto cadastrado com sucesso!\n");
}

void listaQuartos(FILE *file) {
    Quarto quarto;
    rewind(file);

    printf("--------------------------");
    printf("\nLista de Quartos:\n");
    // Lê o arquivo e imprime a lista
    while (fread(&quarto, sizeof(Quarto), 1, file)) {
        printf("\nNumero: %d\n", quarto.numero);
        printf("Quantidade de Hospedes: %d\n", quarto.quantidadeHospedes);
        printf("Valor da Diaria: %.2f\n", quarto.valorDiaria);
        printf("Status: %s\n", quarto.status);
    }
    printf("--------------------------\n");
}

// Função para converter datas no formato DD/MM/YYYY para struct tm
struct tm stringToDate(const char *dateString, int hour, int minute) {
    struct tm date = {0};

    // Lê a string de data no formato "DD/MM/YYYY" e atribui aos campos correspondentes de struct tm
    sscanf(dateString, "%d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year);

    date.tm_mon -= 1; // Ajusta o mês de 1-12 para 0-11
    date.tm_year -= 1900; // Ajusta o ano para o formato esperado por struct tm
    date.tm_hour = hour;
    date.tm_min = minute;

    return date;
}

int calcularQuantidadeDiarias(const char *dataEntrada, const char *dataSaida) {
    struct tm entrada = stringToDate(dataEntrada, 14, 0); // 14:00 na data de entrada
    struct tm saida = stringToDate(dataSaida, 12, 0); // 12:00 na data de saída
    time_t tEntrada = mktime(&entrada);
    time_t tSaida = mktime(&saida);
    double diff = difftime(tSaida, tEntrada) / (60 * 60 * 22); // 22 horas, pois uma diária dura das 14:00 até as 12:00 do dia seguinte
    return (int) diff;
}

int verificarDisponibilidadeQuarto(FILE *fileEstadias, int numeroQuarto, const char *dataEntrada, const char *dataSaida) {
	Estadia estadia;
	int disponivel = 1; // Assume que o quarto está inicialmente disponível

	// Verifica se o quarto está realmente disponível na data da estadia
	while (fread(&estadia, sizeof(Estadia), 1, fileEstadias) == 1) {
		if (estadia.numeroQuarto == numeroQuarto) {
			struct tm novaEntrada = stringToDate(dataEntrada, 14, 0); // 14:00 na data de entrada
			struct tm novaSaida = stringToDate(dataSaida, 12, 0); // 12:00 na data de saída
			struct tm entradaExistente = stringToDate(estadia.dataEntrada, 14,
					0);
			struct tm saidaExistente = stringToDate(estadia.dataSaida, 12, 0);

			time_t tNovaEntrada = mktime(&novaEntrada);
			time_t tNovaSaida = mktime(&novaSaida);
			time_t tEntradaExistente = mktime(&entradaExistente);
			time_t tSaidaExistente = mktime(&saidaExistente);

			if ((tNovaEntrada >= tEntradaExistente
					&& tNovaEntrada < tSaidaExistente)
					|| (tNovaSaida > tEntradaExistente
							&& tNovaSaida <= tSaidaExistente)
					|| (tNovaEntrada <= tEntradaExistente
							&& tNovaSaida >= tSaidaExistente)) {
				disponivel = 0; // Quarto ocupado
				break;
			}
		}
	}

	return disponivel;
}

int verificarDuplicidadeCodigoEstadia(FILE *file, int codigo) {
	Estadia estadia;
	rewind(file);

	// Verifica se o código já existe
	while (fread(&estadia, sizeof(Estadia), 1, file)) {
		if (estadia.codigo == codigo) {
			return 1; // Código duplicado encontrado
		}
	}
	return 0; // Código é único
}

void reservarEstadia(FILE *fileEstadias, FILE *fileClientes, FILE *fileQuartos) {
    Estadia estadia;
    Cliente cliente;
    Quarto quarto;

    // Solicita o código do cliente
    printf("Digite o codigo do cliente: ");
	scanf("%d", &estadia.codigoCliente);
	getchar();

	int clienteEncontrado = 0;
	rewind(fileClientes);
	 // Procura pelo cliente com o código fornecido
	while (fread(&cliente, sizeof(Cliente), 1, fileClientes)) {
		if (cliente.codigo == estadia.codigoCliente) {
			clienteEncontrado = 1;
			break;
		}
	}

	if (!clienteEncontrado) {
		printf("Cliente nao encontrado!\n");
		return;
	}

	// Solicita a quantidade de hóspedes
	printf("Digite a quantidade de hospedes: ");
	scanf("%d", &estadia.quantidadeHospedes);
	getchar(); // Consumir newline deixado pelo scanf

	// Solicita a data de entrada e a de saida
	printf("Digite a data de entrada (DD/MM/YYYY): ");
	fgets(estadia.dataEntrada, MAX_DATA, stdin);
	estadia.dataEntrada[strcspn(estadia.dataEntrada, "\n")] = '\0'; // Remover newline

	printf("Digite a data de saida (DD/MM/YYYY): ");
	fgets(estadia.dataSaida, MAX_DATA, stdin);
	estadia.dataSaida[strcspn(estadia.dataSaida, "\n")] = '\0'; // Remover newline

	int quartoEncontrado = 0;
	rewind(fileQuartos);
	// Procura por um quarto disponível para as datas e quantidade de hóspedes especificadas
	while (fread(&quarto, sizeof(Quarto), 1, fileQuartos)) {
		if (quarto.quantidadeHospedes >= estadia.quantidadeHospedes
				&& verificarDisponibilidadeQuarto(fileEstadias, quarto.numero,
						estadia.dataEntrada, estadia.dataSaida)) {
			quartoEncontrado = 1; // Quarto encontrado
			estadia.numeroQuarto = quarto.numero;
			break;
		}
	}

	// Quarto não encontrado
	if (!quartoEncontrado) {
		printf("Nenhum quarto disponivel para as datas solicitadas ou para a quantidade de hóspedes!\n");
		return;
	}

	// Chama a função que calcula a quantidade de diárias
    estadia.quantidadeDiarias = calcularQuantidadeDiarias(estadia.dataEntrada, estadia.dataSaida);

    // Gera um código aleatório para a estadia, garantindo que ele seja único
    do {
        estadia.codigo = rand() % 1000;
    } while (verificarDuplicidadeCodigoEstadia(fileEstadias, estadia.codigo));

    // Move o ponteiro do arquivo para o final e escreve a estadia no arquivo
    fseek(fileEstadias, 0, SEEK_END);
    if (fwrite(&estadia, sizeof(Estadia), 1, fileEstadias) != 1) {
        printf("Erro ao reservar estadia!\n");
        return;
    }

    // Atualiza o status do quarto para "ocupado"
    rewind(fileQuartos);
    while (fread(&quarto, sizeof(Quarto), 1, fileQuartos)) {
        if (quarto.numero == estadia.numeroQuarto) {
            strcpy(quarto.status, "ocupado");
            fseek(fileQuartos, -sizeof(Quarto), SEEK_CUR);
            fwrite(&quarto, sizeof(Quarto), 1, fileQuartos);
            break;
        }
    }

    printf("Estadia reservada com sucesso!\n");
}


void listaEstadias(FILE *fileEstadias, FILE *fileClientes, FILE *fileQuartos) {
	Estadia estadia;
	Cliente cliente;
	Quarto quarto;

	rewind(fileEstadias);

	printf("--------------------------");
	printf("\nLista de Estadias:\n");
	while (fread(&estadia, sizeof(Estadia), 1, fileEstadias) == 1) {
		printf("\nCodigo da estadia: %d\n", estadia.codigo);

		// Busca informações do cliente e imprime
		rewind(fileClientes);
		while (fread(&cliente, sizeof(Cliente), 1, fileClientes) == 1) {
			if (cliente.codigo == estadia.codigoCliente) {
				printf("Nome do cliente: %s\n", cliente.nome);
				break;
			}
		}

		// Busca informações do quarto e imprime
		rewind(fileQuartos);
		while (fread(&quarto, sizeof(Quarto), 1, fileQuartos) == 1) {
			if (quarto.numero == estadia.numeroQuarto) {
				printf("Numero do quarto: %d\n", quarto.numero);
				printf("Quantidade de hospedes: %d\n",
						estadia.quantidadeHospedes);
				printf("Valor da diaria: %.2f\n", quarto.valorDiaria);
				break;
			}
		}

		printf("Data de entrada: %s\n", estadia.dataEntrada);
		printf("Data de saida: %s\n", estadia.dataSaida);
		printf("Quantidade de diarias: %d\n", estadia.quantidadeDiarias);
	}
	printf("--------------------------\n");
}

void darBaixaEstadia(FILE *fileEstadias, FILE *fileQuartos) {
    int codigoEstadia;
    Estadia estadia;
    Quarto quarto;

    // Solicita o código da estadia
    printf("Digite o codigo da estadia para dar baixa: ");
    if (scanf("%d", &codigoEstadia) != 1) {
        printf("Codigo de estadia invalido.\n");
        getchar();
        return;
    }
    getchar();

    int encontrouEstadia = 0;

    rewind(fileEstadias);
    while (fread(&estadia, sizeof(Estadia), 1, fileEstadias)) {
        if (estadia.codigo == codigoEstadia) {
            // Calcula valor total a ser pago
            rewind(fileQuartos);
            while (fread(&quarto, sizeof(Quarto), 1, fileQuartos)) {
                if (quarto.numero == estadia.numeroQuarto) {
                    float valorTotal = estadia.quantidadeDiarias * quarto.valorDiaria;

                    // Alterar o status do quarto para "desocupado"
                    strcpy(quarto.status, "desocupado");
                    fseek(fileQuartos, -sizeof(Quarto), SEEK_CUR);
                    fwrite(&quarto, sizeof(Quarto), 1, fileQuartos);

                    printf("Estadia com codigo %d foi encerrada.\n", codigoEstadia);
                    printf("Valor total a ser pago pelo cliente: %.2f\n", valorTotal);

                    encontrouEstadia = 1; // Encontrou estadia
                    break;
                }
            }
        }
    }

    // Estadia não foi encontrada
    if (!encontrouEstadia) {
        printf("Estadia com codigo %d nao encontrada.\n", codigoEstadia);
    }
}

void pesquisarCliente(FILE *fileClientes) {
    int opcao;

    // Solicita opção de pesquisa
    printf("\nEscolha uma opção de pesquisa:\n");
    printf("1. Pesquisar por código\n");
    printf("2. Pesquisar por nome\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1: {
            int codigo;

            // Solicita código do cliente
            printf("Digite o código do cliente: ");
            scanf("%d", &codigo);
            getchar();

            Cliente cliente;
            rewind(fileClientes);
            // Busca pelo cliente com o código especificado
            while (fread(&cliente, sizeof(Cliente), 1, fileClientes)) {
                if (cliente.codigo == codigo) {
                	// Cliente encontrado, imprime seus dados
                	printf("\n--------------------------\n");
                    printf("Cliente encontrado:\n");
                    printf("Codigo: %d\n", cliente.codigo);
                    printf("Nome: %s\n", cliente.nome);
                    printf("Endereco: %s\n", cliente.endereco);
                    printf("Telefone: %s\n", cliente.telefone);
                    printf("--------------------------\n");
                    return;
                }
            }
            // Cliente não foi encontrado
            printf("\nCliente com código %d não encontrado.\n", codigo);
            break;
        }
        case 2: {
            char nome[MAX_NOME];
            // Solicita nome do cliente
            printf("Digite o nome do cliente: ");
            fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0'; // Remover newline

            Cliente cliente;
            int encontrou = 0;
            rewind(fileClientes);
            // Busca pelo cliente com o nome especificado
            while (fread(&cliente, sizeof(Cliente), 1, fileClientes)) {
                if (strcmp(cliente.nome, nome) == 0) {
                	// Cliente encontrado, imprime seus dados
                	printf("\n--------------------------\n");
                    printf("Cliente encontrado:\n");
                    printf("Codigo: %d\n", cliente.codigo);
                    printf("Nome: %s\n", cliente.nome);
                    printf("Endereco: %s\n", cliente.endereco);
                    printf("Telefone: %s\n", cliente.telefone);
                    printf("--------------------------\n");
                    encontrou = 1;
                }
            }
            // Cliente não foi encontrado
            if (!encontrou) {
                printf("\nCliente com nome '%s' não encontrado.\n", nome);
            }
            break;
        }
        default:
            printf("Opção inválida.\n");
            break;
    }
}

void pesquisarFuncionario(FILE *fileFuncionarios) {
    int opcao;

    // Solicita opção de pesquisa
    printf("\nEscolha uma opção de pesquisa:\n");
    printf("1. Pesquisar por código\n");
    printf("2. Pesquisar por nome\n");
    printf("Opção: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1: {
            int codigo;
            // Solicita código do funcionário
            printf("Digite o código do funcionário: ");
            scanf("%d", &codigo);
            getchar();

            Funcionario funcionario;
            rewind(fileFuncionarios);
            // Busca pelo funcionário com o código especificado
            while (fread(&funcionario, sizeof(Funcionario), 1, fileFuncionarios)) {
                if (funcionario.codigo == codigo) {
                	// Funcionário encontrado, imprime seus dados
                	printf("\n--------------------------\n");
                    printf("Funcionário encontrado:\n");
                    printf("Codigo: %d\n", funcionario.codigo);
                    printf("Nome: %s\n", funcionario.nome);
                    printf("Telefone: %s\n", funcionario.telefone);
                    printf("Cargo: %s\n", funcionario.cargo);
                    printf("Salario: %.2f\n", funcionario.salario);
                    printf("--------------------------\n");
                    return;
                }
            }
            // Funcionário não foi encontrado
            printf("\nFuncionário com código %d não encontrado.\n", codigo);
            break;
        }
        case 2: {
            char nome[MAX_NOME];
            // Solicitação do nome do funcionário
            printf("Digite o nome do funcionário: ");
            fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0'; // Remover newline

            Funcionario funcionario;
            int encontrou = 0;
            rewind(fileFuncionarios);
            // Busca pelo funcionário com o nome especificado
            while (fread(&funcionario, sizeof(Funcionario), 1, fileFuncionarios)) {
                if (strcmp(funcionario.nome, nome) == 0) {
                	// Funcionário encontrado, imprime seus dados
                	printf("\n--------------------------\n");
                    printf("Funcionário encontrado:\n");
                    printf("Codigo: %d\n", funcionario.codigo);
                    printf("Nome: %s\n", funcionario.nome);
                    printf("Telefone: %s\n", funcionario.telefone);
                    printf("Cargo: %s\n", funcionario.cargo);
                    printf("Salario: %.2f\n", funcionario.salario);
                    printf("--------------------------\n");
                    encontrou = 1;
                }
            }
            // Funcionário não foi encontrado
            if (!encontrou) {
                printf("\nFuncionário com nome '%s' não encontrado.\n", nome);
            }
            break;
        }
        default:
            printf("Opção inválida.\n");
            break;
    }
}

void mostrarEstadiasCliente(FILE *fileEstadias, FILE *fileClientes, FILE *fileQuartos) {
    Cliente cliente;
    Estadia estadia;
    Quarto quarto;
    int codigoCliente;

    // Solicitação do código do cliente
    printf("Digite o código do cliente: ");
    scanf("%d", &codigoCliente);
    getchar();

    int encontrouCliente = 0;
    // Busca informações do cliente
    rewind(fileClientes);
    while (fread(&cliente, sizeof(Cliente), 1, fileClientes) == 1) {
        if (cliente.codigo == codigoCliente) {
            encontrouCliente = 1;
            // Cliente encontrado, imprime seus dados
            printf("\n--------------------------\n");
            printf("Nome do cliente: %s\n", cliente.nome);
            printf("Código do cliente: %d\n", cliente.codigo);
            printf("Estadias do Cliente:\n");
            printf("--------------------------\n");
            break;
        }
    }
    // Cliente não foi encontrado
    if (!encontrouCliente) {
        printf("Cliente com código %d não encontrado.\n", codigoCliente);
        return;
    }

    // Imprime estadias do cliente
    rewind(fileEstadias);
    int encontrouEstadia = 0;
    while (fread(&estadia, sizeof(Estadia), 1, fileEstadias) == 1) {
        if (estadia.codigoCliente == codigoCliente) {
            encontrouEstadia = 1;
            // Buscar informações do quarto
            rewind(fileQuartos);
            while (fread(&quarto, sizeof(Quarto), 1, fileQuartos) == 1) {
                if (quarto.numero == estadia.numeroQuarto) {
                    printf("Código da estadia: %d\n", estadia.codigo);
                    printf("Número do quarto: %d\n", quarto.numero);
                    printf("Data de entrada: %s\n", estadia.dataEntrada);
                    printf("Data de saída: %s\n", estadia.dataSaida);
                    printf("Quantidade de diárias: %d\n", estadia.quantidadeDiarias);
                    printf("--------------------------\n");
                    break;
                }
            }
        }
    }
    // Estadias não encontradas
    if (!encontrouEstadia) {
        printf("\nEste cliente não possui estadias registradas.\n");
    }
}


int main() {
	setbuf(stdout, NULL); // Desabilita o buffering do stdout
    srand(time(NULL)); // Inicializa o srand

    // Abertura dos arquivos de dados principais (clientes, funcionários, quartos, estadias)
    FILE *fileClientes = fopen("clientes.dat", "rb+");
    if (fileClientes == NULL) {
        fileClientes = fopen("clientes.dat", "wb+");
        if (fileClientes == NULL) {
            printf("Erro ao abrir o arquivo de clientes!\n");
            return 1;
        }
    }

    FILE *fileFuncionarios = fopen("funcionarios.dat", "rb+");
    if (fileFuncionarios == NULL) {
        fileFuncionarios = fopen("funcionarios.dat", "wb+");
        if (fileFuncionarios == NULL) {
            printf("Erro ao abrir o arquivo de funcionarios!\n");
            fclose(fileClientes);
            return 1;
        }
    }

    FILE *fileQuartos = fopen("quartos.dat", "rb+");
    if (fileQuartos == NULL) {
        fileQuartos = fopen("quartos.dat", "wb+");
        if (fileQuartos == NULL) {
            printf("Erro ao abrir o arquivo de quartos!\n");
            fclose(fileClientes);
            fclose(fileFuncionarios);
            return 1;
        }
    }

    FILE *fileEstadias = fopen("estadias.dat", "rb+");
    if (fileEstadias == NULL) {
        fileEstadias = fopen("estadias.dat", "wb+");
        if (fileEstadias == NULL) {
            printf("Erro ao abrir o arquivo de estadias!\n");
            fclose(fileClientes);
            fclose(fileFuncionarios);
            fclose(fileQuartos);
            return 1;
        }
    }

    int opcao;

    do {
    	// Menu principal
    	printf("\n--------------------------\n");
    	printf("Hotel Descanso Garantido\n");
    	printf("--------------------------\n");
        printf("Menu:\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Listar Clientes\n");
        printf("3. Cadastrar Funcionario\n");
        printf("4. Listar Funcionarios\n");
        printf("5. Cadastrar Quarto\n");
        printf("6. Listar Quartos\n");
        printf("7. Reservar Estadia\n");
        printf("8. Listar Historico de Estadias\n");
        printf("9. Dar Baixa em Estadia\n");
        printf("10. Pesquisar Cliente\n");
        printf("11. Pesquisar Funcionario\n");
        printf("12. Pesquisar Estadias de um Cliente\n");
        printf("0. Sair\n");
        printf("Escolha uma opçao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrarCliente(fileClientes);
                break;
            case 2:
                listaClientes(fileClientes);
                break;
            case 3:
                cadastrarFuncionario(fileFuncionarios);
                break;
            case 4:
                listaFuncionarios(fileFuncionarios);
                break;
            case 5:
                cadastrarQuarto(fileQuartos);
                break;
            case 6:
                listaQuartos(fileQuartos);
                break;
            case 7:
            	reservarEstadia(fileEstadias, fileClientes, fileQuartos);
                break;
            case 8:
                listaEstadias(fileEstadias, fileClientes, fileQuartos);
                break;
            case 9:
                darBaixaEstadia(fileEstadias, fileQuartos);
                break;
            case 10:
            	pesquisarCliente(fileClientes);
            	break;
            case 11:
                pesquisarFuncionario(fileFuncionarios);
                break;
            case 12:
            	mostrarEstadiasCliente(fileEstadias, fileClientes, fileQuartos);
            	break;
            case 0:
                printf("Saindo do Menu...\n");
                break;
            default:
                printf("Opçao invalida!\n");
        }
    } while (opcao != 0);

    // Fechamento de todos os arquivos abertos
    fclose(fileClientes);
    fclose(fileFuncionarios);
    fclose(fileQuartos);
    fclose(fileEstadias);

    return 0;
}
