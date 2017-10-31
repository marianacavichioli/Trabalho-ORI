#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_NOME 48
#define TAMANHO_CPF 11
#define TAMANHO_IDADE 3
#define TAMANHO_ENDERECO 38
#define QNT_REGISTROS 5
#define TAM_BLOCOS 512
#define TAMANHO_REGISTRO 100
#define TAMANHO_CABECALHO 3

//Registro de 100 bytes
struct Registro {
	char cpf[TAMANHO_CPF];
	char nome[TAMANHO_NOME];
	char endereco[TAMANHO_ENDERECO];
	char idade[TAMANHO_IDADE];
};

struct Bloco{
	int cabecalho[TAMANHO_CABECALHO];
	struct Registro registro[QNT_REGISTROS];
};

//struct Bloco bloco;

struct Bloco* criarBloco(){
	Bloco* bloco = (Bloco*) malloc(sizeof(Bloco));
	memset(bloco,0,TAM_BLOCOS);
	bloco->cabecalho[0] = 8;
	bloco->cabecalho[1] = 8;
	bloco->cabecalho[2] = 8;
	return bloco;
}

//Fun��o para criar arquivo.
void CriarArquivo() {
	FILE *arquivo;

	arquivo = fopen("teste.txt", "wb");
	printf("Arquivo criado\n");
	if (arquivo == NULL) {
		printf("N�o foi poss�vel abrir o arquivo");
	}else{
		Bloco* bloco = criarBloco();
		//fwrite(bloco, TAM_BLOCOS, 1, arquivo);
		free(bloco);
		fclose(arquivo);
	}
}

void escreverRegistro(Bloco* bloco, FILE* arquivo, int quantidade_registros, int quantidade_blocos){

	printf("CPF: \n");
	scanf("%s", &bloco->registro[quantidade_registros].cpf);
	printf("Nome: \n");
	scanf("%s", &bloco->registro[quantidade_registros].nome);
	printf("Endereco: \n");
	scanf("%s", &bloco->registro[quantidade_registros].endereco);
	printf("Idade: \n");
	scanf("%s", &bloco->registro[quantidade_registros].idade);
	printf("\n");

	fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
	fwrite(bloco, TAM_BLOCOS, 1, arquivo);
}


//Fun��o para inserir um registro no arquivo.
int InserirRegistro() {

	FILE *arquivo;
	int quantidade_registros = 0, quantidade_blocos = 0;
	int final;
	Bloco* bloco_registro = criarBloco();
	arquivo = fopen("teste.txt", "rb+");
	

	do {
		printf("Digite 1 para inserir os registros ou 0 para sair: \n");
		scanf("%d", &final);

		if (final == 1) {
			if(!arquivo){
				printf("Não foi possivel abrir o arquivo\n");
				return 0;
			}else{			
				while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
					if((bloco_registro->cabecalho[0] == 8) && (bloco_registro->cabecalho[1] == 8) && (bloco_registro->cabecalho[2] == 8)){
						while(quantidade_registros <= 4){
							if(strcmp(bloco_registro->registro[quantidade_registros].cpf, "00000000000") == 0 || strcmp(bloco_registro->registro[quantidade_registros].cpf, "-1-1-1-1-1-1-1-1-1-1-1") == 0){
								escreverRegistro(bloco_registro, arquivo, quantidade_registros, quantidade_blocos);
								free(bloco_registro);
								fclose(arquivo);
								return 1;	
							}else{
								quantidade_registros++;
							}
						}
						quantidade_blocos++;
						quantidade_registros = 0;
					}else{
						return 0;
					}
				}
				bloco_registro = criarBloco();
				printf("CPF: \n");
				scanf("%s", &bloco_registro->registro[0].cpf);
				printf("Nome: \n");
				scanf("%s", &bloco_registro->registro[0].nome);
				printf("Endereco: \n");
				scanf("%s", &bloco_registro->registro[0].endereco);
				printf("Idade: \n");
				scanf("%s", &bloco_registro->registro[0].idade);
				printf("\n");
				fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
				fwrite(bloco_registro, TAM_BLOCOS, 1, arquivo);
				free(bloco_registro);
				fclose(arquivo);
				return 1;
			}
		}		
	} while (final != 0);
}

//Funcao para listar registros do arquivo.
int ListarRegistro() {
	
	FILE *arquivo;
	arquivo = fopen("teste.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;
	

	if (!arquivo) {
		printf("Falha ao abrir arquivo\n");
		return 0;
	}else{
		while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
			if((bloco_registro->cabecalho[0] == 8) && (bloco_registro->cabecalho[1] == 8) && (bloco_registro->cabecalho[2] == 8)){
				while(quantidade_registros <= 4){
					if(strcmp(bloco_registro->registro[quantidade_registros].cpf, "00000000000") == 0 || strcmp(bloco_registro->registro[quantidade_registros].cpf, "-1-1-1-1-1-1-1-1-1-1-1") == 0){
						quantidade_registros++;
					}else{
						printf("CPF: %s\nNome: %s\nEndereco: %s\nIdade: %s\n\n", bloco_registro->registro[quantidade_registros].cpf, bloco_registro->registro[quantidade_registros].nome, bloco_registro->registro[quantidade_registros].endereco, bloco_registro->registro[quantidade_registros].idade);
						quantidade_registros++;
					}
				}

				quantidade_blocos++;
				quantidade_registros = 0;
			}else{
				return 0;
			}
		}

		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

//Fun��o para buscar registros no arquivo
/*void BuscarRegistro() {
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

}*/

int main() {
	int opcoes;

	printf("Escolha uma das op��es: \n");
	printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compacta��o do arquivo\n");
	printf("\n");
	scanf("%d", &opcoes);

	while(opcoes != 0){

		if (opcoes == 1) {
			CriarArquivo();
		}else if(opcoes == 2){
			InserirRegistro();
		}else if(opcoes == 3){
			printf("teste" );
			//BuscarRegistro();
		}else if(opcoes == 4){
			printf("teste" );
			//	RemoverRegistro();
		}else if(opcoes == 5){
			ListarRegistro();
		}else if(opcoes == 6){
			printf("teste" );
			//CompactacaoArquivo();
		}

		printf("Escolha uma das op��es: \n");
		printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compacta��o do arquivo\n");
		printf("\n");
		scanf("%d", &opcoes);
	}
		
	

	return 0;
}