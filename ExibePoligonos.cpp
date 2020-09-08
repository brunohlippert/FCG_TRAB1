// **********************************************************************
// PUCRS/Escola PolitŽcnica
// COMPUTA‚ÌO GRçFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"
#include "Faixa.h"
#include "Aresta.h"

#include "Temporizador.h"
Temporizador T;
double AccumDeltaT=0;
float numFaixas = 10;

Poligono Mapa;
Poligono ConvexHull;
// Limites l—gicos da ‡rea de desenho
Ponto Min, Max;

vector <Faixa> faixas;
float distanciaEntreFaixas;

// **********************************************************************
//    Calcula o produto escalar entre os vetores V1 e V2
// **********************************************************************
double ProdEscalar(Ponto v1, Ponto v2)
{
    return v1.x*v2.x + v1.y*v2.y+ v1.z*v2.z;
}
// **********************************************************************
//    Calcula o produto vetorial entre os vetores V1 e V2
// **********************************************************************
void ProdVetorial (Ponto v1, Ponto v2, Ponto &vresult)
{
    vresult.x = v1.y * v2.z - (v1.z * v2.y);
    vresult.y = v1.z * v2.x - (v1.x * v2.z);
    vresult.z = v1.x * v2.y - (v1.y * v2.x);
}
/* ********************************************************************** */
/*                                                                        */
/*  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)             */
/*                                                                        */
/* k : ponto inicial da reta 1                                            */
/* l : ponto final da reta 1                                              */
/* m : ponto inicial da reta 2                                            */
/* n : ponto final da reta 2                                              */
/*                                                                        */
/* s: valor do par‰metro no ponto de interse‹o (sobre a reta KL)         */
/* t: valor do par‰metro no ponto de interse‹o (sobre a reta MN)         */
/*                                                                        */
/* ********************************************************************** */
int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // n‹o h‡ intersec‹o

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // h‡ intersec‹o
}

// **********************************************************************
bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = intersec2d( k,  l,  m,  n, s, t);
    if (!ret) return false;
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0)
        return true;
    else return false;
}
// **********************************************************************
// **********************************************************************



void LeMapa(const char *nome)

{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    int nLinha = 0;
    unsigned int qtdVertices;

    input >> qtdVertices;
    double x,y;

    // Le a primeira linha apenas para facilitar o calculo do limites
    input >> x >> y;

    Min = Ponto(x,y);
    Max = Ponto(x,y);
    Mapa.insereVertice(Ponto(x,y));
    for (int i=0; i< qtdVertices; i++)
    {
        // Le cada elemento da linha
        input >> x >> y;
        // atualiza os limites
        if (x<Min.x) Min.x = x;
        if (y<Min.y) Min.y = y;

        if (x>Max.x) Max.x = x;
        if (y>Max.y) Max.y = y;
        if(!input)
            break;
        nLinha++;
        //cout << "Arquivo: " << x << " " << y << endl;
        Mapa.insereVertice(Ponto(x,y));
    }
    cout << "leitura concluida." << endl;
    //cout << "Linhas lidas: " << nLinha << endl;
    //cout << "Limites:" << endl;
    cout << "Minimo:"; Min.imprime();
    cout << "\tMaximo:"; Max.imprime();
    cout << endl;
}


//
//          IMPLEMENTACOES DO TRABALHO
//

/**
 * Carrega as faixas com base no tamanho do poligono desenhado.
 */
void carregaFaixas(){
    float deltaTamanho = (Max.y - Min.y);
    distanciaEntreFaixas = deltaTamanho / numFaixas;

    for(int i = 0; i < numFaixas; i++){
        Faixa novaFaixa = Faixa((Min.y)+ (i * distanciaEntreFaixas), distanciaEntreFaixas);

        faixas.push_back(novaFaixa);
    }

    //Adiciona as arestas nas faixas correspondentes.
    for(int i = 0; i < Mapa.getNVertices() - 1; i++){
        Aresta novaAresta = Aresta(Mapa.getVertice(i), Mapa.getVertice(i+1));

        //Calcula o intervalo de faixas a qual a aresta esta presente
        //Fazemos - Min.y para descobrir a distancia do menor ponto ate o ponto y testado
        //Pois caso o y fosse negativo apenas dividir nao retornaria o index na lista de faixas
        int faixaInicial = (int)((novaAresta.getP1().y - Min.y)/ distanciaEntreFaixas);
        int faixaFinal = (int)((novaAresta.getP2().y - Min.y) / distanciaEntreFaixas);

        //Passamos pelo intervalo de faixas adicionando a nova aresta
        for(int faixa = faixaInicial; faixa < faixaFinal + 1; faixa++){
            faixas[faixa].addAresta(novaAresta);
        }
    }

    for(int i = 0; i < numFaixas; i++){
        cout << "Faixa " << i << " arestas: " << faixas[i].getArestas().size() << endl;
    }
}

/**
 * Vetor gerado por dois pontos.
 *
 * @param Ponto p1: Ponto A do vetor a ser gerado (origem)
 * @param Ponto p2: Ponto B do vetor a ser gerado (destino)
 * @return Ponto vetor gerado
 */
Ponto getVetor(Ponto p1, Ponto p2){
    return Ponto(p2.x - p1.x, p2.y - p1.y);
}

/**
 * Modulo do vetor.
 *
 * @param Ponto vetor: vetor que se quer descobrir o modulo.
 * @return modulo do vetor.
 */
double moduloVetor(Ponto vetor){
    return sqrt((vetor.x * vetor.x) + (vetor.y * vetor.y));
}


/**
 * Angulo entre vetor e a linha horizontal.
 *
 * Calcula o angulo entre o vetor passado e a linha horizontal levando
 * em conta se o ponto atual do convex hull esta do lado direito ou esquerdo
 * do poligono
 *
 * @param Ponto v2: vetor que se quer descobrir o angulo.
 * @param int ladoDireito: inteiro que define se o ponto inicial do vetor v2 esta
 * do lado direito do poligono ou nao.
 * @return angulo externo em graus entre o vetor v2 e a linha horizonta.
 */
double getAnguloHorizontal(Ponto v2, int ladoDireito){

    //Se for o lado esquerdo do poligono, usamos o vetor na direcao negativa de x
    Ponto v1 = Ponto(-1, 0);

    //Senao, na direcao positiva
    if(ladoDireito == 1){
        v1 = Ponto(1, 0);
    }

    double proEsc = ProdEscalar((v1), (v2));
    double angl = proEsc / (moduloVetor(v1) * moduloVetor(v2));
    double graus = acos(angl) * (180.0/3.141592653589793238463);

    //Se for o lado direito do poligono temos que contar apenas
    //o angulo "da direita" entre a horizontal e a linha
    //Se for o lado esquerdo, temos que calcular o angulo "da esquerda"
    if(ladoDireito == 1){
        //Se for 180, sao 0 graus de diferenca da horizontal
        if(graus == 180){
            graus = 0;

        //Se o y esta descendo, o prod escalar da o angulo do lado "de baixo"
        //Entre a horizontal e a linha, entao diminuimos o angulo de 360
        //Para ter o angulo obtuso "de cima"
        }else if(v2.y < 0){
            graus = 360 - graus;
        }
        return graus;
    } else {
        //Se for 180, sao 0 graus de diferenca da horizontal
        if(graus == 180){
            graus = 0;

        //Se o y esta subindo, o prod escalar da o angulo do lado "de cima"
        //Entre a horizontal e a linha, entao diminuimos o angulo de 360
        //Para ter o angulo obtuso "de baixo"
        }else if(v2.y > 0){
            graus = 360 - graus;
        }
        return graus;
    }

}

/**
 * Adiciona ao Poligono ConvexHull os pontos do Poligono Mapa
 * que geram o convex hull de Mapa.
 *
 * @return void
 */
void getConvexHull(){
    //Copia dos vertices para que possamos excluir os ja visitados.
    vector <Ponto> vertices = Mapa.cpyPontosMapa();

    //Ponto com menor Y em Mapa para iniciar o algoritmo;
    int indexMinimo = Mapa.getMinimo();
    Ponto ptMinimo = vertices[indexMinimo];

    //Ponto com maior Y em Mapa para trocar a logica dos angulos em getAnguloHorizontal().
    int indexMaximo = Mapa.getMaximo();
    Ponto ptMaximo = vertices[indexMaximo];

    //Inicialemente, vamos para o lado direito do poligono.
    int ladoDireito = 1;

    //Ponto que usamos a cada iteracao para calcular os angulos.
    Ponto atual = ptMinimo;

    //While true.
    while(1==1){
        //Calculamos primeiramente para o primeiro ponto do vetor.
        double menorAngulo = getAnguloHorizontal(getVetor(atual ,vertices[0]), ladoDireito);
        Ponto auxAdd = vertices[0];
        int indexMenorAngulo = 0;

        //Verificamos se existem outro com um angulo menor.
        for (int i=1; i< vertices.size(); i++){
            double auxAngulo = getAnguloHorizontal(getVetor(atual ,vertices[i]), ladoDireito);
            if(auxAngulo < menorAngulo){
                menorAngulo = auxAngulo;
                auxAdd = vertices[i];
                indexMenorAngulo = i;
            }
        }

        //Atualizamos o novo ponto e excluimos da lista de vertices.
        atual = auxAdd;
        ConvexHull.insereVertice(auxAdd);
        vertices.erase(vertices.begin() + indexMenorAngulo);

        //Se for o ponto de maximo Y entao mudamos a estrategia dos angulos
        //para o lado direito do poligono.
        if(auxAdd.x == ptMaximo.x && auxAdd.y == ptMaximo.y){
            ladoDireito = 0;
        }

        //Se chegou no ponto minimo novamente, então finalizou
        if(auxAdd.x == ptMinimo.x && auxAdd.y == ptMinimo.y){
            break;
        }
    }
}


// **********************************************************************
//
// **********************************************************************
void initOLD()
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    LeMapa("EstadoRS.txt");
    getConvexHull();
    Min.x--;Min.y--;
    Max.x++;Max.y++;
    carregaFaixas();
    //cout << "Vertices no Vetor: " << Mapa.getNVertices() << endl;

}
// **********************************************************************
//  void init(void)
//  Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
	// Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Leitura do arquivo
    Mapa.insereVertice(Ponto(-2,1));
    Mapa.insereVertice(Ponto(-3,5));
    Mapa.insereVertice(Ponto(3,3));
    Mapa.insereVertice(Ponto(5,5));
    Mapa.insereVertice(Ponto(5,3));
    Mapa.insereVertice(Ponto(8,-5));
    Mapa.insereVertice(Ponto(4,-1));
    Mapa.insereVertice(Ponto(-1,-1));
    Mapa.insereVertice(Ponto(-4,-3));
    Mapa.insereVertice(Ponto(-2,1));

    getConvexHull();

    // Seta os limites da ‡rea de desenho
    Min = Ponto(-10, -10, 0);
    Max = Ponto( 10,  10, 1);

    carregaFaixas();
}


double nFrames=0;
double TempoTotal=0;
// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza‹o da tela em 30
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        //cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        //cout << "Nros de Frames sem desenho: " << nFrames << endl;
        //cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x,
            Min.y,Max.y,
            0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}

// **********************************************************************
//
// **********************************************************************
void DesenhaFaixas()
{
    for(int i = 0; i < faixas.size(); i++){
        faixas[i].desenhaFaixa(Min.x, Max.x);
    }
}

// **********************************************************************
//  void display( void )
//
// **********************************************************************
void display( void )
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites lógicos da área OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();

    glLineWidth(2);
    glColor3f(1,1,0); // R, G, B  [0..1]
    Mapa.desenhaPoligono();

    glLineWidth(2);
    glColor3f(1,0,0); // R, G, B  [0..1]
    ConvexHull.desenhaPoligono();

	glPointSize(5);
    glColor3f(0,1,0); // R, G, B  [0..1]
    Mapa.desenhaVertices();

    glLineWidth(1);
    glColor3f(0.5,0.5,0); // R, G, B  [0..1]
    DesenhaFaixas();

	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo nœmero de segundos e informa quanto frames
// se passaram neste per’odo.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // Se pressionar UP
			glutFullScreen ( ); // Vai para Full Screen
			break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP
								// Reposiciona a janela
            glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializações
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida‹o da tela. A funcao "display"
    // será chamada automaticamente sempre que a
    // m‡quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
