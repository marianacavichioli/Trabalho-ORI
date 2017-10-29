#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_NOME 48
#define TAMANHO_CPF 11
#define TAMANHO_IDADE 3
#define TAMANHO_ENDERECO 38

FILE *arquivo;
int quantidade_registros;

//Registro de 100 bytes
struct Registro {
	char cpf[TAMANHO_CPF];
	char nome[TAMANHO_NOME];
	char endereco[TAMANHO_ENDERECO];
	char idade[TAMANHO_IDADE];
};

struct Registro registro[5];


//Função para criar arquivo.
void CriarArquivo() {

	arquivo = fopen("teste.txt", "wt");
	printf("Arquivo criado\n");
	if (arquivo == NULL) {
		printf("Não foi possível abrir o arquivo");
	}

	fclose(arquivo);
	arquivo = NULL;
}

int QuantidadeRegistros() {
	int quantidade_registros = 0;
	int retorno;

	retorno = fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
	if (retorno == 1) {
		while (retorno == 1) {
			quantidade_registros++;
			retorno = fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
		}
	}

	printf("%d registros\n", quantidade_registros);
	
	return quantidade_registros;
}

//Função para inserir um registro no arquivo.
void InserirRegistro() {

	quantidade_registros = 0;
	int final;

	arquivo = fopen("teste.txt", "r+");

	quantidade_registros = QuantidadeRegistros();

	if (quantidade_registros < 5) {
		do {
			printf("Digite 1 para inserir os registros ou 0 para sair: \n");
			scanf("%d", &final);

			if (final == 1) {
				printf("CPF: \n");
				scanf("%s", &registro[quantidade_registros].cpf);
				printf("Nome: \n");
				scanf("%s", &registro[quantidade_registros].nome);
				printf("Endereço: \n");
				scanf("%s", &registro[quantidade_registros].endereco);
				printf("Idade: \n");
				scanf("%s", &registro[quantidade_registros].idade);
				printf("\n");
				
				fwrite(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
				quantidade_registros++;
			}
		} while ((final != 0) && (quantidade_registros < 5));
	}

	fclose(arquivo);
}

//Função para listar registros do arquivo.
void ListarRegistro() {

	quantidade_registros = 0;
	int final, retorno;
	arquivo = fopen("teste.txt", "r");

	if (arquivo != NULL) {
		fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
		while (!feof(arquivo)) {
			printf("%s ", registro[quantidade_registros].cpf);
			printf("%s ", registro[quantidade_registros].nome);
			printf("%s ", registro[quantidade_registros].endereco);
			printf("%s ", registro[quantidade_registros].idade);
			printf("\n");
			fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
		}

		fclose(arquivo);
	}
	else
	{
		printf("Falha ao abrir arquivo");
	}
}

//Função para buscar registros no arquivo
void BuscarRegistro() {
	char chave[11];
	
	printf("Digite o CPF que deseja buscar\n");
	scanf("%s", &chave);
	if (strlen(chave) != 11)
	{
		while (strlen(chave) != 11)
		{
			printf("CPF invalido!\n");
			printf("Digite o CPF que deseja buscar\n");
			scanf("%s", &chave);
		}
	}
	
	if (arquivo != NULL)
	{
		arquivo = fopen("teste.txt", "r");
		bool encontrou = false;
		int quantidade_registros = QuantidadeRegistros();
		fseek(arquivo, SEEK_SET, 0);
		while (!encontrou)
		{
			if (quantidade_registros == 0)
			{
				printf("Nenhum registro cadastrado\n");
				break;
			}
			else
			{
				fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
				while (!feof(arquivo)) {
					if(registro[quantidade_registros].cpf == chave)
					fread(&registro[quantidade_registros], sizeof(Registro), 1, arquivo);
				}
			}
		}
	}
	else
	{
		printf("Arquivo nao foi criado!\n");
	}

}

int main() {
	//struct Registro registro[512];
	int opcoes;

	printf("Escolha uma das opções: \n");
	printf("1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactação do arquivo\n");
	printf("\n");
	scanf("%d", &opcoes);

	switch (opcoes) {
	case 1:
		CriarArquivo();
		break;
	case 2:
		InserirRegistro();
		break;
	case 3:
		BuscarRegistro();
		break;
	//case 4:
	//	RemoverRegistro();
	//	break;
	case 5:
		ListarRegistro();
		break;
		/*case 6:
		CompactacaoArquivo();
		break;*/
	}


	return 0;
}