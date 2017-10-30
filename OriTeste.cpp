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

FILE *arquivo;

//Registro de 100 bytes
struct Registro {
	char cpf[TAMANHO_CPF];
	char nome[TAMANHO_NOME];
	char endereco[TAMANHO_ENDERECO];
	char idade[TAMANHO_IDADE];
};

struct Bloco{
	struct Registro registro[QNT_REGISTROS];
	char complemento[12];
};

//struct Bloco bloco;

struct Bloco* criarBloco(void){
	Bloco* bloco = (Bloco*) malloc(sizeof(Bloco));
	memset(bloco,0,TAM_BLOCOS);
	return bloco;
}

int quantidadeBytes(){
	int tamanho = 0;
	arquivo = fopen("teste.txt", "rt");
	if(arquivo != NULL){
		fseek(arquivo, 0, SEEK_END);
		tamanho = ftell(arquivo);
	}else{
		printf("Arquivo inexistente\n");
	}

	fclose(arquivo);
	return tamanho;
}

//Fun��o para criar arquivo.
void CriarArquivo() {

	arquivo = fopen("teste.txt", "wt");
	printf("Arquivo criado\n");
	if (arquivo == NULL) {
		printf("N�o foi poss�vel abrir o arquivo");
	}

	fclose(arquivo);
	arquivo = NULL;
}

void escreverRegistro(Bloco* bloco){

	int quantidade_registros = 0;

	quantidade_registros = quantidadeBytes()/100;
	
	printf("CPF: \n");
	scanf("%s", &bloco->registro[quantidade_registros].cpf);
	printf("Nome: \n");
	scanf("%s", &bloco->registro[quantidade_registros].nome);
	printf("Endereco: \n");
	scanf("%s", &bloco->registro[quantidade_registros].endereco);
	printf("Idade: \n");
	scanf("%s", &bloco->registro[quantidade_registros].idade);
	printf("\n");
	
	fwrite(&bloco->registro[quantidade_registros], TAMANHO_REGISTRO, 1, arquivo);
	quantidade_registros++;		
}


//Fun��o para inserir um registro no arquivo.
void InserirRegistro() {
	int final, quantidade_registros = 0, quantidade_blocos = 0;
	Bloco* bloco = criarBloco();
	arquivo = fopen("teste.txt", "r+");
	int retorno;

	quantidade_registros = quantidadeBytes()/100;
	quantidade_blocos = quantidadeBytes()/512;

	while(!feof(arquivo)){
		fread(&bloco->registro[quantidade_registros], TAMANHO_REGISTRO, 1, arquivo);
	}

	do {
		printf("%d\n", quantidade_registros);
		printf("Digite 1 para inserir os registros ou 0 para sair: \n");
		scanf("%d", &final);

		if (final == 1) {
			if(!arquivo){
				printf("Não foi possivel abrir o arquivo\n");
			}else{			
				//quantidade_registros = QuantidadeRegistros(bloco);
				if(quantidade_registros == 0){
					printf("Nenhum Registro\n");
					fwrite(bloco, TAM_BLOCOS, 1, arquivo);
					fseek(arquivo, 0, SEEK_SET);
					escreverRegistro(bloco);
					
				}else if(quantidade_registros == 5){
					printf("Mais de 5\n");
					fseek(arquivo, 0, SEEK_END);
					fwrite(bloco, TAM_BLOCOS, 1, arquivo);
					fseek(arquivo, quantidade_blocos*TAM_BLOCOS, SEEK_CUR);
					escreverRegistro(bloco);
					
				}else if(quantidade_registros < 5){
					fseek(arquivo, 0, SEEK_SET);
					fseek(arquivo, quantidade_registros*TAMANHO_REGISTRO, SEEK_CUR);
					escreverRegistro(bloco);
				}
				
			}
		}		
	} while (final != 0);

	fclose(arquivo);
}

//Fun��o para listar registros do arquivo.
void ListarRegistro() {

	Bloco* bloco = criarBloco();

	int final, retorno, quantidade_registros = 0;
	arquivo = fopen("teste.txt", "r");

	quantidade_registros = quantidadeBytes()/100;

	if (arquivo != NULL) {
		fread(bloco, TAM_BLOCOS, 1, arquivo);
		while (!feof(arquivo)) {
			printf("%s ", bloco->registro[quantidade_registros].cpf);
			fseek(arquivo, TAMANHO_CPF, SEEK_CUR);
			printf("%s ", bloco->registro[quantidade_registros].nome);
			fseek(arquivo, TAMANHO_NOME, SEEK_CUR);
			printf("%s ", bloco->registro[quantidade_registros].endereco);
			fseek(arquivo, TAMANHO_ENDERECO, SEEK_CUR);
			printf("%s ", bloco->registro[quantidade_registros].idade);
			printf("\n");
			fseek(arquivo, TAMANHO_IDADE, SEEK_CUR);
			fread(bloco, TAM_BLOCOS, 1, arquivo);
		}

		fclose(arquivo);
	}
	else
	{
		printf("Falha ao abrir arquivo");
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