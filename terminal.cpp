#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

using namespace std;

void criarThreadPwd ();
void criarThreadCd (string comando);
void criarProcessoExecvp (string comando);

void *pwd ( void *ptr );
void *cd ( void *ptr );
void criarExecvp (string comando);

void receberComandos (void *ptrInput);
void *input ( void *ptr );

int main()
{
    bool continuar = true;

    while (continuar == true)
    {
        string comando;
        string *ptrInput = &comando;

        receberComandos(ptrInput);

        if (comando == "exit")
        {
            continuar = false;
        }

        else if (comando == "pwd")
        {
            criarThreadPwd();
        }

        else if (comando.substr(0,2) == "cd")
        {
            criarThreadCd(comando);
        }

        else if (comando.substr(0,4) == "exec")
        {
            comando = comando.substr(5, ( comando.length() - 5 ));
            
            if (comando == "exit")
            {
                _exit(0);
            }

            else if (comando == "pwd")
            {
                pwd (NULL);

                return 0;
            }

            else if (comando.substr(0,2) == "cd")
            {
                cd ((void*)ptrInput);

                return 0;
            }

            else
            {
                criarExecvp(comando);

                return 0;
            }
        }

        else
        {
            criarProcessoExecvp(comando);
        }

    }

    //system call para exit
    _exit(0);

}

void *pwd ( void *ptr )
{
    char cwd[300];
    
    //chama system call getcwd() para descobrir diretório atual e avisa erro se retornar NULL
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        cout << "Erro." << endl;

    else
        cout << cwd << endl;
}

void *cd ( void *ptr )
{
    string *ptrString = (string*)ptr;

    string comando = *ptrString;

    //separa caminho da string do comando
    string caminho = comando.substr(3, ( comando.length() - 3 ));

    //transforma caminho em um array de chars
    char charCaminho[caminho.length()];
    strcpy(charCaminho, caminho.c_str());

    int verificar;

    verificar = chdir(charCaminho);

    if (verificar != 0)
        cout << "Erro." << endl;
}

void *input ( void *ptr )
{
    string *ptrString = (string*)ptr;

    getline(cin, *ptrString);
}

void receberComandos (void *ptrInput)
{
    pthread_t threadInput;
    int iretInput;

    iretInput = pthread_create(&threadInput, NULL, input, (void*) ptrInput);

    if (iretInput != 0)
        cout << "Erro" << endl;

    pthread_join( threadInput, NULL);
}

void criarThreadPwd ()
{
    pthread_t thread1;
    int iret1;

    iret1 = pthread_create(&thread1, NULL, pwd, NULL);

    if (iret1 != 0)
        cout << "Erro" << endl;

    pthread_join( thread1, NULL);
}

void criarThreadCd (string comando)
{
    string *ptr = &comando;

    pthread_t thread1;
    int iret1;

    iret1 = pthread_create(&thread1, NULL, cd, (void*) ptr);

    if (iret1 != 0)
        cout << "Erro" << endl;

    pthread_join( thread1, NULL);
}

void criarProcessoExecvp (string comando)
{
    char linha[100];

    char *comando2[5];

    int i,j,k;

    for(i=0;i<100;i++) {
        linha[i]='\0';
    }

    for(i=0;i<5;i++) {
        comando2[i]= NULL;
    }

    comando2[0] = linha;

    for(i=0; i != comando.length();i++) {
        linha[i]=comando[i];
    }

    linha[i]='\0';
    
    k = 1;

    for(j=0;j<i+1;j++) {
        if(linha[j]==' ') {
            linha[j]='\0';
            comando2[k]=&linha[j+1];
            k = k+1;
        }
    }

    if(fork()==0)
    {
        execvp(comando2[0],comando2);
        printf("Erro\n");
    }
}

void criarExecvp (string comando)
{
    char linha[100];

    char *comando2[5];

    int i,j,k;

    for(i=0;i<100;i++) {
        linha[i]='\0';
    }

    for(i=0;i<5;i++) {
        comando2[i]= NULL;
    }

    comando2[0] = linha;

    for(i=0; i != comando.length();i++) {
        linha[i]=comando[i];
    }

    linha[i]='\0';
    
    k = 1;

    for(j=0;j<i+1;j++) {
        if(linha[j]==' ') {
            linha[j]='\0';
            comando2[k]=&linha[j+1];
            k = k+1;
        }
    }

    execvp(comando2[0],comando2);
}
