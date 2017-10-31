#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_NOME 46
#define TAMANHO_CPF 12
#define TAMANHO_IDADE 4
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
	memset(bloco, 0, TAM_BLOCOS);
	bloco->cabecalho[0] = 7;
	bloco->cabecalho[1] = 7;
	bloco->cabecalho[2] = 7;
	return bloco;
}


void registroInvalido(Bloco* bloco_registro, int quantidade_registros){
	int i;
	
	for(i = 0; i < TAMANHO_CPF - 1; i++){
		bloco_registro->registro[quantidade_registros].cpf[i] = '#';
	}for(i = 0; i < TAMANHO_NOME - 1; i++){
		bloco_registro->registro[quantidade_registros].nome[i] = '#';
	}for(i = 0; i < TAMANHO_ENDERECO - 1; i++){
		bloco_registro->registro[quantidade_registros].endereco[i] = '#';
	}for(i = 0; i < TAMANHO_IDADE - 1; i++){
		bloco_registro->registro[quantidade_registros].idade[i] = '#';
	}

}
//Funcao para criar arquivo.
void CriarArquivo() {
	FILE *arquivo;

	arquivo = fopen("teste.txt", "wb");
	printf("Arquivo criado\n\n");
	if (arquivo == NULL) {
		printf("Nao foi possivel abrir o arquivo");
	}
	fclose(arquivo);
}

void escreverRegistro(Bloco* bloco, FILE* arquivo, int quantidade_registros, int quantidade_blocos){

	printf("\nCPF: \n");
	scanf("%s", &bloco->registro[quantidade_registros].cpf);
	if (strlen(bloco->registro[quantidade_registros].cpf) != 11)
	{
		while (strlen(bloco->registro[quantidade_registros].cpf) != 11)
		{
			printf("\nCPF invalido!\n\n");
			printf("\nCPF:\n");
			scanf("%s", &bloco->registro[quantidade_registros].cpf);
		}
	}	
	printf("\nNome: \n");
	scanf("%s", &bloco->registro[quantidade_registros].nome);
	printf("\nEndereco: \n");
	scanf("%s", &bloco->registro[quantidade_registros].endereco);
	printf("\nIdade: \n");
	scanf("%s", &bloco->registro[quantidade_registros].idade);
	if (strlen(bloco->registro[quantidade_registros].idade) > 3)
	{
		while (strlen(bloco->registro[quantidade_registros].idade) > 3)
		{
			printf("\nIdade invalida!\n\n");
			printf("\nIdade:\n");
			scanf("%s", &bloco->registro[quantidade_registros].idade);
		}
	}	
	printf("\n");

	fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
	fwrite(bloco, TAM_BLOCOS, 1, arquivo);
}


//Funcao para inserir um registro no arquivo.
int InserirRegistro() {

	FILE *arquivo;
	int quantidade_registros = 0;
	int quantidade_blocos = 0;
	int soma =0;
	int i;
	int final;
	Bloco* bloco_registro = criarBloco();
	arquivo = fopen("teste.txt", "rb+");
	

	do {
		printf("\nDigite 1 para inserir os registros ou 0 para sair: \n");
		scanf("%d", &final);

		if (final == 1) {
			if(!arquivo){
				printf("\nNão foi possivel abrir o arquivo\n");
				return 0;
			}else{			
				while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
					if((bloco_registro->cabecalho[0] == 7) && (bloco_registro->cabecalho[1] == 7) && (bloco_registro->cabecalho[2] == 7)){
						while(quantidade_registros < 5){
							for(i = 0; i < TAMANHO_CPF-1; i++){
								soma = soma + bloco_registro->registro[quantidade_registros].cpf[i];
							}
							if(soma == 0 || strcmp("###########", bloco_registro->registro[quantidade_registros].cpf) == 0){
								escreverRegistro(bloco_registro, arquivo, quantidade_registros, quantidade_blocos);
								free(bloco_registro);
								fclose(arquivo);
								return 1;	
							}else{
								quantidade_registros = quantidade_registros + 1;
							}
							soma = 0;
						}
						quantidade_blocos = quantidade_blocos + 1;
						quantidade_registros = 0;
					}
					else{
						return 0;
					}
				}
				bloco_registro = criarBloco();
				printf("\nCPF: \n");
				scanf("%s", &bloco_registro->registro[0].cpf);
				if (strlen(bloco_registro->registro[0].cpf) != 11)
				{
					while (strlen(bloco_registro->registro[0].cpf) != 11)
					{
						printf("\nCPF invalido!\n\n");
						printf("\nCPF:\n");
						scanf("%s", &bloco_registro->registro[0].cpf);
					}
				}
				printf("\nNome: \n");
				scanf("%s", &bloco_registro->registro[0].nome);
				printf("\nEndereco: \n");
				scanf("%s", &bloco_registro->registro[0].endereco);
				printf("\nIdade: \n");
				scanf("%s", &bloco_registro->registro[0].idade);
				if (strlen(bloco_registro->registro[0].idade) > 3)
				{
					while (strlen(bloco_registro->registro[0].idade) > 3)
					{
						printf("\nIdade invalida!\n\n");
						printf("\nIdade:\n");
						scanf("%s", &bloco_registro->registro[0].idade);
					}
				}	
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
	int soma = 0;
	int i;
	arquivo = fopen("teste.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;
	

	if (!arquivo) {
		printf("Falha ao abrir arquivo\n");
		return 0;
	}else{
		while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
			if((bloco_registro->cabecalho[0] == 7) && (bloco_registro->cabecalho[1] == 7) && (bloco_registro->cabecalho[2] == 7)){
				while(quantidade_registros <= 4){
					for(i = 0; i < TAMANHO_CPF-1; i++){
						soma = soma + bloco_registro->registro[quantidade_registros].cpf[i];
					}
					if(soma == 0 || strcmp("###########", bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
						printf("Nenhum registro inserido\n\n");
						return 0;
					}else{
						printf("CPF: %s\nNome: %s\nEndereco: %s\nIdade: %s\n\n", bloco_registro->registro[quantidade_registros].cpf, bloco_registro->registro[quantidade_registros].nome, bloco_registro->registro[quantidade_registros].endereco, bloco_registro->registro[quantidade_registros].idade);
						quantidade_registros++;
					}
					soma = 0;
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

//Funcao para buscar registros no arquivo
int BuscarRegistro() {
	char chave[11];

	FILE *arquivo;
	int soma = 0;
	int i;
	arquivo = fopen("teste.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;
	
	printf("Digite o CPF que deseja buscar\n");
	scanf("%s", &chave);
	if (strlen(chave) != 11)
	{
		while (strlen(chave) != 11)
		{
			printf("CPF invalido!\n\n");
			printf("Digite o CPF que deseja buscar\n");
			scanf("%s", &chave);
		}
	}

	if (!arquivo) {
		printf("Falha ao abrir arquivo\n");
		return 0;
	}else{
		while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
			if((bloco_registro->cabecalho[0] == 7) && (bloco_registro->cabecalho[1] == 7) && (bloco_registro->cabecalho[2] == 7)){
				while(quantidade_registros <= 4){
					for(i = 0; i < TAMANHO_CPF-1; i++){
						soma = soma + bloco_registro->registro[quantidade_registros].cpf[i];
					}
					if(soma == 0 || strcmp("###########", bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
					}else{
						if(strcmp(chave, bloco_registro->registro[quantidade_registros].cpf) == 0){
							printf("CPF: %s\nNome: %s\nEndereco: %s\nIdade: %s\n\n", bloco_registro->registro[quantidade_registros].cpf, bloco_registro->registro[quantidade_registros].nome, bloco_registro->registro[quantidade_registros].endereco, bloco_registro->registro[quantidade_registros].idade);
							return 1;
						}else{
							printf("CPF nao encontrado\n\n");
							return 0;
						}
					}
					soma = 0;
				}
				quantidade_blocos++;
				quantidade_registros = 0;
			}else{
				return 0;
			}
		}
		printf("Nenhum registro inserido\n\n");
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

int removerRegistro() {
	char chave[11];

	FILE *arquivo;
	int soma = 0;
	int i, j;
	arquivo = fopen("teste.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;
	
	printf("Digite o CPF do registro que deseja remover\n");
	scanf("%s", &chave);
	if (strlen(chave) != 11)
	{
		while (strlen(chave) != 11)
		{
			printf("CPF invalido!\n\n");
			printf("Digite o CPF que deseja buscar\n");
			scanf("%s", &chave);
		}
	}

	if (!arquivo) {
		printf("Falha ao abrir arquivo\n");
		return 0;
	}else{
		while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
			if((bloco_registro->cabecalho[0] == 7) && (bloco_registro->cabecalho[1] == 7) && (bloco_registro->cabecalho[2] == 7)){
				while(quantidade_registros <= 4){
					for(i = 0; i < TAMANHO_CPF-1; i++){
						soma = soma + bloco_registro->registro[quantidade_registros].cpf[i];
					}
					if(soma == 0 || strcmp("###########", bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
					}else{
						if(strcmp(chave, bloco_registro->registro[quantidade_registros].cpf) == 0){
							printf("Registro removido\n\n");
							registroInvalido(bloco_registro, quantidade_registros);
							fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
							fwrite(bloco_registro, TAM_BLOCOS, 1, arquivo);
							free(bloco_registro);
							fclose(arquivo);
							return 1;
						}else{
							printf("CPF nao encontrado\n\n");
							return 0;
						}
					}
					soma = 0;
				}
				quantidade_blocos++;
				quantidade_registros = 0;
			}else{
				return 0;
			}
		}
		printf("Nenhum registro inserido\n\n");
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

int main() {
	int opcoes;

	printf("Escolha uma das opcoes: \n");
	printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactacao do arquivo\n");
	printf("\n");
	scanf("%d", &opcoes);

	while(opcoes != 0){

		if (opcoes == 1) {
			CriarArquivo();
		}else if(opcoes == 2){
			InserirRegistro();
		}else if(opcoes == 3){
			BuscarRegistro();
		}else if(opcoes == 4){
			removerRegistro();
		}else if(opcoes == 5){
			ListarRegistro();
		}else if(opcoes == 6){
			printf("teste" );
			//CompactacaoArquivo();
		}

		printf("Escolha uma das opcoes: \n");
		printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactacao do arquivo\n");
		printf("\n");
		scanf("%d", &opcoes);
	}
		
	

	return 0;
}