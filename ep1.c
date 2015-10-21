/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 01                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int mintermos[1024][11],
    maxtermos[1024][11],
    impressos[512][10],
    numMin = 0,
    numMax = 0,
    digitos = 0;
	
	
/***************************************************************/
/**                                                           **/ 
/**    Estrutura principal do programa, será uma arvore de    **/
/**  celulas, onde o pai de todos é o cubo que cobre todos    **/
/**  os outros abaixo dele.                                   **/
/**                                                           **/
/***************************************************************/

typedef struct ST_intervalos{
	int situacao;
	int inter[10];
	struct ST_intervalos *pai, *prox;
}*STinter;

STinter *nCubo, *fimCubo;

int verificaCobertura(int c[], int k){
	/***************************************************************/
	/**                                                           **/
	/**     Vefirica de o implicante primo ja foi impresso, ou    **/
	/**  algum outro que cobre ele.                               **/
	/**  char c[]: implicante que sera verificado                 **/
	/**  int k: numero de quantos implicantes ja foram impressos  **/
	/**                                                           **/
	/***************************************************************/
	int i, j, d; 
	if(k == 0) return 1;
	
	for(i = 0, d = 0; i < k; i++, d = 0){
		for(j = 0; j < digitos; j++)
			if((impressos[i][j] == c[j]) || (impressos[i][j] == -1)) d++;
		if(d == digitos) return 0;	
	}
	return 1;
}

int imprime(){
	/**************************************************/
	/**                                              **/             
	/**      Imprime todos os implicantes primos     **/
	/**                                              **/          
	/**************************************************/
	int n = digitos, i, k = 0;
	STinter x;

	for(; n >= 0; n--) for(x = nCubo[n]; x; x = x -> prox)
		if(x -> situacao == 0){
			x -> situacao = 2;
			if((!x->pai || x -> pai -> situacao == 1) && verificaCobertura(x -> inter, k)) {
				for(i = 0;  i < digitos; i++) {
					if(x -> inter[i] == -1) printf("X");
					else printf("%i", x -> inter[i]);
					impressos[k][i] = x -> inter[i];
				}
				printf(" "); k++;		
			}	
		}
	puts("");
	return 0;
}


int leString(){
	/**********************************************************/
	/**                                                      **/     
	/**      Le o comando de entrada do programa.            **/
	/**                                                      **/  
	/**********************************************************/
	int k = 0;
	char letra;
	
	/* Le os mintermos */
    	while (1) {
		letra = fgetc(stdin);
		if (letra == '1' || letra == '0')
			mintermos[numMin][k++] = letra - '0';
		else if(k > 0){
			numMin++; digitos = k; k = 0; 
		}
		if (letra == '\n') break;
    	} 
	return 0;
}

int achaComplementar(){
	/***************************************************************/
	/**                                                           **/
	/**      Pego os mintermos na entrada, calcula o maxtermos    **/
	/**                                                           **/
	/***************************************************************/
	int totalTermos = 1, i, j, k, m, d, n;
	
	for(i = 0; i < digitos; i++) totalTermos *= 2; /* Calcula o numero maximo */
	
	for(j = 0, m = 1; j < numMin; j++, m = 1){ /* Para cada mintermo, calcula seu numero em decimal */
		mintermos[j][digitos] = 0;
		for(i = digitos-1; i >= 0; i--, m *= 2)
			mintermos[j][digitos] += mintermos[j][i]*m; 
	}
	
	for(i = 0, k = 0, n = 0; i < totalTermos; i++, k = 0){
		/* De todos os possivel termos, acha o f<1> */
		for(j = 0; j < numMin; j++) if(i == mintermos[j][digitos]) k = 1;
		if(k) continue;
		
		for(j = 0, d = totalTermos/2, m = i; j < digitos; j++, d /= 2){
			/* Transforma em binario os numeros que nao pertercem ao mintermos */
			if(m - d < 0) maxtermos[n][j] = 0;
			else{
				maxtermos[n][j] = 1; m -= d;
			}
		}
		maxtermos[n][j] = i;
		n++;
	}
	numMax = totalTermos - numMin;
	
	return 0;
}

int guardaArvore(int nivel, int cubo[]){
	/***************************************************************/
	/**                                                           **/
	/**      Para cada implicante primo achado, guarda na arvore  **/
	/**  no seu respectivo lugar apontando para um dos cubos que  **/
	/**  cobre ele.                                               **/
	/**  int nivel: indica o nivel de onde ele será inserido      **/
	/**             (numero de X's)                               **/
	/**  int cubo[]: implicante primo que sera inserido           **/
	/**                                                           **/
	/***************************************************************/
	
	int i;
	STinter aux = malloc(sizeof(struct ST_intervalos));
	aux -> prox = NULL;
	aux -> pai = fimCubo[nivel+1];
	
	if(!nCubo[nivel]) nCubo[nivel] = (fimCubo[nivel] = aux);
	else fimCubo[nivel] = (fimCubo[nivel] -> prox = aux);
	
	for(i = 0;  i < digitos; i++ ) {
		aux -> inter[i] = cubo[i]; 
	}
	aux -> situacao = 0;
	return 0;
}

int geraIntervalos(int n, int c[], int d, int a){
	/***************************************************************/
	/**                                                           **/
	/**      Dado, (int n) o nivel, (int c[]) o cubo, (int d) a   **/
	/**  casa decimal e o (int a) o digito que será colocado,     **/
	/**  gera e guarda o respectivo implicante e depois entra     **/
	/**  na recursão para gerar dois implicantes no nivel abaixo  **/
	/**                                                           **/    
	/***************************************************************/
	int i, cubo[6];

	for(i = 0;  i < digitos; i++ ) cubo[i] = (i == d)? a : c[i];

	guardaArvore(n, cubo);
	
	for(i = 0; i < digitos; i++){
		if(cubo[i] == -1){
			geraIntervalos(n-1, cubo, i, 0);
			geraIntervalos(n-1, cubo, i, 1);
		}
	}
	return 0;
}

int pertenceMax(int c[]){
	/***************************************************************/
	/**                                                           **/    
	/**      Verifica se c[], pertence aos maxtermos              **/
	/**                                                           **/
	/***************************************************************/
	int numero = 0, i, m;
	for(i = digitos-1, m = 1; i >= 0; i--, m *= 2) numero += c[i]*m; 
	for(i = 0; i < numMax; i++) if(numero == maxtermos[i][digitos]) return 1;
	return 0;
}

int tiraComplementares(){
	/***************************************************************/
	/**                                                           **/
	/**      Para cada maxtermos, tira todos os implicantes que   **/
	/**  que cobrem ele.                                          **/
	/**                                                           **/
	/***************************************************************/
	STinter x, y;
	for(x = nCubo[0]; x ; x = x -> prox){
		if(pertenceMax(x -> inter)){
			y = x;
			while(y){
				y -> situacao = 1;
				y = y -> pai;
			}
		}
	}
	return 0;
}

int liberaMemoria(){
	int i;
	STinter x, del = NULL;
	for(i = digitos; i >= 0; i--){
		for(x = nCubo[digitos]; x; x = x -> prox){
			free(del);
			del = x;
		}
		free(del);
	}
	free(nCubo); free(fimCubo);
	return 0;
}

int main (int argc, char* argv[])
{
	int i = 0;
	
	leString(); /* Le os mintermos */
	achaComplementar(); /* Acha os maxtermos */
	
	/* Aloca os vetores de ponteiros, que apontarao para o inicio e o fim de cada nivel da arvore */
	nCubo = malloc((digitos + 1)*sizeof(struct ST_intervalos *));
	fimCubo = malloc((digitos + 1)*sizeof(struct ST_intervalos *));
	for(i = 0; i <= digitos; i++) nCubo[i] = (fimCubo[i] = NULL);
	
	/* Adiciona a arvore o 'pai', o implicante primo que cobre todos */
	nCubo[digitos] = malloc(sizeof(struct ST_intervalos));
	nCubo[digitos] -> pai = (nCubo[digitos] -> prox = NULL);
	for(i = 0; i < digitos; i++) nCubo[digitos] -> inter[i] = -1;
	fimCubo[digitos] = nCubo[digitos];
	
	/* Constroi a arvore com todos os implicantes */
	for(i = 0; i < digitos; i++){
		geraIntervalos(digitos-1, nCubo[digitos] -> inter, i, 0);
		geraIntervalos(digitos-1, nCubo[digitos] -> inter, i, 1);
	}
	
	/* Tira os maxtermos da arvore*/
	tiraComplementares();
	imprime();
	liberaMemoria();
	return 0;
	
}
	

