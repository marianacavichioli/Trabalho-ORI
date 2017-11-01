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
#define CPF_INVALIDO "###########"


/////////////ESTRUTURAS PARA ARMAZENAR DADOS//////////////

//Estrutura para o registro com tamanho de 100 bytes.
struct Registro {
	char cpf[TAMANHO_CPF];
	char nome[TAMANHO_NOME];
	char endereco[TAMANHO_ENDERECO];
	char idade[TAMANHO_IDADE];
};

//Estrutura para o bloco com tamanho de 512 bytes.
struct Bloco{
	int cabecalho[TAMANHO_CABECALHO];
	struct Registro registro[QNT_REGISTROS];
};


/////////////////FUNCOES AUXILIARES////////////////////

//Funcao para alocar um novo bloco na memoria.
struct Bloco* criarBloco(){
	Bloco* bloco = (Bloco*) malloc(sizeof(Bloco));
	memset(bloco, 0, TAM_BLOCOS);
	bloco->cabecalho[0] = 7;
	bloco->cabecalho[1] = 7;
	bloco->cabecalho[2] = 7;
	return bloco;
}

//Funcao para copiar o registro de um bloco a um bloco auxiliar.
void copiarRegistro(Bloco* bloco_registro, Bloco* bloco_auxiliar,int quantidade_registros, int qntRegistro_auxiliar){
	strcpy(bloco_auxiliar->registro[qntRegistro_auxiliar].cpf, bloco_registro->registro[quantidade_registros].cpf);
	strcpy(bloco_auxiliar->registro[qntRegistro_auxiliar].nome, bloco_registro->registro[quantidade_registros].nome);
	strcpy(bloco_auxiliar->registro[qntRegistro_auxiliar].endereco, bloco_registro->registro[quantidade_registros].endereco);
	strcpy(bloco_auxiliar->registro[qntRegistro_auxiliar].idade, bloco_registro->registro[quantidade_registros].idade);
}

//Funcao para validar a chave digitada.
void chaveValida(char* chave){

	printf("Digite o CPF do registro que deseja remover\n");
		scanf("%s", chave);
		printf("\n");
		while (strlen(chave) != 11)	{
			printf("CPF invalido!\n\n");
			printf("Digite o CPF que deseja buscar\n");
			scanf("%s", chave);
			printf("\n");
		}	
}

//Funcao para validar o CPF digitado.
void cpfValido(Bloco* bloco_registro, int quantidade_registros){
	while (strlen(bloco_registro->registro[quantidade_registros].cpf) != 11){
		printf("\nCPF invalido!\n\n");
		printf("\nCPF:\n");
		scanf("%s", &bloco_registro->registro[quantidade_registros].cpf);
	}
}

//Funcao para validar a idade digitada.
void idadeValida(Bloco* bloco_registro, int quantidade_registros){
	while (strlen(bloco_registro->registro[quantidade_registros].idade) > 3){
		printf("\nIdade invalida!\n\n");
		printf("\nIdade:\n");
		scanf("%s", &bloco_registro->registro[quantidade_registros].idade);
	}
}

//Funcao para marcar um registro como invalido.
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

//Funcao para escrever dados em um registro.
void escreverRegistro(Bloco* bloco, FILE* arquivo, int quantidade_registros, int quantidade_blocos){

	printf("\nCPF: \n");
	scanf("%s", &bloco->registro[quantidade_registros].cpf);
	
	//Verifica se a entrada de CPF eh uma entrada valida.
	cpfValido(bloco, quantidade_registros);
		
	printf("\nNome: \n");
	scanf("%s", &bloco->registro[quantidade_registros].nome);
	printf("\nEndereco: \n");
	scanf("%s", &bloco->registro[quantidade_registros].endereco);
	printf("\nIdade: \n");
	scanf("%s", &bloco->registro[quantidade_registros].idade);
	
	//Verifica se a entrada de idade eh uma entrada valida.
	idadeValida(bloco, quantidade_registros);

	printf("\n");

	fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
	fwrite(bloco, TAM_BLOCOS, 1, arquivo);
}


///////////////////FUNCOES BASICAS///////////////////////

//Funcao para criar um novo arquivo.
void criarArquivo(FILE* arquivo) {
	arquivo = fopen("arquivo.txt", "wb");
	printf("Arquivo criado\n\n");
	if (arquivo == NULL) {
		printf("Nao foi possivel abrir o arquivo");
	}
	fclose(arquivo);
}

//Funcao para inserir um novo registro no arquivo.
int inserirRegistro() {

	FILE *arquivo;									
	int quantidade_registros = 0;
	int quantidade_blocos = 0;
	int soma = 0;
	int i;
	Bloco* bloco_registro = criarBloco();
	arquivo = fopen("arquivo.txt", "rb+");

	if(!arquivo){
		printf("\nNão foi possivel abrir o arquivo\n");
		return 0;
	}else{

		//Le o arquivo enquanto existirem dados.		
		while(fread(bloco_registro, TAM_BLOCOS, 1, arquivo) != 0){
			//Verifica se o bloco eh valido atraves do cabecalho.
			if((bloco_registro->cabecalho[0] == 7) && (bloco_registro->cabecalho[1] == 7) && (bloco_registro->cabecalho[2] == 7)){
				//Enquanto existirem menos de 6 registros no bloco eh possivel realizar a insercao.
				while(quantidade_registros < 5){
					//Realiza a soma dos digitos do CPF encontrado para verificar se existe um registro ou nao.
					for(i = 0; i < TAMANHO_CPF-1; i++){
						soma = soma + bloco_registro->registro[quantidade_registros].cpf[i];
					}
					//Verifica se a soma eh igual a zero ou se encontrou um registro invalido(preenchido com #).
					if(soma == 0 || strcmp(CPF_INVALIDO, bloco_registro->registro[quantidade_registros].cpf) == 0){
						escreverRegistro(bloco_registro, arquivo, quantidade_registros, quantidade_blocos);
						free(bloco_registro);
						fclose(arquivo);
						return 1;	
					}
					//Foi encontrado um registro valido e, assim, incrementa a quantidade de registros para ler o proximo.
					else{
						quantidade_registros++;
					}
					soma = 0;
				}

				//Incrementa a quantidade de blocos para iniciar um novo bloco.
				quantidade_blocos++;
				quantidade_registros = 0;
			}
			else{
				return 0;
			}
		}

		//Se o arquivo esta vazio ou chegou ao fim do bloco cria um novo bloco para inserir o registro.
		bloco_registro = criarBloco();
		escreverRegistro(bloco_registro, arquivo, quantidade_registros, quantidade_blocos);
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

//Funcao para listar todos os registros do arquivo.
int listarRegistro() {
	
	FILE *arquivo;
	int soma = 0;
	int i;
	arquivo = fopen("arquivo.txt", "rb+");

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
					//Se encontrou um resgistro invalido ou um espaco vazio no arquivo incrementa a quantidade de registros para ir para o proximo.
					if(soma == 0 || strcmp(CPF_INVALIDO, bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
					}
					//Se encontrou um registro valido imprime seus dados na tela.
					else{
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
		printf("\n");
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

//Funcao para buscar registros no arquivo a partir de uma chave.
int buscarRegistro() {

	char chave[11]; //Responsavel por pegar a entrada do usuario para realizar a busca.

	FILE *arquivo;
	int soma = 0;
	int i;
	arquivo = fopen("arquivo.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;
	
	//Pede uma chave de entrada ao usuario e verifica se eh valida ou não.
	chaveValida(chave);

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
					if(soma == 0 || strcmp(CPF_INVALIDO, bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
					}
					//Se a chave for igual ao CPF de um registro no arquivo imprime seus dados na tela.
					else{
						if(strcmp(chave, bloco_registro->registro[quantidade_registros].cpf) == 0){
							printf("CPF: %s\nNome: %s\nEndereco: %s\nIdade: %s\n\n", bloco_registro->registro[quantidade_registros].cpf, bloco_registro->registro[quantidade_registros].nome, bloco_registro->registro[quantidade_registros].endereco, bloco_registro->registro[quantidade_registros].idade);
							return 1;
						}

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
		printf("Nenhum registro inserido\n\n");
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

//Funcao para remover um registro do arquivo a partir de uma chave.
int removerRegistro() {
	char chave[11];

	FILE *arquivo;
	int soma = 0;
	int i;
	arquivo = fopen("arquivo.txt", "rb+");

	Bloco* bloco_registro = criarBloco();

	int quantidade_blocos = 0, quantidade_registros = 0;

	chaveValida(chave);

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
					if(soma == 0 || strcmp(CPF_INVALIDO, bloco_registro->registro[quantidade_registros].cpf) == 0){
						quantidade_registros++;
					}else{
						//Se a chave for igual ao CPF de um registro no arquivo realiza a remocao logica de seus dados.
						if(strcmp(chave, bloco_registro->registro[quantidade_registros].cpf) == 0){
							//Completa todo o registro com #.
							registroInvalido(bloco_registro, quantidade_registros);
							fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_SET);
							fwrite(bloco_registro, TAM_BLOCOS, 1, arquivo);
							free(bloco_registro);
							fclose(arquivo);
							printf("Registro removido\n\n");

							return 1;
						}
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
		printf("Nenhum registro inserido\n\n");
		free(bloco_registro);
		fclose(arquivo);
		return 1;
	}
}

//Funcao para compactar um arquivo.
int compactarArquivo() {

	FILE *arquivo_auxiliar;

	FILE *arquivo;
	arquivo = fopen("arquivo.txt", "rb+");

	arquivo_auxiliar = fopen("arquivo_auxiliar.txt", "wb+");

	Bloco* bloco_registro = criarBloco();
	Bloco* bloco_auxiliar = criarBloco();


	int quantidade_blocos = 0, quantidade_registros = 0, qntRegistro_auxiliar = 0, qntBloco_auxiliar = 0;
	int soma = 0, i;
	
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
					//Se a soma for diferente de zero então encontrou um registro valido.
					if(soma != 0 && strcmp(CPF_INVALIDO, bloco_registro->registro[quantidade_registros].cpf) != 0){
						soma = 0;
						if((bloco_auxiliar->cabecalho[0] == 7) && (bloco_auxiliar->cabecalho[1] == 7) && (bloco_auxiliar->cabecalho[2] == 7)){
							//Copia o registro do bloco do arquivo para um bloco auxiliar.									
							copiarRegistro(bloco_registro, bloco_auxiliar, quantidade_registros, qntRegistro_auxiliar);
							qntRegistro_auxiliar++;
						}
					}
					quantidade_registros++;

					//Enquanto a quantidade de registros for menor ou igual a 5, grava o bloco auxilar em um arquivo auxiliar. Nao eh o ultimo bloco a ser lido.
					if(qntRegistro_auxiliar == 5){
						fwrite(bloco_auxiliar, TAM_BLOCOS, 1, arquivo_auxiliar);
						qntRegistro_auxiliar=0;
						free(bloco_auxiliar);
						bloco_auxiliar = criarBloco();
					}			
				}
				quantidade_blocos++;
				quantidade_registros = 0;				
			}

			//Quando 5 resgistros estão separados em mais de um bloco, grava todos em um bloco auxiliar no arquivo auxiliar.
			if(qntRegistro_auxiliar == 5){
				fwrite(bloco_auxiliar, TAM_BLOCOS, 1, arquivo_auxiliar);
				qntRegistro_auxiliar=0;
				free(bloco_auxiliar);
				bloco_auxiliar = criarBloco();
			}
		}

		//Terminou a leitura dos blocos e escreve o bloco auxiliar no arquivo auxiliar.
		if(qntRegistro_auxiliar > 0){
			fwrite(bloco_auxiliar, TAM_BLOCOS, 1, arquivo_auxiliar);
			free(bloco_auxiliar);
		}		
		free(bloco_registro);
		fclose(arquivo);
		fclose(arquivo_auxiliar);

		//Remove o arquivo inicial(com registros invalidos) e renomeia o arquivo auxiliar(sem registros invalidos.
		remove("arquivo.txt");
		rename("arquivo_auxiliar.txt", "arquivo.txt");

		printf("Arquivo compactado.\n\n");

		return 1;
	}	
}

