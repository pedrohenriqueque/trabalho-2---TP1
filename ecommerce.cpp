#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
//***************************************************************************************
// Fun��es utilit�rias
//***************************************************************************************

/*
 * Remove os espa�os em branco do in�cio e do fim da string "str"
 */
void trim(char str[]) {
    int i, j;

    // Limpa os brancos do fim
    j = strlen(str)-1;
    while (j >= 0 && str[j] == ' ')
        str[j--] = 0;

    // Procura o 1o caracter diferente de branco ou o fim da string
    j = 0;
    while (str[j] != 0 && str[j] == ' ')
       j++;

     // Se a string n�o est� vazia, remove os brancos do inicio
    if (str[j] != 0) {
        i = 0;
        while (str[j] != 0)
            str[i++] = str[j++];
        str[i] = 0;
    }
}

/*
 * Verifica se a string "str" � composta somente dos caracteres existentes em "validos"
 */
bool contem_caracteres(const char str[], const char validos[]) {
    for (int i = 0; str[i] != 0; i++)
        if (! strchr(validos, str[i]))
            return false;

    return true;
}

/*
 * Conta a quantidade de casas decimais em "str"
 * Essa fun��o assume que "str" cont�m um numero real v�lido
 */
int conta_decimais(const char str[])
{
    int i = 0, dec = 0;

    // Se come�a com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Pula os digitos at� o ponto
    while (str[i] != 0 && str[i] != '.')
        i++;

    // Conta os digitos ap�s o ponto
    if (str[i] == '.') {
        i++;
        while (str[i] != 0) {
            i++;
            dec++;
        }
    }

    return dec;
}

/*
 * Verifica se a string "str" representa um n�mero real:
 * - Pode come�ar com sinal de menos ou n�o
 * - O n�mero pode ter ponto decimal ou n�o (no m�ximo um)
 * - Os demais caracteres tem que ser d�gitos 0..9
 */
bool eh_numero_real(const char str[]) {
    bool achou_ponto = false;
    int i = 0;

    // Se come�a com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string s� tem d�gitos e, no m�ximo, um ponto
    while (str[i] != 0) {
        if (! isdigit(str[i]))
            if (str[i] == '.') {
                // Verifica se � ou n�o o primeiro ponto que encontrou
                if (achou_ponto)
                    return false;
                else
                    achou_ponto = true;
            }
            else
                return false;
        i++;
    }

    return true;
}

/*
 * Verifica se a string "str" representa um n�mero inteiro:
 * - Pode come�ar com sinal de menos ou n�o
 * - Os demais caracteres tem que ser d�gitos 0..9
 */
bool eh_numero_inteiro(const char str[]) {
    int i = 0;

    // Se come�a com sinal de menos, pula o sinal
    if (str[0] == '-')
        i++;

    // Verifica se a string s� tem d�gitos
    while (str[i] != 0) {
        if (! isdigit(str[i]))
            return false;
        i++;
    }

    return true;
}

/*
 * Separa "str" no formato DDMMAAAA nos componentes DD, MM e AAAA
 * str deve ter, obrigatoriamente, 8 d�gitos
 */
void separa_dma(char str[],int &mes, int &ano) {

    // fun��o atoi (ascii to integer)
    // "08022022" -> 8022022
    
    int dt = atoi(str);

    ano = dt % 10000;
    mes = dt / 10000 % 100;
}

//***************************************************************************************
// Fun��es para valida��o de datas
//***************************************************************************************

bool eh_bissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int numero_dias_mes(int mes, int ano)
{
    if (mes < 1 || mes > 12 || ano <= 0)
        return -1;

    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 ||
        mes == 8 || mes == 10 || mes == 12)
            return 31;
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
        return 30;
    else if (eh_bissexto(ano))
        return 29;
    else
        return 28;
}

bool eh_data_valida(int dia, int mes, int ano)
{
    return ano > 0 &&
           mes >= 1 && mes <= 12 &&
           dia >= 1 && dia <= numero_dias_mes(mes, ano);
}

//***************************************************************************************
// Fun��es para leitura de strings
//***************************************************************************************

/*
 * L� uma string com no m�ximo "max" caracteres e armazena em "buffer"
 * Os caracteres a mais ser�o descartados
 */
void le_string(char buffer[], int max) {
    // L� no m�ximo "max-1" caracteres
    fgets(buffer, max, stdin);

    // Substitui o ultimo caracter por NULO se ele for igual a \n
    int tam = strlen(buffer);

    if (buffer[tam-1] == '\n')
        buffer[tam-1] = 0;

    // Limpar o que n�o foi lido pelo fgets
    //fflush(stdin);
}

/*
 * L� uma string com tamanho >= "min" e tamanho <= "max" e armazena em "str"
 */
void le_string(const char label[], const char msg_erro[], char str[], int min, int max) {
    char buffer[1000]; // Buffer tempor�rio para leitura dos dados
    int t;

    do {
        printf("%s", label);
        le_string(buffer, 1000);
        trim(buffer);
        t = strlen(buffer);
        if (t < min || t > max)
            puts(msg_erro);
    } while (t < min || t > max);

    // Copia o dado lido para str
    strcpy(str, buffer);
}

/*
 * L� uma string com tamanho no intervalo (min, max) e armazena em "str"
 * A string lida dever� ser composta somente dos caracteres definidos em "caracteres_validos"
 */
void le_string(const char label[], const char msg_erro[], char str[], int min, int max, const char caracteres_validos[]) {
    bool valido;

    do {
        le_string(label, msg_erro, str, min, max);

        // Verifica se todos os caracteres de str pertencem a caracteres_validos
        valido = contem_caracteres(str, caracteres_validos);

        if (! valido)
            puts(msg_erro);

    } while (! valido);
}

//***************************************************************************************
// Fun��es para leitura de caracteres
//***************************************************************************************

/*
 * L� um caracter e verifica se ele � v�lido
 */
char le_caracter(const char label[], const char msg_erro[], const char validos[], bool maiuscula){
    char buffer[2];

    for (;;) {
        le_string(label, msg_erro, buffer, 1, 1);

        char c = buffer[0];

        if (maiuscula && c >= 'a' && c <= 'z')
            c -= 32;
        else if (! maiuscula && c >= 'A' && c <= 'Z')
            c += 32;

        // Verifica se o caracter � v�lido
        if (! strchr(validos, c))
            puts(msg_erro);
        else
            return c;
    }
}

//***************************************************************************************
// Fun��es para leitura de n�mero reais e inteiros
//***************************************************************************************

/*
 * L� e retorna um numero real:
 * - O n�mero deve ter casas decimais no intervalo (min_dec, max_dec)
 * - O numero deve estar no intervalo (min, max)
 */
double le_real(const char label[], const char msg_erro[], int min_dec, int max_dec, double min, double max) {
    bool valido;
    char buffer[31]; // Assume que um n�mero n�o vai ter mais de 30 d�gitos
    double n;

    do {
        // Assume que um n�mero n�o vai ter mais de 30 d�gitos
        le_string(label, msg_erro, buffer, 0, 30);

        // Verifica a string � um numero real
        if (! eh_numero_real(buffer))
            valido = false;
        else {
            // Conta as casas decimais para verificar se est� no intervalo definido
            int decimais = conta_decimais(buffer);

            if (decimais < min_dec || decimais > max_dec)
                valido = false;
            else {
                // Converte a string em double
                n = strtod(buffer, NULL);

                // Verifica se N est� no intervalo definido
                valido = n >= min && n <= max;
            }
        }

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    return n;
}

/*
 * L� e retorna um numero inteiro:
 * - O numero deve estar no intervalo (min, max) inclusive
 */
long le_inteiro(const char label[], const char msg_erro[], long min, long max) {
    bool valido;
    char buffer[31]; // Assumi que um n�mero n�o vai ter mais de 30 d�gitos
    long n;

    do {
        // Assumi que um n�mero n�o vai ter mais de 30 d�gitos
        le_string(label, msg_erro, buffer, 0, 30);

        // Verifica a string � um numero inteiro
        if (! eh_numero_inteiro(buffer))
            valido = false;
        else {
            // Converte a string em long
            n = atol(buffer);

            // Verifica se N est� no intervalo definido
            valido = n >= min && n <= max;
        }

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    return n;
}

//***************************************************************************************
// Fun��es para leitura de datas no formato DDMMAAAA
//***************************************************************************************

struct Data {
    int dia;
    int mes;
    int ano;
    int hora;
    int min;
    int seg;
};
void data_hora_atual(int &dia, int &mes, int &ano,
int &hora, int &min, int &seg) {
time_t t = time(NULL);
struct tm lt = *localtime(&t);
ano = lt.tm_year + 1900;
mes = lt.tm_mon + 1;
dia = lt.tm_mday;
hora = lt.tm_hour;
min = lt.tm_min;
seg = lt.tm_sec;
}
Data le_data(const char label[], const char msg_erro[]) {
    bool valido;
    int m, a;
    char dt[6];
    Data data;

    do {
        le_string(label, msg_erro, dt, 6, 6, "0123456789");

        // Separa a data lida de  mês e ano
        separa_dma(dt, m, a);

        // Verifica se mês e ano formam uma data válida
        valido = eh_data_valida(1, m, a); 

        if (! valido)
            puts(msg_erro);

    } while (! valido);

    data.dia = 1; 
    data.mes = m;
    data.ano = a;

    return data;
}

//***************************************************************************************
// Fun��es para leitura e valida��o de CPF
//***************************************************************************************

/*
 * Valida um n�mero e verifica se ele representa um cpf v�lido
 */
bool cpf_valido(long long cpf)
{
    int primeiro_dv, segundo_dv, soma, resto, j, k;
    long long numero_cpf;
    int mult[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    // Se cpf n�o est� no intervalo v�lido ou se tem todos os d�gitos iguais
    // ent�o � inv�lido
    if (cpf < 11111111111L || cpf > 99999999999L || cpf % 11111111111L == 0)
        return false;

    // Pega o primeiro DV e o segundo DV do cpf
    primeiro_dv = cpf % 100 / 10,
    segundo_dv  = cpf % 10;

    // Calcula primeiro DV
    numero_cpf = cpf / 100; // N�mero do cpf sem os DVs
    soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
        j = 0;
    else
        j = 11 - resto;

    // Compara o primeiro DV calculado com o informado
    // Se � inv�lido, ent�o nem precisa calcular o segundo
    if (j != primeiro_dv)
        return false;

    // Calcula segundo DV
    numero_cpf = cpf / 10;  // N�mero do cpf sem segundo DV
    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (numero_cpf % 10) * mult[i];
        numero_cpf /= 10;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1)
        k = 0;
    else
        k = 11 - resto;

    // Compara o segundo DV calculado com o informado
    if (k != segundo_dv)
        return false;

    return true;
}

/*
 * L� um CPF v�lido e retorna como LONG LONG
 */
long long le_cpf(const char label[], const char msg_erro[]) {
    char cpf[12];
    bool valido;
    long long numero_cpf;

    do {
        le_string(label, msg_erro, cpf, 11, 11, "0123456789");

        numero_cpf = atoll(cpf);

        valido = cpf_valido(numero_cpf);

        if (! valido)
            puts(msg_erro);
    } while (! valido);

    return numero_cpf;
}

/*
 * L� CPF v�lido e retorna como STRING
 */
void le_cpf(const char label[], const char msg_erro[], char cpf[]) {
    bool valido;
    long long numero_cpf;

    do {
        le_string(label, msg_erro, cpf, 11, 11, "0123456789");

        numero_cpf = atoll(cpf);

        valido = cpf_valido(numero_cpf);

        if (! valido)
            puts(msg_erro);
    } while (! valido);
}

//***************************************************************************************
// Fun��es para leitura e valida��o de cart�es de cr�dito
//***************************************************************************************

/*
 * L� um numero de cart�o de cr�dito v�lido e retorna como LONG LONG
 */
long long le_numero_cc(const char label[], const char msg_erro[]) {
    char cc[17];
    bool valido;
    long long numero_cc;

    do {
        le_string(label, msg_erro, cc, 16, 16, "0123456789");

        if (! valido)
            puts(msg_erro);
    } while (! valido);

    return numero_cc;
}

/*
 * L� um numero de cart�o de cr�dito v�lido e retorna como STRING
 */
void le_numero_cc(const char label[], const char msg_erro[], char cc[]) {
    bool valido;

    do {
        le_string(label, msg_erro, cc, 16, 16, "0123456789");

        if (! valido)
            puts(msg_erro);
    } while (! valido);
}
struct Produto{
    int codigo;
    char descricao[41];
    char categoria;
    int qtd_estoque;
    float preco_maximo;
    int desconto;
};

struct Pedido{
    int numero;
    Data dt;
    char cpf[14];
    long long nc;
    Produto produtos[100];
};

void incluirProduto(Produto lista_produto[],int &qtd){
   int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    if(qtd > 0){
    for(int i = 0 ; i < qtd ; i++){
        if(codigo == lista_produto[i].codigo){
        puts("O Codigo ja foi Utilizado");
        return;
    }
    }
    }
    lista_produto[qtd].codigo = codigo;
    le_string("Descricao: ", "Descricao deve ter de 3 a 40 caracteres", lista_produto[qtd].descricao, 4, 40);
    lista_produto[qtd].categoria = le_caracter("Conceito (A, B, C , D ou E): ", "Conceito invalido", "ABCDE", true);
    lista_produto[qtd].qtd_estoque = le_inteiro("Quantidade do Produto: ", "A Quantidade do Produto deve estar entre  1 a 9999", 1, 9999);
    lista_produto[qtd].preco_maximo = le_real("Preco: ", "Preco deve ser maior que zero com ate duas casas decimais", 0, 2, 0.01, 9999999.99);
    lista_produto[qtd].desconto = le_inteiro("Desconto: ", "O Codigo deve estar entre  0 a 99", 0, 99);
    qtd++;
     
}
void alterarProduto(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int desconto = 0;
    int preco_maximo = 0;
    int estoque = 0;
    for(int i = 0 ; i < qtd_carrinho ; i++){
        if(codigo == carrinho[i].codigo){
            puts("Nao eh possivel alterar um produto que esta no carrinho");
            return;
        }
    }
    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo == lista_produto[i].codigo){
        do{
            estoque = le_inteiro("Quantidade do Produto: ", "A Quantidade do Produto deve ser maior que a anterior ou  menor que 9999",0, 9999);
                if(estoque == 0)
                    break;
                if(estoque < lista_produto[i].qtd_estoque)
                    puts("A Quantidade do Produto deve ser maior que a anterior ou  menor que 9999");    
        }while(estoque < lista_produto[i].qtd_estoque);
            if(estoque != 0)
                lista_produto[i].qtd_estoque = estoque; 
            preco_maximo = le_real("Preco: ", "Preco deve ser maior que zero com ate duas casas decimais", 0, 2, 0, 999999999);
            if(preco_maximo > 0)
                lista_produto[i].preco_maximo = preco_maximo;
            desconto = le_inteiro("Desconto: ", "O Codigo deve estar entre  0 a 99", -1, 99);
            if(desconto >= 0)
                lista_produto[i].desconto = desconto;
        }
    }   
}
void incluirCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int comprados = -1;
    int contador = 0;
    for(int i = 0 ; i < qtd_carrinho ; i++){
        if(codigo == carrinho[i].codigo){
        puts("O Codigo ja foi Utilizado");
        return;
    }
    }
    for(int i = 0 ; i < qtd_produto; i++)
        if(codigo != lista_produto[i].codigo)
            contador++;
    if(contador == qtd_produto){
        puts("Produto nao existe");
        return;
    }

    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo == lista_produto[i].codigo){
            comprados = le_inteiro("Quantidade do Produto: ", "A Quantidade do Produto deve ser menor ou igual ao do estoque",1,lista_produto[i].qtd_estoque);
            carrinho[qtd_carrinho] = lista_produto[i];
            lista_produto[i].qtd_estoque = lista_produto[i].qtd_estoque - comprados;
            carrinho[qtd_carrinho].qtd_estoque = comprados;
            qtd_carrinho++;
}
}
}
void aumentarCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int aumentar = -1;
    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo == lista_produto[i].codigo){
            aumentar = i;
        }
    }
    if(aumentar == -1)
        puts("ERRO");
    for(int i = 0 ; i < qtd_carrinho ; i++){
        if(codigo == carrinho[i].codigo && lista_produto[aumentar].qtd_estoque > 0){
            carrinho[i].qtd_estoque++;
            lista_produto[aumentar].qtd_estoque--;
        }else if(codigo == carrinho[i].codigo && lista_produto[aumentar].qtd_estoque == 0)
            puts("Nao ha mais estoque do produto");

}
}
void diminuirCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int diminuir = -1;
    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo == lista_produto[i].codigo){
            diminuir = i;
        }
    }
    if(diminuir == -1)
        puts("ERRO");
    for(int i = 0 ; i < qtd_carrinho ; i++){
        if(codigo == carrinho[i].codigo){
            carrinho[i].qtd_estoque--;
            lista_produto[diminuir].qtd_estoque++;
            if(carrinho[i].qtd_estoque == 0){
                carrinho[i] = carrinho[qtd_carrinho - 1];
                qtd_carrinho--;
            }
        }

        

}
}
void excluirProduto(Produto lista_produto[],Produto carrinho[],int &qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int qtd = qtd_produto;
    for(int i = 0 ; i < qtd_carrinho ;i++){
        if(codigo == carrinho[i].codigo){
            puts("Produto está no carrinho");
            return;
        }    
    }        
    int contador = 0;
    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo != lista_produto[i].codigo)
            contador++;
    }
    if(contador == qtd_produto){
        puts("Produto não Incluido na Lista");
        return;
    }
    for(int i = 0 ; i < qtd_produto; i++){
        if(codigo == lista_produto[i].codigo && lista_produto[i].qtd_estoque == 0 ){
            lista_produto[i] = lista_produto[qtd_produto - 1];
            qtd_produto--;
        }
    }
    if(qtd == qtd_produto)
        puts("Ainda Há Estoque do Produto");
}
void excluirCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    int codigo = le_inteiro("Codigo: ", "O Codigo deve estar entre  1 a 999", 1, 999);
    int excluir = -1;
    for(int i = 0 ; i < qtd_produto ; i++){
        if(codigo == lista_produto[i].codigo)
            excluir = i;
    }

    for(int i = 0 ; i < qtd_carrinho ; i++){
        if(codigo == carrinho[i].codigo){
            lista_produto[excluir].qtd_estoque = lista_produto[excluir].qtd_estoque + carrinho[i].qtd_estoque;
            carrinho[i] = carrinho[qtd_carrinho - 1];
            qtd_carrinho--;
        }
    }


}
void ordenarPorDescricao(Produto carrinho[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (strcmp(carrinho[j].descricao, carrinho[j+1].descricao) > 0) {
                Produto temp = carrinho[j];
                carrinho[j] = carrinho[j+1];
                carrinho[j+1] = temp;
            }
        }
    }
}
void mostrarCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
    if(qtd_carrinho == 0){
        puts("Carrinho esta vazio");
        return;
    }
    float total = 0;
        for(int i = 0 ; i < qtd_carrinho ; i++){
            total+=(carrinho[i].preco_maximo * (carrinho[i].qtd_estoque)) * (1.0 - float(carrinho[i].desconto  / float(100)));
        }
        puts("---------------------------------------------------------------------------------------");
        puts("Codigo Descricao                               Categ.  Qtd     Preco Desconto     Valor");
        puts("---------------------------------------------------------------------------------------");
        for(int i = 0 ; i < qtd_carrinho ; i++){
        printf(" %03d   %-40s  %c     %-4d  %7.2f    %5.1f %9.2f\n",carrinho[i].codigo,
        carrinho[i].descricao,carrinho[i].categoria,carrinho[i].qtd_estoque,carrinho[i].preco_maximo,float(carrinho[i].desconto),(carrinho[i].preco_maximo * carrinho[i].qtd_estoque) * (1.0 - float(carrinho[i].desconto  / float(100))));
        }
        puts("---------------------------------------------------------------------------------------");
        printf("Total: %.2f\n",total);
        puts("---------------------------------------------------------------------------------------");
}
void deletarCarrinho(Produto lista_produto[],Produto carrinho[],int qtd_produto,int &qtd_carrinho){
while(qtd_carrinho > 0){    
 for(int j = 0 ; j < qtd_produto ; j++){   
    for(int i = 0 ; i < qtd_carrinho ;i++){
            if(carrinho[i].codigo == lista_produto[j].codigo){
            lista_produto[j].qtd_estoque = lista_produto[j].qtd_estoque + carrinho[i].qtd_estoque;
            carrinho[i] = carrinho[qtd_carrinho - 1];
            qtd_carrinho--;
        }
    }
}
}
}
bool CartaodeCredito(const char *numero) {
    int soma = 0;
    bool trocou = false;
    for (int i = strlen(numero) - 1; i >= 0; i--) {
        int n = numero[i] - '0';
        if (trocou) {
             n *= 2;
             if (n > 9) {
              n = (n % 10) + 1;
            }
        }
         soma += n;
         trocou = !trocou;
    }
    return (soma % 10 == 0);
}
void concluirCompra(Produto lista_produto[],Produto carrinho[],Pedido pedidos[],int qtd_produto,int &qtd_carrinho,int &qtd_pedidos){
    Data auxiliar;
    char numero[25]; 
    if(qtd_carrinho == 0){
        puts("Nao ha produtos no carrinho para serem comprados");
        return;
    }
    le_cpf("CPF: ", "CPF invalido",pedidos[qtd_pedidos].cpf);
    do{
        printf("Digite o Numero do Cartao de Credito: ");
        scanf("%s", numero);
        CartaodeCredito(numero);
        if(CartaodeCredito(numero)== false)
            puts("Numero de Cartao Invalido");
    }while(CartaodeCredito(numero) == false);
    pedidos[qtd_pedidos].nc= atoll(numero);
    getchar();
    int codigo_cartao = le_inteiro("Codigo de Seguranca: ", "O Codigo de Seguranca deve ser entre 111 e 999", 111, 999);
  for(int i = 0 ; i < qtd_carrinho ; i++)  
    pedidos[qtd_pedidos].produtos[i] = carrinho[i];
    pedidos[qtd_pedidos].numero = qtd_carrinho;
    
    data_hora_atual(pedidos[qtd_pedidos].dt.dia, pedidos[qtd_pedidos].dt.mes,pedidos[qtd_pedidos].dt.ano,pedidos[qtd_pedidos].dt.hora,pedidos[qtd_pedidos].dt.min,pedidos[qtd_pedidos].dt.seg);
    do{
    auxiliar=le_data("Data de Vencimento (MMAAAA): ", "Data invalida");
    }while(auxiliar.ano < pedidos[qtd_pedidos].dt.ano || auxiliar.ano == pedidos[qtd_pedidos].dt.ano && auxiliar.mes < pedidos[qtd_pedidos].dt.mes);
    qtd_pedidos++;
    while(qtd_carrinho > 0){    
 for(int j = 0 ; j < qtd_produto ; j++){   
    for(int i = 0 ; i < qtd_carrinho ;i++){
            if(carrinho[i].codigo == lista_produto[j].codigo){
            carrinho[i] = carrinho[qtd_carrinho - 1];
            qtd_carrinho--;
        }
    }
}
}
}
void ordenarPorCategoria(Produto produtos[], int n) {
    if(n == 0){
        puts("Não Há Produtos Cadastrados");
    }
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (produtos[j].categoria > produtos[j+1].categoria || 
                (produtos[j].categoria == produtos[j+1].categoria && 
                 strcmp(produtos[j].descricao, produtos[j+1].descricao)>0)) {
                Produto temp = produtos[j];
                produtos[j] = produtos[j+1];
                produtos[j+1] = temp;
            }
        }
    }
}
void ordenarPorPreco(Produto produtos[], int n) {
    if(n == 0){
        puts("Não há Produtos Cadastrados");
    }
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (produtos[j].preco_maximo > produtos[j+1].preco_maximo) {
                Produto temp = produtos[j];
                produtos[j] = produtos[j+1];
                produtos[j+1] = temp;
            }
        }
    }
}
void ordenarPorHorario(Pedido pedidos[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (pedidos[j].dt.ano < pedidos[j+1].dt.ano) {
                Pedido temp = pedidos[j];
                pedidos[j] = pedidos[j+1];
                pedidos[j+1] = temp;
            } else if (pedidos[j].dt.ano == pedidos[j+1].dt.ano) {
                if (pedidos[j].dt.mes < pedidos[j+1].dt.mes) {
                    Pedido temp = pedidos[j];
                    pedidos[j] = pedidos[j+1];
                    pedidos[j+1] = temp;
                } else if (pedidos[j].dt.mes == pedidos[j+1].dt.mes) {
                    if (pedidos[j].dt.dia < pedidos[j+1].dt.dia) {
                        Pedido temp = pedidos[j];
                        pedidos[j] = pedidos[j+1];
                        pedidos[j+1] = temp;
                    } else if (pedidos[j].dt.dia == pedidos[j+1].dt.dia) {
                        if (pedidos[j].dt.hora < pedidos[j+1].dt.hora) {
                            Pedido temp = pedidos[j];
                            pedidos[j] = pedidos[j+1];
                            pedidos[j+1] = temp;
                        } else if (pedidos[j].dt.hora == pedidos[j+1].dt.hora) {
                            if (pedidos[j].dt.min < pedidos[j+1].dt.min) {
                                Pedido temp = pedidos[j];
                                pedidos[j] = pedidos[j+1];
                                pedidos[j+1] = temp;
                            } else if (pedidos[j].dt.min == pedidos[j+1].dt.min) {
                                if (pedidos[j].dt.seg < pedidos[j+1].dt.seg) {
                                    Pedido temp = pedidos[j];
                                    pedidos[j] = pedidos[j+1];
                                    pedidos[j+1] = temp;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void printProdutos(Produto lista_produto[],int qtd_produto){
           if(qtd_produto == 0)
                return;
            puts("-----------------------------------------------------------------------------");
            puts("Codigo Descricao                               Categ.  Qtd     Preco Desconto");
            puts("-----------------------------------------------------------------------------");

            for(int i = 0 ; i < qtd_produto ; i++){
            printf(" %03d   %-40s  %c     %-4d  %7.2f    %5.1f\n",lista_produto[i].codigo,
            lista_produto[i].descricao,lista_produto[i].categoria,lista_produto[i].qtd_estoque,lista_produto[i].preco_maximo,float(lista_produto[i].desconto));
            }
            puts("-----------------------------------------------------------------------------");
}
void printPedidos(Pedido pedidos[], int qtd_pedidos){
        float total = 0;
        int ordem = qtd_pedidos;
        for(int i = 0 ; i < qtd_pedidos ; i++, ordem--){
                for(int j = 0 ; j < pedidos[i].numero ; j++){
                total +=(pedidos[i].produtos[j].preco_maximo * pedidos[i].produtos[j].qtd_estoque) * (1.0 - float(pedidos[i].produtos[j].desconto  / float(100)));
                }
                puts("---------------------------------------------------------------------------------------");
                puts("Numero Data        Hora        CPF       Cartao       Total");
                printf("%06d %02d/%02d/%02d %02d:%02d",ordem,pedidos[i].dt.dia, pedidos[i].dt.mes,pedidos[i].dt.ano,pedidos[i].dt.hora,pedidos[i].dt.min);
                printf("  %.3s.%.3s.%.3s-%.2s", pedidos[i].cpf, pedidos[i].cpf+3, pedidos[i].cpf+6, pedidos[i].cpf+9);
                printf("    %lld    %8.2f\n",pedidos[i].nc % 10000,total);
                puts("=======================================================================================");
                puts("Codigo Descricao                               Categ.  Qtd     Preco Desconto     Valor");
                puts("---------------------------------------------------------------------------------------");
                for(int j = 0 ; j < pedidos[i].numero ; j++){
                printf(" %03d   %-40s  %c     %-4d  %7.2f    %5.1f %9.2f\n",pedidos[i].produtos[j].codigo,pedidos[i].produtos[j].descricao,pedidos[i].produtos[j].categoria,pedidos[i].produtos[j].qtd_estoque,pedidos[i].produtos[j].preco_maximo,float(pedidos[i].produtos[j].desconto),(pedidos[i].produtos[j].preco_maximo * pedidos[i].produtos[j].qtd_estoque) * (1.0 - float(pedidos[i].produtos[j].desconto  / float(100))));
            }
            total = 0;   
            puts("=======================================================================================");   
            }
}     
void menuCarrinho(Produto lista_produto[],Produto carrinho[],Pedido pedidos[],int qtd_produto,int &qtd_carrinho,int &qtd_pedidos){
    int opcao;
while(true){
    ordenarPorDescricao(carrinho,qtd_carrinho);
    mostrarCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
    
    puts("================");
    puts("Menu do Carrinho");
    puts("================");
    puts("1-Incluir");
    puts("2-Excluir");
    puts("3-Aumentar");
    puts("4-Diminuir");
    puts("5-Esvaziar");
    puts("6-Comprar");
    puts("7-Voltar");
    opcao = le_inteiro("Opcao: ", "Escolha  entre  1 a 7", 1, 7);
    
    switch (opcao){
        case 1:
            incluirCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 2:
            excluirCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 3:
            aumentarCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 4:
            diminuirCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 5:
            deletarCarrinho(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 6:
            concluirCompra(lista_produto,carrinho,pedidos,qtd_produto,qtd_carrinho,qtd_pedidos);
            break;
        case 7:
            return;
            break;

    }

}
}
void menuProdutos(Produto lista_produto[],Produto carrinho[],int &qtd_produto,int &qtd_carrinho){
    int opcao;
while(true){
    puts("================");
    puts("Menu de Produtos");
    puts("================");
    puts("1-Incluir");
    puts("2-Excluir");
    puts("3-Alterar");
    puts("4-Consultar por Categoria");
    puts("5-Consultar por preco");
    puts("6-Voltar");
    opcao = le_inteiro("Opcao: ", "Escolha  entre  1 a 6", 1, 6);

    switch(opcao){

        case 1:
            incluirProduto(lista_produto,qtd_produto);
            break;
        case 2:
            excluirProduto(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 3:
            alterarProduto(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 4:
            ordenarPorCategoria(lista_produto,qtd_produto);
            printProdutos(lista_produto,qtd_produto);
            break;
        case 5:
            ordenarPorPreco(lista_produto,qtd_produto);
            printProdutos(lista_produto,qtd_produto);
            break;
        case 6:
            return;
            break;
    }
}
}
int MenuPrincipal(Produto lista_produto[],Produto carrinho[],Pedido pedidos[],int qtd_produto,int &qtd_carrinho,int &qtd_pedidos){

    int opcao;
while(true){   
    puts("=================================");
    puts("E-Commerce - Menu Principal (1.0)");
    puts("=================================");
    puts("1-Carrinho");
    puts("2-Produtos");
    puts("3-Pedidos");
    puts("4-Fim");
    opcao = le_inteiro("Opcao: ", "Escolhe entre 1 a 4", 1, 4);

    
    
    switch (opcao){
        case 1:
            menuCarrinho(lista_produto,carrinho,pedidos,qtd_produto,qtd_carrinho,qtd_pedidos);
            break;
        case 2:
            menuProdutos(lista_produto,carrinho,qtd_produto,qtd_carrinho);
            break;
        case 3:
            ordenarPorHorario(pedidos, qtd_pedidos);
            printPedidos(pedidos, qtd_pedidos);
            break;
        case 4:
            return 0;
            break;
    }
}    
}







//***************************************************************************************
// MAIN
//***************************************************************************************

int main()
{
    Produto lista_produto[100];
    int qtd_produto = 0;
    Produto carrinho[100];
    int qtd_carrinho = 0;
    Pedido pedidos[100];
    int qtd_pedidos = 0;
    
    MenuPrincipal(lista_produto,carrinho,pedidos,qtd_produto,qtd_carrinho,qtd_pedidos);
}   
    




