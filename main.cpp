#include "funcoes_basicas.hpp"


int main() {
	int opcoes;
	FILE *arquivo;
	printf("Escolha uma das opcoes: \n");
	printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactar arquivo\n");
	printf("\n");
	scanf("%d", &opcoes);

	while(opcoes != 0){

		if (opcoes == 1) {
			criarArquivo(arquivo);
		}else if(opcoes == 2){
			inserirRegistro();
		}else if(opcoes == 3){
			buscarRegistro();
		}else if(opcoes == 4){
			removerRegistro();
		}else if(opcoes == 5){
			listarRegistro();
		}else if(opcoes == 6){
			compactarArquivo();
		}

		printf("Escolha uma das opcoes: \n");
		printf("0-Sair\n1-Criar arquivo\n2-Inserir novo registro\n3-Buscar por um registro\n4-Remover um registro\n5-Listar registros\n6-Compactar arquivo\n");
		printf("\n");
		scanf("%d", &opcoes);
	}
	return 0;
}