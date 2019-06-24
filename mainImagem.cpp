//!  PROGRAMA DESENVOLVIDO POR CÁSSIO FERREIRA BRAGA !!!

///	 BASEADO NO CÓDIGO DO PROFESSOR LUCAS LUIS GUTKOSKI

///\
OLHA SÓ, DÁ PRA CHAMAR A ATENÇÃO COLOCANDO 3 BARRAS TAMBÉM!!!

///UÉ, AGR EU JÁ TÔ NA DISNEY \
E DÁ PRA COMENTAR A LINHA DE BAIXO COLOCANDO UMA BARRA INVERTIDA NO FINAL DA LINHA DE CIMA, SE ESSA FOR COMENTADA

/** (outra maneira de comentar de forma destacada... os fóruns são bem úteis icojsaiocjisjcioja)

    Esse programa foi testado com diversos tipos de imagens, de tamanhos variados,
    chegando até mesmo a uma imagem 8k. Lembrando que até o momento, funciona apenas
    com imagens no formato BMP 24 bits.

*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define VERSAO "5.3.0"

#define IMAGEM "Imagens\\imagem.bmp"
#define IBAGEM "Imagens\\ibagem(%d).bmp"
#define PRETOEBRANCO "Imagens\\imagemPB(%d).bmp"
#define NEGATIVA "Imagens\\imagemNegativa(%d).bmp"
#define NEGATIVAPB "Imagens\\imagemNegativaPB(%d).bmp"
#define BORRADO "Imagens\\imagemBorrada(%d).bmp"
#define BORDA "Imagens\\imagemBorda(%d).bmp"
#define AZUL "Imagens\\imagemAzul(%d).bmp"
#define VERDE "Imagens\\imagemVerde(%d).bmp"
#define VERMELHA "Imagens\\imagemVermelha(%d).bmp"

#define ALTURA (*Altura)
#define LARGURA (*Largura)
#define CABECALHO (*Cabecalho)
#define DADOS ((*Tamanho)-(*Cabecalho))

void idioma(void);

void escolha(void);
void credits(void);

void Detalhes (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *P);

void Cria (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned char *C, unsigned char *P, char *Imagem, int Num);
void Cinza (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P);
void Negativa (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned char *C, unsigned char *P);
void Borrada (unsigned long *Largura, unsigned long *Altura, unsigned char *C, int Force);
void Borda (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P, int Force);
void Colorido (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P, int *Cor);

void idioma(void)
{
    setlocale(LC_ALL, "Portuguese");
}

void escolha(void)
{
    puts("\nDIGITE [0] PARA SAIR DO PROGRAMA;");
    puts("DIGITE [y] PARA MOSTRAR OS CRÉDITOS DO TRABALHO;");
    puts("DIGITE [z] PARA MOSTRAR DETALHES DA IMAGEM;");
    puts("DIGITE [1] PARA COPIAR A IMAGEM;");
    puts("DIGITE [2] PARA CRIAR UMA CÓPIA PRETA E BRANCA DA IMAGEM;");
    puts("DIGITE [3] PARA CRIAR UMA CÓPIA NEGATIVA DA IMAGEM;");
    puts("DIGITE [4] PARA CRIAR UMA CÓPIA NEGATIVA PRETA E BRANCA DA IMAGEM;");
    puts("DIGITE [5] PARA CRIAR UMA CÓPIA BORRADA DA IMAGEM;");
    puts("DIGITE [6] PARA CRIAR UMA CÓPIA EM BORDAS DA IMAGEM;");
    puts("DIGITE [7] PARA CRIAR UMA CÓPIA AZUL;");
    puts("DIGITE [8] PARA CRIAR UMA CÓPIA VERMELHO;");
    puts("DIGITE [9] PARA CRIAR UMA CÓPIA VERDE;");
    puts("\n");
};

void credits(void)
{
    puts("\nCódigo escrito por Cássio Ferreira Braga, da turma 4324.");
    puts("Baseado em um código do prof. Lucas Luis Gutkoski.");
    puts("\nUm agradecimento especial a todos os colegas que me ajudaram com dicas e breves explicações,\nfoi de grande ajuda!");
}

void Detalhes (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *P)
{
     printf("\n\nImagem %c%c de tamanho %lu bytes\n",P[0],P[1],*Tamanho);
     printf("\nDeslocamento em bytes ate comecar a imagem : %lu\n",*Cabecalho);
     printf("\nA Imagem tem %lu pixels de largura por %lu pixels de altura\n\n",*Largura,*Altura);
}

struct cabecalho_da_img
{
  unsigned char tipo[2];//para os caracteres BM
  unsigned long tamanho;//tamanho total do arquivo em Bytes (unsigned long: 32bits = 4 bytes)
  unsigned char reservado[4];//reservado, sempre 0
  unsigned long offset;//quantos bytes até o início da imagem propriamente dita.
  unsigned long TamCab;//tamanho do cabeçalho sempre 40d ou 0x28h
  unsigned long Larg;//Largura da imagem em pixels
  unsigned long Alt;//Altura da imagem em pixels

};

int main()
{
    idioma();

    FILE *arq;

    struct cabecalho_da_img img;

    printf("ModBMP Version: %s\n", VERSAO);
    printf("\nAbrindo arquivo: %s",IMAGEM);

    if((arq = fopen(IMAGEM,"rb"))  != NULL)
    {
        printf("\n%s aberto.\n",IMAGEM);

        fread(img.tipo,sizeof(char),2,arq);//(Endereço do destino,tamanho de cada dado,quantidade de dados a ser lida,ponteiro do arquivo)
        fread(&img.tamanho,sizeof(unsigned long),1,arq); //Tamanho, em bytes
        fread(img.reservado,sizeof(unsigned char),4,arq);//lê BfReser1 e BfReser2 ,2 bytes cada
        fread(&img.offset,sizeof(unsigned long),1,arq); //Deslocamento em bytes para início da área de dados
        fread(&img.TamCab,sizeof(unsigned long),1,arq); //Tamanho do cabeçalho
        fread(&img.Larg,sizeof(unsigned long),1,arq); //Largura da imagem
        fread(&img.Alt,sizeof(unsigned long),1,arq); //Altura da imagem

        char chozaum;
        int color, borra, board, quant;

        unsigned char *pontPixel = (unsigned char*)malloc((img.tamanho-img.offset)*sizeof(unsigned char));
        unsigned char *pontPix = (unsigned char*)malloc((img.tamanho-img.offset)*sizeof(unsigned char));
        unsigned char *pontPix2;
        unsigned char *pontCab = (unsigned char*)malloc((img.offset)*sizeof(unsigned char));

        if(!pontPixel || !pontPix || !pontCab)
        {
            puts("Memória insuficiente para o funcionamento do programa! Fechando...");
            return(0);
        }

        fseek(arq,0,SEEK_SET); //Ponteiro no início do cabeçalho da imagem

        /*for(unsigned char *Header = pontCab; Header < pontCab + img.offset; )
        {
            *(Header++) = getc(arq);
        }

        for(unsigned char *Pixel = pontPixel; Pixel < pontPixel + (img.tamanho-img.offset); )
        {
            *(Pixel++) = getc(arq);
        }*/

        fread(pontCab,sizeof(unsigned char),img.offset,arq);
        fread(pontPixel,sizeof(unsigned char),(img.tamanho-img.offset),arq);

        fclose(arq);

        escolha();

        for(;;)
        {
            printf("\nSelecione uma das opções: ");
            scanf("\t%c", &chozaum); ///Colocar um caractere de espaço (' ', \n ou \t) para evitar que o espaço seja pego no próximo scan...
            ///Esse bug no scanf ocorre apenas se %c for utilizado...

            if(chozaum!='0' && chozaum!='z' && chozaum!='y')
            {
                printf("\nEscolha quantas cópias da imagem devem ser feitas (min 1, max 30): ");
                scanf("%d", &quant);

                if(quant<1 || quant>30)
                {
                    chozaum=NULL;
                }
            }

            switch(chozaum)
            {
                case '0':
                    return(0);
                break;

                case '1':
                    Cria(&img.tamanho, &img.offset, pontCab, pontPixel, IBAGEM, quant);
                break;

                case '2':
                    Cinza(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, PRETOEBRANCO, quant);
                break;

                case '3':
                    Negativa(&img.tamanho, &img.offset, pontPix, pontPixel);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, NEGATIVA, quant);
                break;

                case '4':
                    Cinza(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel);
                    Negativa(&img.tamanho, &img.offset, pontPix, pontPix);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, NEGATIVAPB, quant);
                break;

                case '5':
                    puts("\nSelecione qual o tamanho da matriz que borrará a imagem.");
                    puts("\tEx: dígito 3 -> Matriz 3x3, dígito 5 -> Matriz 5x5.\n");
                    printf("Digite o valor para escolher o tamanho da matriz: ");

                    scanf("%d", &borra);
                    puts(" ");

                    if(borra<2 || borra>31)
                    {
                        puts("É melhor escolher um tamanho entre 2 e 31...");
                    }
                    else
                    {
                        if(borra%2)
                        {
                            Cinza(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel);
                            Borrada(&img.Larg, &img.Alt, pontPix, borra);
                            Cria(&img.tamanho, &img.offset, pontCab, pontPix, BORRADO, quant);
                        }
                        else
                        {
                            borra++;
                            Cinza(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel);
                            Borrada(&img.Larg, &img.Alt, pontPix, borra);
                            Cria(&img.tamanho, &img.offset, pontCab, pontPix, BORRADO, quant);
                        }
                    }
                break;

                case '6':
                    pontPix2 = (unsigned char*)malloc((img.tamanho-img.offset)*sizeof(unsigned char));

                    ///strcpy(pontPix2,pontPix); //Não funcionou, provavelmente porque trabalha apenas com char, e não unsigned char.

                    if(!pontPix2)
                    {
                        puts("Memória insuficiente para executar esta ação!");
                    }
                    else
                    {
                        puts("\nSelecione qual o tamanho da matriz de bordas.");
                        puts("\tEx: dígito 3 -> Matriz 3x3, dígito 5 -> Matriz 5x5.\n");
                        printf("Digite o valor para escolher o tamanho da matriz: ");

                        scanf("%d", &board);
                        puts(" ");

                        if(board<2 || board>9)
                        {
                            puts("É melhor escolher um tamanho entre 2 e 9...");
                        }
                        else
                        {
                            Cinza(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel);
                            memcpy(pontPix2, pontPix, (img.tamanho-img.offset)*sizeof(unsigned char));

                            if(board%2)
                            {
                                Borda(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPix2, board);
                                Cria(&img.tamanho, &img.offset, pontCab, pontPix, BORDA, quant);
                            }
                            else
                            {
                                board++;
                                Borda(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPix2, board);
                                Cria(&img.tamanho, &img.offset, pontCab, pontPix, BORDA, quant);
                            }
                        }
                    }
                    free(pontPix2);
                break;

                case '7':
                    color=0; //!BLUE
                    Colorido(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel, &color);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, AZUL, quant);
                break;

                case '8':
                    color=2; //!RED
                    Colorido(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel, &color);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, VERMELHA, quant);
                break;

                case '9':
                    color=1; //!GREEN
                    Colorido(&img.tamanho, &img.offset, &img.Larg, &img.Alt, pontPix, pontPixel, &color);
                    Cria(&img.tamanho, &img.offset, pontCab, pontPix, VERDE, quant);
                break;

                case 'z':
                    Detalhes(&img.tamanho, &img.offset, &img.Larg, &img.Alt, img.tipo);
                break;

                case 'y':
                    credits();
                break;

                default:
                    {
                        puts("\nValor digitado inválido");
                    }
            }//!FIM DO SWITCH
        }//!FIM DO FOREVER
    }//!FIM DO IF ARQ
    else
    {
        printf("\nNão foi possível abrir o arquivo: %s.\n", IMAGEM);
    }
}//!FIM DO INT MAIN

void Cria (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned char *C, unsigned char *P, char *Imagem, int Num)
{
    char *Moso = (char*)malloc(50);

    FILE *copia;

    for(int y=Num; y-->0; )
    {
        sprintf(Moso, Imagem, y);

        if((copia = fopen(Moso,"w+b"))  != NULL)
        {
            /*for(unsigned char *Cab=C; Cab< C+(*Cabecalho); )
            {
                putc(*(Cab++),copia);
            }

            for(unsigned char *Img=P; Img< P+((*Tamanho)-(*Cabecalho)); )
            {
                putc(*(Img++),copia);
            }*/

            fwrite(C,sizeof(unsigned char),CABECALHO,copia);
            fwrite(P,sizeof(unsigned char),DADOS,copia);

            printf("\nImagem ");
            printf("%s ", (Moso));
            puts("criada!");
            fclose(copia);
        }
        else
        {
            printf("\nNão foi possível abrir o arquivo: %s.", IMAGEM);
            fclose(copia);
        }
    }
    free(Moso);
}

void Cinza (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P)
{
    long u=0;
    unsigned int i=0;

    for(unsigned char *Bl=C, *Wh=P;Bl<C+(ALTURA*LARGURA*3);Bl+=3,Wh+=3,i+=3)
    {
        u=(i/(LARGURA*3))*((LARGURA%4)); //!!! LIXO DE CADA LINHA, PARA NÃO OCASIONAR ERROS NA IMAGEM !!!

        *(Bl+u) = *(Bl+u+1) = *(Bl+u+2) = (*(Wh+u) + *(Wh+u+1) + *(Wh+u+2))/3;
    }
}

void Negativa (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned char *C, unsigned char *P)
{
    for(unsigned long i=0; i<((*Tamanho)-(*Cabecalho));++i)
    {
        C[i] = -1*P[i];
    }
}

void Borrada (unsigned long *Largura, unsigned long *Altura, unsigned char *C, int Force)
{
    long Borra, u;
    unsigned long twerk;
    int divBorra = (Force*Force*3);
    int divForce = Force/2;
    unsigned long BordaNaoBorra = ((divForce*LARGURA)+divForce);

    for(unsigned long i=(ALTURA*LARGURA)-BordaNaoBorra; --i>BordaNaoBorra; )
    {
        Borra = 0;

        if((i%LARGURA)<divForce)
        {
            i -= (Force);
        }

        u=(i/(LARGURA))*((LARGURA%4)); //!!! LIXO DE CADA LINHA, PARA NÃO OCASIONAR ERROS NA IMAGEM !!!

        twerk = ((i/(LARGURA))*LARGURA*3)+(i%(LARGURA))*3+u;

        for(int k=-divForce;k<divForce+1;++k)
        {
            for(int j=-divForce*3;j<(divForce+1)*3;++j)
            {
                Borra += C[twerk+(k*LARGURA*3)+j];
            }
        }

        Borra /= divBorra;

        C[twerk] = C[twerk+1] = C[twerk+2] = Borra;
    }
}

void Borda (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P, int Force)
{
    long Borra=0, u=0;

    /*for(unsigned long i=0; i<((*Tamanho)-(*Cabecalho));++i)
    {
        P[i] = C[i];
    }*/

    for(unsigned int i=(Force/2);i+(Force/2)<ALTURA;i++)
    {
        for(unsigned int l=(Force/2);l+(Force/2)<LARGURA;l++)
        {
            u=i*((LARGURA%4)); //!!! LIXO DE CADA LINHA, PARA NÃO OCASIONAR ERROS NA IMAGEM !!!

            Borra = 0;

            for(int k=-(Force/2);k<(Force/2)+1;++k)
            {
                for(int j=-(Force/2)*3;j<((Force/2)+1)*3;++j)
                {
                    if(k==0 && j>-1 && j<3)
                    {
                        Borra += P[(i*LARGURA*3)+(k*LARGURA*3)+l*3+j+u]*((Force*Force)-1);
                    }
                    else
                    {
                        Borra += P[(i*LARGURA*3)+(k*LARGURA*3)+l*3+j+u]*(-1);
                    }
                }
            }

            if(Borra > 255)
            {
                Borra = 255;
            }
            else if(Borra < 0)
            {
                Borra = 0;
            }

            C[(i*LARGURA*3)+(0*LARGURA*3)+l*3+0+u] = Borra;
            C[(i*LARGURA*3)+(0*LARGURA*3)+l*3+1+u] = Borra;
            C[(i*LARGURA*3)+(0*LARGURA*3)+l*3+2+u] = Borra;
        }
    }
}

void Colorido (unsigned long *Tamanho, unsigned long *Cabecalho, unsigned long *Largura, unsigned long *Altura, unsigned char *C, unsigned char *P, int *Cor)
{
    long u=0;

    for(unsigned int i=0;i<ALTURA*LARGURA*3;i+=3)
    {
        u=(i/(LARGURA*3))*((LARGURA%4)); //!!! LIXO DE CADA LINHA, PARA NÃO OCASIONAR ERROS NA IMAGEM !!!

        C[i+u] = 0;
        C[i+u+1] = 0;
        C[i+u+2] = 0;

        C[i+u+(*Cor)] = P[i+u+(*Cor)];
    }

}

