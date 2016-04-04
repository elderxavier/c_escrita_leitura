#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <process.h>
#include <ctype.h>
/*constantes*/
#define _ARQUIVO "pacientes.txt"

/*Estrutura de dados para paciente*/
struct {
    char *nome;
    char *sobrenome;
    char *sexo;
    char *idade;
    char *altura;
    char *peso;
} paciente;

/*snippet origem http://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c*/
char** str_split(char* a_str, const char a_delim) {
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;

    result = malloc(sizeof (char*) * count);

    if (result) {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == (count - 1));
        *(result + idx) = 0;
    }

    return result;
}

/*Inseri registros em arquivo de texto paciente*/
void inseir() {
    FILE * arquivo;
    int cadastro = 0;
    char nome[30];
    char sobrenome[50];
    char sexo[10];
    char idade[8];
    char altura[8];
    char peso[8];
    char *a = 's';
    while (a != 'n') {
        cadastro++;
        printf("\n");
        printf("Registro: %d\n", cadastro);
        arquivo = fopen(_ARQUIVO, "a");

        printf("Digite nome paciente: ");
        scanf("%s",&nome);
        fprintf(arquivo, "%s;", nome);

        printf("Digite sobrenome: ");
        scanf("%s",&sobrenome);
        fprintf(arquivo, "%s;", sobrenome);

        printf("Digite sexo: ");
        scanf("%s",&sexo);
        fprintf(arquivo, "%s;", sexo);

        printf("Digite idade: ");
        scanf("%s",&idade);
        fprintf(arquivo, "%s;", idade);


        printf("Digite altura: ");
        scanf("%s",&altura);
        fprintf(arquivo, "%s;", altura);


        printf("Digite peso: ");
        scanf("%s",&peso);
        fprintf(arquivo, "%s\n", peso);

        printf("Deseja inserir novo? [s/n]");
        scanf("%s", &a);
        fclose(arquivo);
    }
}

/*Lista todos os arquivos com saida formatada (segregada)*/
int listarTodos() {
    FILE * arquivo;
    arquivo = fopen(_ARQUIVO, "r");
    int cadastro = 0;
    while ((fscanf(arquivo, "%s %s %s %s %s %s\n", &paciente.nome, &paciente.sobrenome, &paciente.sexo, &paciente.idade, &paciente.altura, &paciente.peso)) != EOF) {
        cadastro++;
        printf("Cadastro: %d\n", cadastro);
        printf("Nome: %s\n", paciente.nome);
        printf("Sobrenome: %s\n", paciente.sobrenome);
        printf("Sexo: %s\n", paciente.sexo);
        printf("Idade: %s\n", paciente.idade);
        printf("Altura: %s\n", paciente.altura);
        printf("Peso: %s\n\n", paciente.peso);
    }

    fclose(arquivo);
    return (0);
}

/*Localiza byte inical e final de ocorrencia passada por "char *busca"*/
int * buscar(char *busca) {
    FILE * arquivo;
    arquivo = fopen(_ARQUIVO, "rb");
    int cadastro = 0;
    char bf[255];
    int ini = -1;
    int len = -1;
    int ret[1];
    while ((fgets(bf, sizeof (bf), arquivo)) != NULL) {
        char *src = strstr(bf, busca);

        if (src) {
            int cont = 0;
            while (cont < sizeof (bf) && bf[cont] != '\n') {
                cont++;
            }
            len = ftell(arquivo) - 1;
            ini = len - (cont);
        }
    }
    fclose(arquivo);
    ret[0] = ini;
    ret[1] = len;
    return (ret);
}

/*altera linha do arquivo*/
int alterar(int inicio, int len, char nome[30], char sobrenome[50], char sexo[10], char idade[8], char altura[8], char peso[8]) {
    char linha[115] = "";
    strcat(linha, nome);
    strcat(linha, ";");
    strcat(linha, sobrenome);
    strcat(linha, ";");
    strcat(linha, sexo);
    strcat(linha, ";");
    strcat(linha, idade);
    strcat(linha, ";");
    strcat(linha, altura);
    strcat(linha, ";");
    strcat(linha, peso);

    FILE* arquivo = fopen(_ARQUIVO, "r+");
    char bf[255];
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao tentar abrir arquivo.txt.");
        return 1;
    }
    fseek(arquivo, 0, SEEK_SET);
    fgets(bf, len, arquivo);
    FILE* temp = fopen("temp.txt", "w+");
    fputs(bf, temp);
    fseek(arquivo, inicio, SEEK_SET);
    fputs(linha, temp);
    fclose(temp);

    fseek(arquivo, (len + 1), SEEK_SET);
    temp = fopen("temp.txt", "a");
    while ((fgets(bf, sizeof (bf), arquivo)) != NULL) {
        fputs(bf, temp);
    }
    fclose(temp);
    fclose(arquivo);

    temp = fopen("temp.txt", "r+");
    arquivo = fopen(_ARQUIVO, "w+");
    fseek(arquivo, 0, SEEK_SET);
    fseek(temp, 0, SEEK_SET);
    while ((fgets(bf, sizeof (bf), temp)) != NULL) {
        fputs(bf, arquivo);
    }
    fclose(temp);
    fclose(arquivo);
    return 0;
}
/*exluir linha do arquivo*/
int excluir(int inicio, int len) {
    char linha[115] = "";

    FILE* arquivo = fopen(_ARQUIVO, "r+");
    char bf[255];
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao tentar abrir arquivo.txt.");
        return 1;
    }
    fseek(arquivo, 0, SEEK_SET);
    fgets(bf, len, arquivo);
    FILE* temp = fopen("temp.txt", "w+");
    fputs(bf, temp);
    fseek(arquivo, inicio, SEEK_SET);
    fputs(linha, temp);
    fclose(temp);

    fseek(arquivo, (len + 1), SEEK_SET);
    temp = fopen("temp.txt", "a");
    while ((fgets(bf, sizeof (bf), arquivo)) != NULL) {
        fputs(bf, temp);
    }
    fclose(temp);
    fclose(arquivo);

    temp = fopen("temp.txt", "r+");
    arquivo = fopen(_ARQUIVO, "w+");
    fseek(arquivo, 0, SEEK_SET);
    fseek(temp, 0, SEEK_SET);
    while ((fgets(bf, sizeof (bf), temp)) != NULL) {
        fputs(bf, arquivo);
    }
    fclose(temp);
    fclose(arquivo);
    return 0;
}

int listarUnico(int inicio, int len) {
    FILE* arquivo = fopen(_ARQUIVO, "r+");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao tentar abrir arquivo.txt.");
        return 1;
    }
    char bf[255];
    fseek(arquivo, inicio, SEEK_SET);
    fgets(bf, sizeof (bf), arquivo);
    fclose(arquivo);
    char** tokens;
    if (sizeof (bf) > 1) {
        tokens = str_split(bf, ';');
        if (tokens) {
            int i;
            for (i = 0; *(tokens + i); i++) {
                switch (i) {
                    case 0:
                        paciente.nome = *(tokens + i);
                        break;
                    case 1:
                        paciente.sobrenome = *(tokens + i);
                        break;
                    case 2:
                        paciente.sexo = *(tokens + i);
                        break;
                    case 3:
                        paciente.idade = *(tokens + i);
                        break;
                    case 4:
                        paciente.altura = *(tokens + i);
                        break;
                    case 5:
                        paciente.peso = *(tokens + i);
                        break;
                }
                //free(*(tokens + i));
            }
            printf("\n");

        }
    }
    submenu2_2(paciente.nome, paciente.sobrenome, paciente.sexo, paciente.idade, paciente.altura, paciente.peso, inicio, len);
    free(tokens);
    return (0);
}

int listarTodos2() {
    FILE * arquivo;
    arquivo = fopen(_ARQUIVO, "r");
    int cadastro = 0;
    char bf[255];
    char** tokens;
    //while ((fscanf(arquivo, "%s %s %s %s %s %s\n", &paciente.nome, &paciente.sobrenome, &paciente.sexo, &paciente.idade, &paciente.altura, &paciente.peso)) != EOF) {
    while ((fgets(bf, sizeof (bf), arquivo)) != NULL) {
        if (sizeof (bf) > 1) {
            cadastro++;
            printf("Registro:%d\n", cadastro);
            tokens = str_split(bf, ';');
            if (tokens) {
                int i;
                for (i = 0; *(tokens + i); i++) {
                    switch (i) {
                        case 0:
                            printf("Nome:%s\n", *(tokens + i));
                            break;
                        case 1:
                            printf("Sobrenome:%s\n", *(tokens + i));
                            break;
                        case 2:
                            printf("Sexo:%s\n", *(tokens + i));
                            break;

                        case 3:
                            printf("Idade:%s\n", *(tokens + i));
                            break;
                        case 4:
                            printf("Altura:%s\n", *(tokens + i));
                            break;
                        case 5:
                            printf("Peso:%s", *(tokens + i));
                            break;
                    }


                    free(*(tokens + i));
                }
                printf("\n");
                free(tokens);
            }
        }
    }

    fclose(arquivo);

    return 0;
}

int menu() {
    system("cls");
    printf("*************************************************\n");
    printf("*\t\tRegistro de Pacientes\t\t*\n");
    printf("*************************************************\n");
    printf("\n1 - Adicionar novo paciente");
    printf("\n2 - Alterar registro de paciente");
    printf("\n3 - Listar todos os registros");
    printf("\n4 - Sair");
    printf("\n\n");
    printf("Entre com uma opcao(1,2,3 ou 4):");

    int option;
    scanf("%d", &option);
    switch (option) {
        case 1:
            submenu1();
            break;
        case 2:
            submenu2();
            break;
        case 3:
            submenu3();
            break;
        case 4:
            system("exit");
            break;
    }

    printf("\n\n\n");
    return 0;
}

int submenu1() {
    system("cls");
    printf("*************************************************\n");
    printf("*\t\tInserir Registro(s)\t\t*\n");
    printf("*************************************************\n");
    inseir();
    system("pause");
    menu();
    return 0;
}

int submenu2() {
    system("cls");
    char nome[30] = "";
    char sobrenome[50] = "";


    printf("Digite o nome do paciente:");
    scanf("%s", &nome);
    printf("Digite o sobrenome do paciente:");
    scanf("%s", &sobrenome);

    strcat(nome, ";");
    strcat(nome, sobrenome);
    printf("%s\n", nome);
    int ini = buscar(nome)[0];
    int len = buscar(nome)[1];
    if (ini >= 0) {
        system("cls");
        listarUnico(ini, len);
    } else {
        printf("registro nao encontrado!\n\n");
    }

    system("pause");
    menu();
    return (0);
}

int submenu2_2(char *nome, char *sobrenome, char *sexo, char *idade, char *altura, char *peso, int inicio, int len) {
    system("cls");
    printf("*************************************************\n");
    printf("*\t\tAlterar Registro\t\t*\n");
    printf("*************************************************\n");
    printf("Nome:%s\n", nome);
    printf("Sobrenome:%s\n", sobrenome);
    printf("Sexo:%s\n", sexo);
    printf("Idade:%s\n", idade);
    printf("Altura:%s\n", altura);
    printf("Peso:%s", peso);


    printf("\n");
    printf("1 - Aterar Nome\n");
    printf("2 - Aterar Sobreome\n");
    printf("3 - Aterar Sexo\n");
    printf("4 - Aterar Idade\n");
    printf("5 - Aterar Altura\n");
    printf("6 - Aterar Peso\n");
    printf("7 - Voltar ao menu\n");
    printf("8 - Excluir registro\n");

    printf("Entre com uma opcao(1,2,3,4,5,6,7 ou 8):");
    char alterarado[50];
    int option;
    char sn[1] = "n";
    scanf("%d", &option);
    switch (option) {
        case 1:
            printf("Digite o novo nome:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                nome = alterarado;
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                //submenu2_2(nome, sobrenome, sexo, idade, altura, peso, inicio, len);
                listarUnico(inicio, len);
            }
            break;
        case 2:
            printf("Digite o novo sobrenome:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                sobrenome = alterarado;
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                listarUnico(inicio, len);
            }
            break;
        case 3:
            printf("Digite o novo sexo:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                sexo = alterarado;
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                listarUnico(inicio, len);
            }
            break;
        case 4:
            printf("Digite a nova idade:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                idade = alterarado;
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                listarUnico(inicio, len);
            }
            break;
        case 5:
            printf("Digite a nova altura:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                altura = alterarado;
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                listarUnico(inicio, len);
            }
            break;
        case 6:
            printf("Digite o novo peso:");
            scanf("%s", &alterarado);
            printf("\nConfirmar alteracao?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                peso = alterarado;
                strcat(peso, "\n");
                alterar(inicio, len, nome, sobrenome, sexo, idade, altura, peso);
                printf("\nRegistro alterado com sucesso!\n");
                system("pause");
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                listarUnico(inicio, len);
            }
            break;
        case 7:
            menu();
            break;
        case 8:
            printf("\nConfirmar exclusao do registro?[s/n]");
            scanf("%s", &sn);
            if (strcmp(sn, "s\n") == 0 || strcmp(sn, "s") == 0) {
                char src[80] = "";
                strcat(src, nome);
                strcat(src, ";");
                strcat(src, sobrenome);
                int ini = buscar(src)[0];
                int len = buscar(src)[1];
                excluir(ini, len);
                printf("\nRegistro excluido com sucesso!\n");
                system("pause");
            }
            break;
    }
    printf("\n\n");
    menu();
    return (0);
}

int submenu3() {
    system("cls");
    listarTodos2();
    system("pause");
    menu();
    return (0);
}

int main(int argc, char *argv[]) {
    menu();    
    
    system("pause");
    return (0);
}


