#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *arquivo;
int qntreg;

//Registro de 100 bytes
struct Registro {
	char nome[48];
	char endereco[50];
	char idade[2];
	
};

struct Registro reg[5];


//Função para criar arquivo.
void CriarArquivo(){
	
	arquivo = fopen("teste.txt", "wt");
	printf("Arquivo criado\n");	
	if(arquivo == NULL){
		printf("Não foi possível abrir o arquivo");
	}

	fclose(arquivo);
	arquivo = NULL;
}

//Função para inserir um registro no arquivo.
void InserirRegistro(){

	qntreg = 0;
	int final, retorno;

	arquivo = fopen("teste.txt", "r+");

	retorno = fread(&reg[qntreg], sizeof(Registro), 1, arquivo); 
	if(retorno==1){
		while(retorno==1){
			qntreg++;
			retorno = fread(&reg[qntreg], sizeof(Registro), 1, arquivo); 
		}
	}

	printf("%d registros\n", qntreg );

	if (qntreg < 5){
		do{
			printf("Digite 1 para inserir os registros ou 0 para sair: \n");
			scanf("%d", &final);

			if(final == 1){
				printf("Nome: \n");
				scanf("%s", &reg[qntreg].nome);
				printf("Endereço: \n");
				scanf("%s", &reg[qntreg].endereco);
				printf("Idade: \n");
				scanf("%s", &reg[qntreg].idade);
				printf("\n");

				fwrite(&reg[qntreg], sizeof(Registro), 1, arquivo);
				qntreg++;
			}
		}while((final != 0)&&(qntreg < 5));
	}

	fclose(arquivo);
}

//Função para listar registros do arquivo.
void ListarRegistro(){

	qntreg = 0;
	int final, retorno;
	arquivo = fopen("teste.txt", "r");

	fread(&reg[qntreg], sizeof(Registro), 1, arquivo); 
	while(!feof(arquivo)){
		printf("%s ", reg[qntreg].nome);
		printf("%s ", reg[qntreg].endereco);
		printf("%s ", reg[qntreg].idade);
		printf("\n");
		fread(&reg[qntreg], sizeof(Registro), 1, arquivo); 
	}

	fclose(arquivo);
}

int main(){
	//struct Registro registro[512];
	int opcoes;
	
	printf("Escolha uma das opções: \n");
	printf("1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactação do arquivo\n");
	printf("\n");
	scanf("%d", &opcoes);
	
	switch(opcoes){
		case 1:
			CriarArquivo();
		break;
		case 2:
			InserirRegistro();
		break;
		/*case 3:
			BuscarRegisto();
		break;
		case 4:
			RemoverRegistro();
		*/break;
		case 5:
			ListarRegistro();
		break;
		/*case 6:
			CompactacaoArquivo();
		break;*/
	}


	return 0;
}