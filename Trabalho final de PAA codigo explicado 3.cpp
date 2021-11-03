// Trabalho final de Projeto e analise de algoritmos
// Prof. Marco antonio
// Alunos: Marcos Rodolfo C. G. Querino
//         Christy Basílio da Silvaj
 
#include <bits/stdc++.h>
 
using namespace std; 
  
// estrutura para representar arestas com peso  
class Aresta  
{  
    public: 
    int orig, dest, peso; // ponto de origem, de destino e peso  
};  // O nome dessas variaveis sugere um grafo direcionado, mas nao e nao!
  
// Estrutura para criar o grafo 
// Esse grafo sera representado por um vetor de arestas
// por ser um grafo nao direcionado, uma aresta que vai da
// origem ao destino tambem e uma aresta do destino a origem
// Ambas serao contadas apenas como uma aresta
class Grafo  
{  
    public: 
    int v, a; // v= nº de vertices, a= nº de arestas 
    Aresta* ares; // ponteiro para o vetor de arestas 
};  
  
// funcao para criar o grafo (construtor) 
Grafo* criar_grafo(int v, int a)  
{  
    Grafo* graf= new Grafo; // cria o grafo dinamicamente 
    
	graf->v= v; // insere os dados dos atributos desse objeto
    graf->a= a;  
  
    graf->ares= new Aresta[a]; // cria o vetor de aresta dinamicamente 
  
    return graf;  
}  
  
// estrutura para representar o Subconjunto com as tecnicas união por classificação e compressão de caminho
class Subconjunto  
{  
    public: 
    int pai;  
    int rank;  
};  
    
// funcao find com compressão de caminho 
int find(Subconjunto subset[], int v)  
{   
    // encontrar a raiz e torna la pai de "i" (compressao de caminho)
    if (subset[v].pai != v) // se o que esta no atributo "pai" do subconjunto "v" for diferente de "v" 
    {                       // entao "pai" recebe retorno dessa funcao, pesquisando
    	subset[v].pai= find(subset, subset[v].pai); 
	}		
    return subset[v].pai;  
}  
  
// une os subconjuntos em "vx" e "vy" por uniao por classificacao   
void Union(Subconjunto subset[], int vx, int vy) // passa o vetor e os vertices da aresta: "orig" e "dest" 
{  
    int vx_raiz= find(subset, vx);  
    int vy_raiz= find(subset, vy);  
  
    // estrutura para encontrar o subconjunto de menor "rank" para entao, coloca lo abaixo da raiz do subconjunto de maior "rank"  
    if (subset[vx_raiz].rank < subset[vy_raiz].rank) // se o subconjunto na posicao "vx_raiz" tiver "rank" menor que o do 
	{                                                // subconjunto na posicao "vy_raiz". No inicio, todos os "rank"'s sao iguais a 0
		subset[vx_raiz].pai= vy_raiz; // o pai passa a ser a raiz de "vy_raiz"
	}       
    else if (subset[vx_raiz].rank > subset[vy_raiz].rank) 
	{
		subset[vy_raiz].pai = vx_raiz; // o pai passa a ser a raiz de "vx_raiz"
	}           
    else // se os "rank"'s sao o mesmo, e no inicio, serao, entao faça um deles ser a raiz, e incremente o "rank" do outro
    {  
        subset[vy_raiz].pai= vx_raiz; // o subconjunto em "vy_raiz" será a raiz 
        subset[vx_raiz].rank++;  
    }  
}  
  
// funcao de comparacao para qsort, compara as arestas por peso  
int comparacao(const void* a, const void* b)  
{  
    Aresta* a1= (Aresta*)a; // qsort envia para ca as duas arestas a serem comparadas 
    Aresta* b1= (Aresta*)b;  
    return a1->peso > b1->peso;  
}  
  
// funcao de kruskal usando a tecnica do algoritmo guloso 
void Kruskal_fominha(Grafo* gr)  
{  
    int v= gr->v; // recebe a quantidade de vertices do grafo 
    Aresta result[v]; // vetor para guardar as arestas que formam o grafo de Kruskal
    int e= 0; // variavel de indice, sera usado no vetor result[]  
    int i= 0; // variavel de indice que sera usada para as arestas ordenadas  
    
	// 1º - ordenar as arestas em ordem crescente de peso, usando uma funcao padronizada de biblioteca de funcoes
    // da linguagem C, essa funcao possui polimorfismo e preciso passar por parametro, uma funcao para que "qsort"
    // saiba o que comparar.
    qsort(gr->ares, gr->a, sizeof(gr->ares[0]), comparacao); // parametros: vetor de arestas do grafo, a quantidade de elementos desse vetor, 
  														     // tamanho em bytes de cada elemento e função para comparar esses elementos
     
    Subconjunto* subset= new Subconjunto[( v * sizeof(subset) )]; // cria o vetor de subconjuntos de tamanho igual quantidade de vertices 
  
    // criando subconjuntos de um unico elemento, que é cada vertice do grafo (cada posicao do vetor e um subconjunto)
    for (int j= 0; j < v; j++)  
    {  
        subset[j].pai= j; // o vertice se torna pai dele mesmo 
        subset[j].rank= 0; // rank inicial e 0 
    }  
  
    // numero de arestas da arvore geradora minima é igual a: vertices do grafo - 1 
    while (e < v - 1 && i < gr->a) // é verdade que "e" é menor que (v-1) e que "i" é menor que (quantidade de arestas do grafo) 
    {  
        // 2º - pegar a aresta de menor peso e incrementar o indice para a proxima iteracao  
        Aresta prox_ares= gr->ares[i++];  
  
        int vx= find(subset, prox_ares.orig);  
        int vy= find(subset, prox_ares.dest);  
   
        // como visto no "trabalho de PAA.doc", apos o "find", verificamos se "vx" e "vy" sao iguais (isso pode ser feito dentro
        // da funcao "union" ou nao). Se sao iguais, essa aresta forma um ciclo e "union" nao atualiza o vetor de subconjuntos "subset"
		if (vx != vy)  
        {  
            result[e++]= prox_ares; // primeiro, result[0], e logo em seguida, faz o incremento de "e" 
            Union(subset, vx, vy);  
        }  
        // senao, discarta a aresta atualmente em "prox_ares"  
    }  
	// basta imprimir as arestas em "result" para mostrar a arvore geradora minima  
    cout << "\nArvore geradora minima, usando algoritmo de Kruskal:\n";  
    for (i= 0; i < e; ++i)
	{
		cout << result[i].orig << " --- " << result[i].dest << " = " << result[i].peso << endl;
	}        
}  
  
// E agora, a parte mais dificil do codigo
int main()  
{  
    int v= 9;   
    int a= 14;   
    Grafo* graf= criar_grafo(v, a);  
  
    // aresta 0-1  
    graf->ares[0].orig = 0;  
    graf->ares[0].dest = 1;  
    graf->ares[0].peso = 4;  
  
    // aresta 0-7  
    graf->ares[1].orig = 0;  
    graf->ares[1].dest = 7;  
    graf->ares[1].peso = 8;  
  
    // aresta 1-7  
    graf->ares[2].orig = 1;  
    graf->ares[2].dest = 7;  
    graf->ares[2].peso = 11;  
  
    // aresta 1-2  
    graf->ares[3].orig = 1;  
    graf->ares[3].dest = 2;  
    graf->ares[3].peso = 20;  
  
    // aresta 2-3  
    graf->ares[4].orig = 2;  
    graf->ares[4].dest = 3;  
    graf->ares[4].peso = 7;  
    
    // aresta 2-5  
    graf->ares[5].orig = 2;  
    graf->ares[5].dest = 5;  
    graf->ares[5].peso = 4;
    
    // aresta 2-8  
    graf->ares[6].orig = 2;  
    graf->ares[6].dest = 8;  
    graf->ares[6].peso = 2;
    
    // aresta 3-4  
    graf->ares[7].orig = 3;  
    graf->ares[7].dest = 4;  
    graf->ares[7].peso = 9;
    
    // aresta 3-5  
    graf->ares[8].orig = 3;  
    graf->ares[8].dest = 5;  
    graf->ares[8].peso = 14;
    
    // aresta 4-5 
    graf->ares[9].orig = 4;  
    graf->ares[9].dest = 5;  
    graf->ares[9].peso = 10;
    
    // aresta 5-6  
    graf->ares[10].orig = 5;  
    graf->ares[10].dest = 6;  
    graf->ares[10].peso = 2;
    
    // aresta 6-7 
    graf->ares[11].orig = 6;  
    graf->ares[11].dest = 7;  
    graf->ares[11].peso = 1;
    
    // aresta 6-8  
    graf->ares[12].orig = 6;  
    graf->ares[12].dest = 8;  
    graf->ares[12].peso = 20;
    
    // aresta 7-8  
    graf->ares[13].orig = 7;  
    graf->ares[13].dest = 8;  
    graf->ares[13].peso = 30;
  
    Kruskal_fominha(graf);  
  
    return 0;  
}
