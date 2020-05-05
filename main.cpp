
/**
 * Programa que armazena um registro de tamanho fixo em um arquivo binário.
 *
 * Realiza as operações de inclusão, inclusão com verificação, exclusão(lógica), alteração, pesquisa, posição e listagem dos registros.
 *
 */

#include <iostream> //cout, endl
#include <cstdlib> //EXIT_SUCCESS
#include <fstream>
#include <cstring>

using namespace std;

//Registro a ser armazenado no arquivo

struct Cliente {
    int codigo;
    char nome[85];
    char cpf[11];
};

//Nome do arquivo binário
const string NOMEARQUIVO = "CLIENTE.DAT";

/**
 * Realiza o preenchimento de um registro do tipo cliente.
 * @param mensagem Mensagem inícia antes da leitura.
 * @return Um registro preenchido.
 */
Cliente leitura(string mensagem) {
    //Mostra a mensagem se ela for diferente de vazio
    if (mensagem != "") {
        cout << mensagem << endl;
    }
    //Declara o cliente a ser preenchido
    Cliente cliente;
    //Preenche o cliente com os dados lidos
    cout << "Digite o Código:";
    cin >> cliente.codigo;
    //Limpa o fluxo
    fflush(stdin); //ou cin.ignore();
    cout << "Digite o Nome:";
    gets(cliente.nome);
    cout << "Digite o CPF:";
    gets(cliente.cpf);

    //Retorna o cliente preenchido
    return cliente;
}

/**
 * Retorna o tamanho do registro de cliente.
 *
 * @return Um inteiro com o tamanho do registro do cliente.
 */
int getTamanhoRegistro() {
    // 4 código + 85 nome + 11 cpf
    // código int = 4 bytes
    // nome String = 85 caracteres = 1 caracter ocupada 1 byte = 85 bytes
    // cpf String = 11 caracteres = 1 caracter ocupada 1 byte = 11 bytes        
    //Total 100 bytes
    return (4 + 85 + 11);
}

/**
 * Retorna a posição de um registro no arquivo.
 *
 * @param chave Valor a ser procurado.
 *
 * @return Um valor com a posição do registro que tem a chave. Se não encontrar posição retorna -1.
 */
int posicaoRegistro(int chave) {
    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Guarda a posição de parada da procura    	
        int posicao = -1;
        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Utilizado para interroper o laço da leitura do arquivo
        bool achei = false;
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo e não achei
        while ((arquivo.tellg() < tamanhoArquivo) && (achei == false)) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Encerra a procura se encontrei a chave
            if (registro.codigo == chave) {
                achei = true;
            }
            //Incrementa o contador de posições
            posicao = posicao + 1;
        }
        //Fecha o arquivo
        arquivo.close();
        //Verifica se achou
        if (achei == true) {
            //Retorna a posição encontrada
            return posicao;
        } else {
            //Não conseguiu encontrar o registro
            return -1;
        }
    }
    return -1;
}

/**
 * Inclui um registro no fim do arquivo.
 *
 * @param registro Registro de cliente a ser adicionado no fim do arquivo.
 * @return Retorna verdadeiro ou falso se conseguiu realizar a inclusão.
 */
bool inserirFimArquivo(Cliente registro) {
    //Declara o arquivo
    ofstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para saída(escrita) de forma binária no fim.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::out | ios::binary | ios::app);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Escreve o registro no arquivo
        arquivo.write((char *) &registro, sizeof (Cliente));
        //Fecha o arquivo
        arquivo.close();
        //Conseguiu realizar a inclusão
        return true;
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
        return false;
    }
}

/**
 * Atualiza um registro no arquivo com base na chave.
 *
 * @param chave Chave do registro a ser atualizado.
 * @param cliente Um cliente com os novos dados.
 *
 * @return Retorna verdadeiro ou falso se conseguiu atualizar o registro.
 */
bool atualizarArquivo(int chave, Cliente cliente) {

    //Pega a posição do registro com a chave a ser alterada
    int posicao = posicaoRegistro(chave);

    //Se encontrou a chave
    if (posicao != -1) {
        //Declara o arquivo
        fstream arquivo;
        //Associa o arquivo a um nome e abre o arquivo para saída(escrita) e entrada(leitura) de forma binária.    
        arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::out | ios::binary);
        //Verifica se o arquivo está aberto
        if (arquivo.is_open()) {
            //Atribui o novo registro a ser alterado
            Cliente registro = cliente;
            //Posiciona no registro a ser alterado
            arquivo.seekg(posicao * getTamanhoRegistro());
            //Escreve o registro alterado no arquivo
            arquivo.write((char *) &registro, sizeof (Cliente));
            //Fecha o arquivo
            arquivo.close();
            //Retorna true pois conseguiu alterar o registro
            return true;
        } else {
            cout << "Arquivo não pode ser aberto" << endl;
            //Retorna false pois não conseguiu alterar o registro
            return false;
        }
    } else {
        //Retorna false pois não conseguiu alterar o registro
        return false;
    }
    return false;
}

/**
 * Exclui lógicamente um registro do arquivo.
 *
 * Atribui -1 para o campo chave do registro.
 *
 * @param chave Chave do registro a ser excluído.
 *
 * @return Verdadeiro ou falso se conseguiu excluir logicamente o registro.
 */
bool excluirLogico(int chave) {
    //Declara o arquivo
    fstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para saída(escrita) e entrada(leitura) de forma binária.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::out | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Guarda a posição da parada da procura    	
        int posicao = -1;
        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Utilizado para interroper o laço da leitura do arquivo
        bool achei = false;
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo e não achei
        while ((arquivo.tellg() < tamanhoArquivo) && (achei == false)) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Encerra a procura se encontrei a chave
            if (registro.codigo == chave) {
                achei = true;
            }
            //Incrementa o contador de posições
            posicao = posicao + 1;
        }
        //Se encontrou o registro realiza a exclusão
        if (achei == true) {
            //Altera o registro a ser excluído
            registro.codigo = -1;
            //Posiciona no registro a ser alterado
            arquivo.seekg(posicao * getTamanhoRegistro());
            //Escreve o registro alterado no arquivo
            arquivo.write((char *) &registro, sizeof (Cliente));
            //Fecha o arquivo
            arquivo.close();
            return true;
        } else {
            //Não conseguiu encontrar o registro
            return false;
        }
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
        return false;
    }
}

/**
 * Excluí fisicamente um registro do arquivo.
 * 
 * Cria um arquivo temporário para receber os registros menos o que deve ser excluído.
 * Apaga o arquivo principal e renomeia o arquivo temporário para principal.
 *
 * @param chave Valor da chave do registro a ser excluído.
 * @return Verdadeiro ou falso se conseguiu excluir fisicamente o registro.
 */
bool excluirFisico(int chave) {

    //Copia os dados do arquivo principal para o arquivo temporário

    //Retorna o nome do arquivo até o ponto
    string NOMEARQUIVOTEMP = NOMEARQUIVO.substr(0, NOMEARQUIVO.find("."));
    //Adiciona a extensão tmp ao nome do arquivo temporário
    NOMEARQUIVOTEMP = NOMEARQUIVOTEMP + ".TMP";

    //Declara o arquivo temporario
    ofstream arquivoTemp;
    //Associa o arquivo temporário a um nome e abre o arquivo para saída(escrita) de forma binária no fim.    
    arquivoTemp.open(NOMEARQUIVOTEMP.c_str(), ios::out | ios::binary);
    //Verifica se o arquivo temporário está aberto
    if (arquivoTemp.is_open()) {
        //Declara o arquivo
        ifstream arquivo;
        //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.
        arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
        //Verifica se o arquivo está aberto
        if (arquivo.is_open()) {
            //Declara um registro para armazenar os dados lido do arquivo.
            Cliente registro;
            //Posiciona no fim do arquivo
            arquivo.seekg(0, arquivo.end);
            //Retorna o tamanho do arquivo	
            int tamanhoArquivo = arquivo.tellg();
            //posiciona no inicio do arquivo
            arquivo.seekg(0, arquivo.beg);
            //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo
            while (arquivo.tellg() < tamanhoArquivo) {
                //Realiza a leitura de um registro do arquivo
                arquivo.read((char *) &registro, sizeof (Cliente));
                //Utiliza somente valores chave diferente de -1
                if (registro.codigo != -1) {
                    //Copia todos os registros menos a chave
                    if (registro.codigo != chave) {
                        //Posiciona o arquivo temporário no final                                
                        arquivoTemp.seekp(0, arquivoTemp.end);
                        //Escreve o registro no arquivo temporário
                        arquivoTemp.write((char *) &registro, sizeof (Cliente));
                    }
                }
            }
            //Fecha o arquivo
            arquivo.close();
        } else {
            cout << "Arquivo não pode ser aberto" << endl;
            return false;
        }
        //Fecha o arquivo temporário
        arquivoTemp.close();
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
        return false;
    }

    //Copia os dados do arquivo temporário para o arquivo principal

    //Declara o arquivo
    ofstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para saída(escrita) de forma binária no fim.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::out | ios::binary);
    //Verifica se o arquivo temporário está aberto
    if (arquivo.is_open()) {
        //Declara o arquivo temporário
        ifstream arquivoTemp;
        //Associa o arquivo a um nome e abre o arquivo temporário para entrada(leitura) de forma binária.
        arquivoTemp.open(NOMEARQUIVOTEMP.c_str(), ios::in | ios::binary);
        //Verifica se o arquivo temporário está aberto
        if (arquivoTemp.is_open()) {
            //Declara um registro para armazenar os dados lido do arquivo temporário.
            Cliente registro;
            //Posiciona no fim do arquivo temporário
            arquivoTemp.seekg(0, arquivoTemp.end);
            //Retorna o tamanho do arquivo temporário
            int tamanhoArquivo = arquivoTemp.tellg();
            //posiciona no inicio do arquivo temporário
            arquivoTemp.seekg(0, arquivoTemp.beg);
            //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo temporário
            while (arquivoTemp.tellg() < tamanhoArquivo) {
                //Realiza a leitura de um registro do arquivo
                arquivoTemp.read((char *) &registro, sizeof (Cliente));
                //Posiciona o arquivo no final                                
                arquivo.seekp(0, arquivo.end);
                //Escreve o registro no arquivo temporário
                arquivo.write((char *) &registro, sizeof (Cliente));
            }
            //Fecha o arquivo temporário
            arquivoTemp.close();

            //Apaga o arquivo temporário
            remove(NOMEARQUIVOTEMP.c_str());
        } else {
            cout << "Arquivo não pode ser aberto" << endl;
        }
        //Fecha o arquivo
        arquivo.close();

        //Terminou a cópia
        return true;

    } else {
        cout << "Arquivo não pode ser aberto" << endl;
        return false;
    }

    return false;
}

/**
 * Pesquisa uma chave no arquivo retornando o registro.
 *
 * @param chave Valor chave a se pesquisado no arquivo.
 *
 * @return Retorna o registro encontrado no arquivo.
 */
Cliente pesquisarRegistro(int chave) {
    //Registro de retorno definido como -1
    Cliente retorno;
    retorno.codigo = -1;

    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {

        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Utilizado para interroper o laço da leitura do arquivo
        bool achei = false;
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo e não achei
        while ((arquivo.tellg() < tamanhoArquivo) && (achei == false)) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Encerra a procura se encontrei a chave
            if (registro.codigo == chave) {
                achei = true;
            }
        }
        //Fecha o arquivo
        arquivo.close();
        //Verifica se achou
        if (achei == true) {
            //retorna o registro encontrado
            return registro;
        } else {
            //retorna o registro de não encontrado
            return retorno;
        }
    }
    return retorno;
}

/**
 * Pesquisa um nome no arquivo retornando o registro.
 *
 * @param chave Valor a ser pesquisado no arquivo.
 *
 * @return Retorna o registro encontrado no arquivo.
 */
Cliente pesquisarRegistroNome(char* chave) {
    //Registro de retorno definido como -1
    Cliente retorno;
    retorno.codigo = -1;

    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.    
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {

        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Utilizado para interroper o laço da leitura do arquivo
        bool achei = false;
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo e não achei
        while ((arquivo.tellg() < tamanhoArquivo) && (achei == false)) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Encerra a procura se encontrei a chave
            if (strcmp(registro.nome, chave) == 0) {
                achei = true;
            }
        }
        //Fecha o arquivo
        arquivo.close();
        //Verifica se achou
        if (achei == true) {
            //retorna o registro encontrado
            return registro;
        } else {
            //retorna o registro de não encontrado
            return retorno;
        }
    }
    return retorno;
}

/**
 * Realiza o retorno dos dados não excluídos do arquivo.
 *
 * Mostra os dados do arquivo onde o campo chave é diferente de -1.
 *  
 */
void listarLogico() {
    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo
        while (arquivo.tellg() < tamanhoArquivo) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Mostra somente valores chave diferente de -1
            if (registro.codigo != -1) {
                //Mostra os dados do registro                    
                cout << registro.codigo << " -  " << registro.nome << " - " << registro.cpf << endl;
            }
        }
        arquivo.close();
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
    }
}

/**
 * Realiza a exibição dos dados do arquivo.
 *
 * Mostra todos os dados do arquivo inclusive os excluídos.
 *
 */
void listarFisico() {
    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo
        while (arquivo.tellg() < tamanhoArquivo) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Mostra os dados do registro                    
            cout << registro.codigo << " -  " << registro.nome << " - " << registro.cpf << endl;
        }
        arquivo.close();
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
    }
}

/**
 * Retorna a quantidade de registros.
 *
 * @return Um número inteiro com a quantidade de registros.
 */
int getQuantidadeRegistro() {
    int contador = 0;
    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Declara um registro para armazenar os dados lido do arquivo.
        Cliente registro;
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //posiciona no inicio do arquivo
        arquivo.seekg(0, arquivo.beg);
        //Enquanto o ponteiro de leitura for menor que o tamanho do arquivo
        while (arquivo.tellg() < tamanhoArquivo) {
            //Realiza a leitura de um registro do arquivo
            arquivo.read((char *) &registro, sizeof (Cliente));
            //Somente para os registros não apagados
            if (registro.codigo != -1) {
                //Incrementa o contador de registro.
                contador = contador + 1;
            }
        }
        arquivo.close();
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
    }
    return contador;
}

/**
 * Recupera as informações do arquivo.
 *     
 */
void informacoes() {
    //Declara o arquivo
    ifstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para entrada(leitura) de forma binária.
    arquivo.open(NOMEARQUIVO.c_str(), ios::in | ios::binary);
    //Verifica se o arquivo está aberto
    if (arquivo.is_open()) {
        //Posiciona no fim do arquivo
        arquivo.seekg(0, arquivo.end);
        //Retorna o tamanho do arquivo	
        int tamanhoArquivo = arquivo.tellg();
        //Mostra as informações do arquivo
        cout << "Tamanho do Arquivo : " << tamanhoArquivo << " Kb " << endl
                << "Número de Registros : " << getQuantidadeRegistro() << endl
                << "Cada regitro possui : " << getTamanhoRegistro() << " Kb " << endl;
        arquivo.close();
    } else {
        cout << "Arquivo não pode ser aberto" << endl;
    }
}

/**
 * Apaga os registro do arquivo.
 *
 * @return Se conseguiu esvaziar o arquivo.
 */
bool zeraArquivo() {
    //Declara o arquivo
    ofstream arquivo;
    //Associa o arquivo a um nome e abre o arquivo para saída(escrita) de forma binária no fim.
    arquivo.open(NOMEARQUIVO.c_str(), ios::out | ios::binary);

    if (arquivo.is_open() == true) {
        //Fecha o arquivo
        arquivo.close();
        return true;
    } else {
        return false;
    }

}

/**
 * Programa principal
 */
int main(int argc, char *argv[]) {

    //Recebe a opção do menu
    int opcao = -1;
    while (opcao != 99) {
        cout << "\n\t### Arquivo Binário Sequencial ###" << endl
                << " 1 - Incluir " << endl
                << " 2 - Incluir com verificação " << endl
                << " 3 - Atualizar " << endl
                << " 4 - Excluir Lógico " << endl
                << " 5 - Excluir Físico " << endl
                << " 6 - Pesquisar Chave " << endl
                << " 7 - Pesquisar Posição " << endl
                << " 8 - Pesquisar Nome " << endl
                << " 9 - Listar Lógico " << endl
                << "10 - Listar Físico " << endl
                << "11 - Informações " << endl
                << "12 - Zera Arquivo " << endl
                << "99 - Sair" << endl
                << "Digite uma Opção: ";
        cin >> opcao;
        cout << endl;
        switch (opcao) {
            case 1:
            {
                //Chama o método leitura para retornar um cliente instanciado e preenchido
                Cliente cliente = leitura("Cadastro de cliente");

                if (inserirFimArquivo(cliente) == true) {
                    cout << "Registro inserido com sucesso." << endl;
                } else {
                    cout << "Registro não foi inserido." << endl;
                }
                break;
            }
            case 2:
            {
                //Chama o método leitura para retornar um cliente instanciado e preenchido
                Cliente cliente = leitura("Cadastro de cliente com verificação do código");

                // Procura a posição do registro com o código no arquivo
                int posicao = posicaoRegistro(cliente.codigo);
                //Enquanto não for localizado(posição != -1) e o usuário desejar proseguir
                while (posicao != -1 && cliente.codigo != -2) {
                    cout << "Este código já existe digite um novo código para o cliente ou -2 para sair:";
                    cin >> cliente.codigo;
                    // Procura a posição do registro com o código no arquivo
                    posicao = posicaoRegistro(cliente.codigo);
                }
                  //Se código diferente de -2 realiza a a inclusão
                if (cliente.codigo != -2) {
                    if (inserirFimArquivo(cliente) == true) {
                        cout << "Registro inserido com sucesso." << endl;
                    } else {
                        cout << "Registro não foi inserido." << endl;
                    }
                } else {
                    cout << "Registro não foi inserido." << endl;
                }
                break;
            }
            case 3:
            {
                //Pergunta qual o código a ser atualizado
                int codigoAtualizar;
                cout << " Digite o código a ser atualizado: ";
                cin >> codigoAtualizar;
                //Declara um novo cliente com os novos dados
                Cliente cliente = leitura("Digite os dados do novo cliente");

                //Atualiza o arquivo cliente com os dados para o código especificado
                if (atualizarArquivo(codigoAtualizar, cliente) == true) {
                    cout << "Registro " << codigoAtualizar << " atualizado com sucesso." << endl;
                } else {
                    cout << "Registro " + codigoAtualizar << " não foi atualizado." << endl;
                }
                break;
            }
            case 4:
            {
                //Pergunta qual o código a ser excluído logicamente
                int codigoExcluir;
                cout << " Digite o codigo a ser excluído: ";
                cin >> codigoExcluir;
                //Excluí o registro com o código especificado
                if (excluirLogico(codigoExcluir) == true) {
                    cout << "Registro " << codigoExcluir << " excluído com sucesso." << endl;
                } else {
                    cout << "Registro " + codigoExcluir << " não foi excluído." << endl;
                }
                break;
            }
            case 5:
            {
                //Pergunta qual o código a ser excluído Fisicamente
                int codigoExcluir;
                cout << " Digite o codigo a ser excluído: ";
                cin >> codigoExcluir;
                //Excluí o registro com o código especificado
                if (excluirFisico(codigoExcluir) == true) {
                    cout << "Registro " << codigoExcluir << " excluído com sucesso." << endl;
                } else {
                    cout << "Registro " + codigoExcluir << " não foi excluído." << endl;
                }
                break;
            }
            case 6:
            {
                //Pergunta qual a chave do cliente deve ser procurada no arquivo
                int chave;
                cout << "Digite o código a ser perquisado: ";
                cin >> chave;
                // Procura o registro do cliente com a chave no arquivo
                Cliente registro = pesquisarRegistro(chave);
                // Se registro.codigo != -1 encontrou o registro
                if (registro.codigo != -1) {
                    cout << "Achei o registro " << endl;
                    cout << registro.codigo << " -  " << registro.nome << " - " << registro.cpf << endl;
                } else {
                    cout << "Não Achei o registro com a chave " << chave << endl;
                }

                break;
            }
            case 7:
            {
                // Pergunta qual a chave do cliente deve ser procurada sua posição no arquivo
                int chave;
                cout << "Digite o código a ser procurado a posição:";
                cin >> chave;
                // Procura a posição do registro com chave no arquivo
                int posicao = posicaoRegistro(chave);
                //Se posição é -1 não encontrou
                if (posicao != -1) {
                    cout << "Achei o registro na posição " << (posicao + 1) << endl;
                } else {
                    cout << "Não Achei o registro com a chave " << chave << endl;
                }
                break;
            }
            case 8:
            {
                //Pergunta qual o nome do cliente deve ser procurada no arquivo                
                char chave[85];
                //Limpa o fluxo
                fflush(stdin); //ou cin.ignore();
                cout << "Digite o nome a ser perquisado: ";
                gets(chave);
                // Procura o registro do cliente com a chave no arquivo
                Cliente registro = pesquisarRegistroNome(chave);
                // Se registro.codigo != -1 encontrou o registro
                if (registro.codigo != -1) {
                    cout << "Achei o registro " << endl;
                    cout << registro.codigo << " -  " << registro.nome << " - " << registro.cpf << endl;
                } else {
                    cout << "Não Achei o registro com o nome " << chave << endl;
                }

                break;
            }
            case 9:
            {
                //Lista logicamente os dados do arquivo. Não inclui chave com -1                    
                cout << "Lista Lógico:" << endl;
                listarLogico();
                cout << endl;
                break;
            }
            case 10:
            {
                //Lista fisicamente os dados do arquivo
                cout << "Lista Físico:" << endl;
                listarFisico();
                cout << endl;
                break;
            }
            case 11:
            {
                //Retorna as informações do arquivo
                informacoes();
                break;
            }
            case 12:
            {
                //Esvazia o arquivo de dados
                if (zeraArquivo() == true) {
                    cout << "Arquivo zerado com sucesso!" << endl;
                } else {
                    cout << "Arquivo não foi zerado!" << endl;
                }
                break;
            }
            case 99:
                cout << endl << "Saindo do sistema";
                break;
            default:
                cout << endl << "Opção inválida";
        }
    }
    return EXIT_SUCCESS;
}
