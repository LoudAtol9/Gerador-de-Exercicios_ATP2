#include "MyMySQL.h" 
#define QNT_CLS 10

// Protótipos das Funções

// Auxiliares:
bool str_comp(char* str_arq, char* str_input);
char* str_trim(char* str);
char* normaliza_str(char* str);
int int_pow(int base, int exp);


// "Tradutores" :
int char_2_int(char* str, int len);
float char_2_float(char* str, int len);
char* type_2_str(int tipo);


// Sizeof's :
size_t type_2_sizeof(int tipo);
size_t sizeof_obj(FILE* file_ptr, size_t tam);


// Transporte com as structs :
PACOTE* criador_PACOTE();
int var_para_pacote(PACOTE* pact, VAR* var);


// Endereços:
fpos_t* retorna_endereco(FILE* file_ptr, int* qnt);
fpos_t* busca_end_por_nome(FILE* file_ptr, char* nome_cls, char* nome_obj);
fpos_t* busca_end_por_id(FILE* file_ptr, int id_cls, int id_obj);
void salva_endereco(bool obj, FILE* file_ptr, fpos_t* r_end, fpos_t* w_end);


// Escreve ou Lê:
void* recebe_var(FILE* file_ptr, char* buffer, int* tipo, int* len);
void escreve_var(FILE* file_ptr, char* buffer, int tipo, int len, void* var, int tam);
void add_var(FILE* file_ptr, char* nome, int tipo, int len);


// Busca (lê) - Edita - Escreve:
void* proto_busca_var_por_nome(int* tipo, int* len, FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var);
void* proto_busca_var_por_ID(int* tipo, int* len, FILE* file_ptr, int id_cls, int id_obj, char* nome_var);
int qnts_classes_nome(FILE* file_ptr);
int qnts_objetos_nome(FILE* file_ptr, char* nome_cls);
int editar_var_por_nome(FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var, void* var, int tam_var);
int editar_var_por_ID(FILE* file_ptr, int id_cls, int id_obj, char* nome_var, void* var, int tam_var);
void inicia_db(FILE* file_ptr, int qnts_cls);
void criar_classe(FILE* file_ptr, char* nome_cls, int qnt_obj, char** nomes_var, int* tipos_var, int* tam_var, int qnt);
void criar_objeto(FILE* file_ptr, char* nome_cls, char* nome_obj);


// MYMYSQL:
FILE* abrir_arq(char* file_nome, bool* criou);
int busca_var_por_nome(PACOTE* pact, FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var);
int busca_var_por_ID(PACOTE* pact, FILE* file_ptr, int id_cls, int id_obj, char* nome_var);


// UX (interface)
void print_cls_db(FILE* file_ptr, char* nome_cls);


// Corta a str pra so a parte inserida no programa anteriormente
char* str_trim(char* str)
{
    int i;
    int pos_newline = 0;

    for (i = 0; i < strlen(str); i++)
        pos_newline = (str[i] == '\n') ? i : pos_newline;
    
    char* new_str = (char*) malloc((pos_newline + 1) * sizeof(char));

    for (i = 0; i < pos_newline; i++)
        new_str[i] = str[i];
    new_str[i] = '\0';

    free(str);
    return new_str;
}

// Recebe variavel e dá um tipo pra ela
int var_para_pacote(PACOTE* pact, VAR* var)
{
    VAR_INT* var_int = (VAR_INT*) malloc(sizeof(VAR_INT));
    VAR_FLOAT* var_float = (VAR_FLOAT*) malloc(sizeof(VAR_FLOAT));
    VAR_CHAR* var_char = (VAR_CHAR*) malloc(sizeof(VAR_CHAR));

    switch (var->tipo)
    {
    case TYPE_INT:
        var_int->ptr_var = (int*) var->ptr_var;
        var_int->len = var->len;
        free(var);
        free(var_float);
        free(var_char);
        pact->int_lst[(pact->int_count)++] = var_int;
        return (pact->int_count - 1);
        break;
    
    case TYPE_FLOAT:
        var_float->ptr_var = (float*) var->ptr_var;
        var_float->len = var->len;
        free(var);
        free(var_int);
        free(var_char);
        pact->float_lst[(pact->float_count)++] = var_float;
        return (pact->float_count - 1);
        break;

    case TYPE_CHAR:
        var_char->ptr_var = (char*) var->ptr_var;
        var_char->ptr_var = str_trim(var_char->ptr_var);
        var_char->len = var->len;
        free(var);
        free(var_int);
        free(var_float);
        pact->char_lst[(pact->char_count)++] = var_char;
        return (pact->char_count - 1);
        break;
    }
    return 1;
}


// Potencia
int int_pow(int base, int exp)
{
    int i;

    if (exp == 0)
        return 1;

    for (i = 0; i < exp; i++)
        base = base * base;

    return base; 
}


// Compara duas Strings
bool str_comp(char* str_arq, char* str_input)
{
    int i;
    bool b;

    for ( i = 0; i < 10; i++)
    {
        if (str_arq[i] == '\n')
            break;
        
        b = (str_arq[i] == str_input[i]);

        if (!b)
            break;
    }
    return b;       
}

// Retorne quantas classes tem 
int qnts_classes_nome(FILE* file_ptr)
{
    int i;
    fpos_t* end_cls;
    fpos_t* end_obj;
    int qnt_cls;
    int qnt_obj;
    char* nome = (char*) malloc(10 * sizeof(char));

    fseek(file_ptr, 0, SEEK_SET);

    // Recebe a lista com os enderecos das classes
    end_cls = retorna_endereco(file_ptr, &qnt_cls);
    return qnt_cls;
}


// Retorna quantas objetos tem
int qnts_objetos_nome(FILE* file_ptr, char* nome_cls)
{
    int i;
    fpos_t* end_cls;
    fpos_t* end_obj;
    int qnt_cls;
    int qnt_obj;
    char* nome = (char*) malloc(10 * sizeof(char));

    fseek(file_ptr, 0, SEEK_SET);

    // Recebe a lista com os enderecos das classes
    end_cls = retorna_endereco(file_ptr, &qnt_cls);

    // Ve qual classe tem o nome requisitado
    for (i = 0; i < qnt_cls; i++)
    {
        fsetpos(file_ptr, &(end_cls[i]));
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_cls))
            break;
    }

    // Recebe a lista com os enderecos dos objetos
    end_obj = retorna_endereco(file_ptr, &qnt_obj);
    return qnt_obj;
}


// Retorna a lista de enderecos
fpos_t* retorna_endereco(FILE* file_ptr, int* qnt)
{
    // Lê a primeira linha com o número de classes
    fread(qnt, sizeof(int), 1, file_ptr);

    // Pega a lista de enderecos das classes
    fpos_t* end = (fpos_t*) malloc((*qnt) * sizeof(fpos_t));
    fread(end, sizeof(fpos_t), *qnt, file_ptr);

    return end;
}



// Retorna o endereco do obj
fpos_t* busca_end_por_nome(FILE* file_ptr, char* nome_cls, char* nome_obj)
{
    int i;
    fpos_t* end_cls;
    fpos_t* end_obj;
    int qnt_cls;
    int qnt_obj;
    char* nome = (char*) malloc(10 * sizeof(char));

    fseek(file_ptr, 0, SEEK_SET);

    // Recebe a lista com os enderecos das classes
    end_cls = retorna_endereco(file_ptr, &qnt_cls);

    // Ve qual classe tem o nome requisitado
    for (i = 0; i < qnt_cls; i++)
    {
        fsetpos(file_ptr, &(end_cls[i]));
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_cls))
            break;
    }

    // Recebe a lista com os enderecos dos objetos
    end_obj = retorna_endereco(file_ptr, &qnt_obj);

    // Ve qual objeto tem o nome requisitado
    for (i = 0; i < qnt_obj; i++)
    {
        fsetpos(file_ptr, &(end_obj[i]));
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_obj))
            break;
    }

    free(nome);
    free(end_cls);
    return end_obj;
}



// Retorna o endereco do obj por ID
fpos_t* busca_end_por_id(FILE* file_ptr, int id_cls, int id_obj)
{
    fpos_t* end_cls;
    fpos_t* end_obj;
    int qnt_cls;
    int qnt_obj;
    char* nome = (char*) malloc(10 * sizeof(char));

    fseek(file_ptr, 0, SEEK_SET);

    // Recebe a lista com os enderecos das classes
    end_cls = retorna_endereco(file_ptr, &qnt_cls);

    // Vai pro endereco do ID da classe
    fsetpos(file_ptr, &(end_cls[id_cls]));
    fread(nome, sizeof(char), 10, file_ptr);

    // Recebe a lista com os enderecos dos objetos
    end_obj = retorna_endereco(file_ptr, &qnt_obj);

    // Vai pro endereco do ID do objeto
    fsetpos(file_ptr, &(end_obj[id_obj]));
    fread(nome, sizeof(char), 10, file_ptr);
    
    free(nome);
    free(end_cls);
    return end_obj;
}


// Recebe o tipo e devolve o tamanho dele
size_t type_2_sizeof(int tipo)
{
    switch (tipo)
    {
    case TYPE_INT:
        return sizeof(int);
        break;
    
    case TYPE_FLOAT:
        return sizeof(float);
        break;

    case TYPE_CHAR:
        return sizeof(char);
        break;
    }
}


// Lê a variavel do objeto e aloca ela no heap
void* recebe_var(FILE* file_ptr, char* buffer, int* tipo, int* len)
{
    void* var;

    fread(tipo, sizeof(int), 1, file_ptr);
    fread(len, sizeof(int), 1, file_ptr);

    var = malloc((*len) * type_2_sizeof(*tipo));
    fread(var, type_2_sizeof(*tipo), *len, file_ptr);

    return var;
}


// Retorna o ponteiro variavel do objeto seu tipo e tamanho
void* proto_busca_var_por_nome(int* tipo, int* len, FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var)
{
    void* var;
    char* nome = (char*) malloc(10 * sizeof(char));
    
    fpos_t* end_obj = busca_end_por_nome(file_ptr, nome_cls, nome_obj);

    while (1)
    {
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_var))
        {
            var = recebe_var(file_ptr, nome, tipo, len);
            return var;
        }
        free(recebe_var(file_ptr, nome, tipo, len));    
    }   
}

// Retorna o index para usar no pact->type_var
int busca_var_por_nome(PACOTE* pact, FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var)
{
    VAR* var = (VAR*) malloc(sizeof(VAR));
    var->ptr_var = proto_busca_var_por_nome(&(var->tipo), &(var->len), file_ptr, nome_cls, nome_obj, nome_var);

    return var_para_pacote(pact, var); 
}


// Retorna o ponteiro variavel do objeto seu tipo e tamanho pelo ID
void* proto_busca_var_por_ID(int* tipo, int* len, FILE* file_ptr, int id_cls, int id_obj, char* nome_var)
{
    int i;
    void* var;
    char* nome = (char*) malloc(10 * sizeof(char));

    fpos_t* end_obj = busca_end_por_id(file_ptr, id_cls, id_obj);

    while (1)
    {
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_var))
        {
            var = recebe_var(file_ptr, nome, tipo, len);
            return var;
        }
        free(recebe_var(file_ptr, nome, tipo, len));    
    }
}

// Retorna o index para usar no pact->type_var
int busca_var_por_ID(PACOTE* pact, FILE* file_ptr, int id_cls, int id_obj, char* nome_var)
{
    VAR* var = (VAR*) malloc(sizeof(VAR));
    var->ptr_var = proto_busca_var_por_ID(&(var->tipo), &(var->len), file_ptr, id_cls, id_obj, nome_var);

    return var_para_pacote(pact, var); 
}


// Para imprimir na tela o tipo
char* type_2_str(int tipo)
{
    switch (tipo)
    {
    case TYPE_INT:
        return "Int";
        break;
    
    case TYPE_FLOAT:
        return "Float";
        break;

    case TYPE_CHAR:
        return "Char";
        break;
    }
}



// Escreve a Variavel no arquivo
void escreve_var(FILE* file_ptr, char* buffer, int tipo, int len, void* var, int tam)
{
    int i;
    int in = 0;
    float f = 0.0;
    char c = ' ';
    char newline = '\n';

    // Salva o nome, tipo, e tamanho
    fwrite(buffer, sizeof(char), 10, file_ptr);
    fwrite(&tipo, sizeof(int), 1, file_ptr);
    fwrite(&len, sizeof(int), 1, file_ptr);
    fwrite(var, type_2_sizeof(tipo), tam, file_ptr);

    switch (tipo)
    {
    case TYPE_INT:
        for (i = 0; i < len - tam; i++)
            fwrite(&in, sizeof(int), 1, file_ptr);
        break;
    
    case TYPE_FLOAT:
        for (i = 0; i < len - tam; i++)
            fwrite(&f, sizeof(float), 1, file_ptr);
        break;
    case TYPE_CHAR:
        fwrite(&newline, sizeof(char), 1, file_ptr);
        for (i = 1; i < len - tam; i++)
            fwrite(&c, sizeof(char), 1, file_ptr);
        break;
    }


}


// Edita a Variavel no local designado 
int editar_var_por_nome(FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var, void* var, int tam_var)
{
    int i;
    int tipo;
    int len;
    char* nome = (char*) malloc(11 * sizeof(char));
    fpos_t* end_obj = busca_end_por_nome(file_ptr, nome_cls, nome_obj);
    fpos_t var_atual;
    
    while (1)
    {
        fgetpos(file_ptr, &var_atual);

        fread(nome, sizeof(char), 10, file_ptr);

        if (str_comp(nome, nome_var))
        {
            fsetpos(file_ptr, &var_atual);
            fread(nome, sizeof(char), 10, file_ptr);
            fread(&tipo, sizeof(int), 1, file_ptr);
            fread(&len, sizeof(int), 1, file_ptr); 
            fsetpos(file_ptr, &var_atual);

            escreve_var(file_ptr, nome, tipo, len, var, tam_var);
            return 0;
        } 
        else    
            free(recebe_var(file_ptr, nome, &tipo, &len));    
    }
}


// Edita a Variavel no local designado pelo ID
int editar_var_por_ID(FILE* file_ptr, int id_cls, int id_obj, char* nome_var, void* var, int tam_var)
{
    int i;
    int tipo;
    int len;
    char* nome = (char*) malloc(11 * sizeof(char));
    fpos_t* end_obj = busca_end_por_id(file_ptr, id_cls, id_obj);
    fpos_t var_atual;
    
    while (1)
    {
        fgetpos(file_ptr, &var_atual);

        fread(nome, sizeof(char), 10, file_ptr);

        if (str_comp(nome, nome_var))
        {
            fsetpos(file_ptr, &var_atual);
            fread(nome, sizeof(char), 10, file_ptr);
            fread(&tipo, sizeof(int), 1, file_ptr);
            fread(&len, sizeof(int), 1, file_ptr); 
            fsetpos(file_ptr, &var_atual);

            escreve_var(file_ptr, nome, tipo, len, var, tam_var);
            return 0;
        } 
        else    
            free(recebe_var(file_ptr, nome, &tipo, &len));
    } 
}


/*
// Edita a Variavel no local designado 
int editar_var_UX(FILE* file_ptr, char* nome_cls, char* nome_obj, int tam_var)
{
    int i;
    int j;
    int tipo;
    int len;
    void* var;
    int* var_int;
    float* var_float;
    char* var_char;
    char* nome = (char*) malloc(11 * sizeof(char));
    fpos_t* end_obj = busca_end_por_nome(file_ptr, nome_cls, nome_obj);
    fpos_t end_1;
    
    while (1)
    {
        fgetpos(file_ptr, &end_1);

        fread(nome, sizeof(char), 10, file_ptr);

        if(nome[0] == ';')
            return 0;

        printf("Variavel: %s\n", nome);
        fread(&tipo, sizeof(int), 1, file_ptr);
        printf("Tipo : %d\n",tipo);
        fread(&len, sizeof(int), 1, file_ptr);
        printf("Tamanho : %d\n", len);
        printf("Escrito : ");

        var = malloc((len) * type_2_sizeof(tipo));
        fread(var, type_2_sizeof(tipo), len, file_ptr);

        switch (tipo)
            {
            case TYPE_INT:
                var_int = (int*) var;
                for (j = 0; j < len; j++)
                    printf("%d ", var_int[j]);
                break;
    
            case TYPE_FLOAT:
                var_float = (float*) var;
                for (j = 0; j < len; j++)
                    printf("%f ", var_float[j]);
                break;

            case TYPE_CHAR:
                var_char = (char*) var;
                for (j = 0; j < len; j++)
                    printf("%c", var_char[j]);
                break;
            }
            printf("\n\n");

        printf("Deseja Mudar o valor da variavel?\n");
        printf("0 - Nao\n");
        printf("1 - Sim\n");
        sscanf("%d", j);

        if (j == 1)
        {
            switch (tipo)
            {
            case TYPE_INT:
                sscanf("%s");
                for (i = 0; i < len; i++)
                {   
                }
                
                break;
    
            case TYPE_FLOAT:
                var_float = (float*) var;
                for (j = 0; j < len; j++)
                    printf("%f ", var_float[j]);
                break;

            case TYPE_CHAR:
                var_char = (char*) var;
                for (j = 0; j < len; j++)
                    printf("%c", var_char[j]);
                break;
            }

            escreve_var(file_ptr, nome, tipo, len, var, tam_var);
        }
        

        free(var);

           
    }
}
*/

// Usa na primeira vez que o db foi criado
void inicia_db(FILE* file_ptr, int qnts_cls)
{
    int i = 0;
    fpos_t end;
    fgetpos(file_ptr, &end);

    fwrite(&i, sizeof(int), 1, file_ptr);
    for (i = 0; i < qnts_cls; i++)
        fwrite(&end, sizeof(fpos_t), 1, file_ptr);
}


// Deixa a str do formato correto a ser adicionado no DB
char* normaliza_str(char* str)
{
    int i;
    int j;
    int len = strlen(str);
    int dif = len - 9;
    char* new_str = (char*) malloc(10 * sizeof(char));

    for (i = 0; i < len; i++)
        new_str[i] = str[i];
    
    new_str[i] = '\n';
    
    for (j = i + 1; j < 10; j++)
        new_str[j] = ' ';

    return new_str;    
}


// Adiciona um campo no Obj
void add_var(FILE* file_ptr, char* nome, int tipo, int len)
{
    int i;
    float j = 0.0;
    char blank_space = ' ';
    char end_str = '\n';
    nome = normaliza_str(nome);

    fwrite(nome, sizeof(char), 10, file_ptr);
    fwrite(&tipo, sizeof(int), 1, file_ptr);
    fwrite(&len, sizeof(int), 1, file_ptr);

    // Preenche o tamanho alocado no arquivo
    switch (tipo)
    {
    case TYPE_INT:
        for (i = 0; i < len; i++)
            fwrite(&len, sizeof(int), 1, file_ptr);
        break;
    
    case TYPE_FLOAT:
        for (i = 0; i < len; i++)
            fwrite(&j, sizeof(float), 1, file_ptr);
        break;

    case TYPE_CHAR:
        for (i = 0; i < len; i++)
            fwrite(&blank_space, sizeof(char), 1, file_ptr);
        break;
    }
    free(nome);
}


// Salva o endereco da classe ou obj no arquivo
void salva_endereco(bool obj, FILE* file_ptr, fpos_t* r_end, fpos_t* w_end)
{
    int i;
    int len;
    char temp_int[10];
    fpos_t temp_fpos;

    // Vai para o local que se deve salvar
    fsetpos(file_ptr, r_end);

    if (obj)
        fread(temp_int, sizeof(char), 10, file_ptr);

    fgetpos(file_ptr, &temp_fpos);
    fread(&len, sizeof(int), 1, file_ptr);
    fsetpos(file_ptr,&temp_fpos);
    len++;
    fwrite(&len, sizeof(int), 1, file_ptr);

    // Acha o local vago e salva o endereco
    for ( i = 0; i < (len - 1); i++)
        fread(&temp_fpos, sizeof(fpos_t), 1, file_ptr);
    
    fwrite(w_end, sizeof(fpos_t), 1, file_ptr);
}


// Adiciona uma classe no DB
void criar_classe(FILE* file_ptr, char* nome_cls, int qnt_obj, char** nomes_var, int* tipos_var, int* tam_var, int qnt)
{
    int i = 0;
    int len;
    char eoo = END_OF_OBJ;
    char nome[10];
    fpos_t end_init;
    fpos_t end_cls;
    fpos_t end_obj_proto;

    nome_cls = normaliza_str(nome_cls);

    // Vai pro final do arquivo e salva a posicao
    fseek(file_ptr, 0, SEEK_END);
    fgetpos(file_ptr, &end_cls);

    // Salva o nome da classe a qnt de objetos
    fwrite(nome_cls, sizeof(char), strlen(nome_cls), file_ptr);
    free(nome_cls);
    fwrite(&i, sizeof(int), 1, file_ptr);

    // Cria o array de ponteiros de objetos
    for (i = 0; i < qnt_obj; i++)
        fwrite(&end_cls, sizeof(fpos_t), 1, file_ptr);

    // Salva a pos do Proto e volta pra salvar a pos da classe no indice
    fgetpos(file_ptr, &end_obj_proto);
    fseek(file_ptr, 0, SEEK_SET);
    fgetpos(file_ptr, &end_init);
    salva_endereco(false, file_ptr, &end_init, &end_cls);

    // Volta pra classe e salva o endereco do proto
    salva_endereco(true, file_ptr, &end_cls, &end_obj_proto);

    // Salva o prototipo da classe
    fsetpos(file_ptr, &end_obj_proto);
    fwrite("proto\n    ", sizeof(char), 10, file_ptr);

    // Cria cada campo pra variavel, nome, tipo, tam e preenche o espaco na memoria
    for (i = 0; i < qnt; i++)
        add_var(file_ptr, nomes_var[i], tipos_var[i], tam_var[i]);

    // Indica o final do obj
    fwrite(&eoo, sizeof(char), 1, file_ptr);
}


// Vê qnts bits tem um obj
size_t sizeof_obj(FILE* file_ptr, size_t tam)
{
    int i;
    int tipo;
    int len;
    char nome[10];

    fread(nome, sizeof(char), 10, file_ptr);

    if (nome[0] == END_OF_OBJ)
        return tam + sizeof(char);

    fread(&tipo, sizeof(int), 1, file_ptr);
    fread(&len, sizeof(int), 1, file_ptr);

    void* count = malloc(1 * type_2_sizeof(tipo));

    for (i = 0; i < len; i++)
        fread(count, type_2_sizeof(tipo), 1, file_ptr);

    tam = tam + (sizeof(char)) * 10 + (sizeof(int)) * 2 + (type_2_sizeof(tipo)) * len;
    return sizeof_obj(file_ptr, tam);
}


// Adiciona mais um obj na classe (com valores padrao)
void criar_objeto(FILE* file_ptr, char* nome_cls, char* nome_obj)
{
    int i;
    char nome[10];
    int qnt_cls;
    int qnt_obj;
    fpos_t end_1;
    fpos_t end_2;
    fpos_t* lst_end_cls;
    fpos_t* lst_end_obj;
    fpos_t end_obj;

    // Acha o endereco da classe
    nome_obj = normaliza_str(nome_obj);
    lst_end_obj = busca_end_por_nome(file_ptr, nome_cls, "proto");

    // Vê o tamanho do Obj
    fsetpos(file_ptr, &(lst_end_obj[0]));
    fread(nome, sizeof(char), 10, file_ptr);
    fgetpos(file_ptr, &end_1);

    // Aloca o Obj
    size_t tam = sizeof_obj(file_ptr, 0);
    void* obj = malloc(1 * tam);

    // Lê o Obj
    fsetpos(file_ptr, &end_1);
    fread(obj, tam, 1, file_ptr);

    // Salva o Obj
    fseek(file_ptr, 0, SEEK_END);
    fgetpos(file_ptr, &end_obj);
    fwrite(nome_obj, sizeof(char), 10, file_ptr);
    fwrite(obj, tam, 1, file_ptr);

    // Salva o endereco do obj na lista
    fseek(file_ptr, 0, SEEK_SET);
    fread(&qnt_cls, sizeof(int), 1, file_ptr);
    lst_end_cls = (fpos_t*) malloc(qnt_cls * sizeof(fpos_t));
    fread(lst_end_cls, sizeof(fpos_t), qnt_cls, file_ptr);

    for (i = 0; i < qnt_cls; i++)
    {
        fsetpos(file_ptr, &(lst_end_cls[i]));
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_cls))
            break;
    }

    fgetpos(file_ptr, &end_1);
    fread(&qnt_obj, sizeof(int), 1, file_ptr);
    for (i = 0; i < qnt_obj; i++)
        fread(&end_2, sizeof(fpos_t), 1, file_ptr);

    fwrite(&end_obj, sizeof(fpos_t), 1, file_ptr);

    qnt_obj++;
    fsetpos(file_ptr, &end_1);
        fwrite(&qnt_obj, sizeof(int), 1, file_ptr);

    free(lst_end_cls);
    free(lst_end_obj);
}


// Abre o arquivo, se nao existir cria
FILE* abrir_arq(char* file_nome, bool* criou)
{
    FILE* file_ptr = fopen(file_nome, "rb+");
    *criou = (file_ptr == NULL) ? true : false;

    if (file_ptr == NULL)
        file_ptr = fopen(file_nome, "wb+");
        inicia_db(file_ptr, QNT_CLS);

    return file_ptr;
}

PACOTE* criador_PACOTE()
{
    PACOTE* pact = (PACOTE*) malloc(sizeof(PACOTE));
    pact->int_count = 0;
    pact->float_count = 0;
    pact->char_count = 0;
    return pact;
}

void print_cls_db(FILE* file_ptr, char* nome_cls)
{
    int i;
    int j;
    fpos_t* end_cls;
    fpos_t* end_obj;
    int qnt_cls;
    int qnt_obj;
    void* var;
    int* var_int;
    float* var_float;
    char* var_char;
    int tipo;
    int len;
    char* nome = (char*) malloc(10 * sizeof(char));

    fseek(file_ptr, 0, SEEK_SET);

    // Recebe a lista com os enderecos das classes
    end_cls = retorna_endereco(file_ptr, &qnt_cls);

    // Ve qual classe tem o nome requisitado
    for (i = 0; i < qnt_cls; i++)
    {
        fsetpos(file_ptr, &(end_cls[i]));
        fread(nome, sizeof(char), 10, file_ptr);
        if (str_comp(nome, nome_cls))
            break;
    }

    // Recebe a lista com os enderecos dos objetos
    end_obj = retorna_endereco(file_ptr, &qnt_obj);

    for(i = 0; i < qnt_obj; i++)
    {   
        printf("\n\n");
        fsetpos(file_ptr, &(end_obj[i]));
        fread(nome, sizeof(char), 10, file_ptr);

        printf("Nome do Objeto:\n   %s\n\n",nome);
        free(nome);

        nome = (char*) malloc(10 * sizeof(char));
        fread(nome, sizeof(char), 10, file_ptr);

        while (nome[0] != ';')
        {
            fread(&tipo, sizeof(int), 1, file_ptr);
            fread(&len, sizeof(int), 1, file_ptr);

            nome = str_trim(nome);
            var = malloc((len) * type_2_sizeof(tipo));

            fread(var, type_2_sizeof(tipo), len, file_ptr);

            printf("Nome da variavel:\n   %s\n",nome);
            printf("Tipo da variavel:\n   %s\n",type_2_str(tipo));
            printf("Tamanho da variavel:\n   %d\n",len);
            printf("Valor variavel\n   ");

            switch (tipo)
            {
            case TYPE_INT:
                var_int = (int*) var;
                for (j = 0; j < len; j++)
                    printf("%d ", var_int[j]);
                break;
    
            case TYPE_FLOAT:
                var_float = (float*) var;
                for (j = 0; j < len; j++)
                    printf("%f ", var_float[j]);
                break;

            case TYPE_CHAR:
                var_char = (char*) var;
                for (j = 0; j < len; j++)
                    printf("%c", var_char[j]);
                break;
            }
            printf("\n\n");
            free(var);
            free(nome);

            nome = (char*) malloc(10 * sizeof(char));
            fread(nome, sizeof(char), 10, file_ptr);
        }

    }


}

int main()
{
    PACOTE* pact = criador_PACOTE();

    float* ptr_float = (float*) malloc(sizeof(float));
    *ptr_float = 59.99;
    void* ptr_void_1 = ptr_float;

    int* ptr_int = (int*) malloc(sizeof(int));
    *ptr_int = 1;
    void* ptr_void_3 = ptr_int;

    char* ptr_char = (char*) malloc(5 * sizeof(float));
    ptr_char = "Pera";
    void* ptr_void_2 = ptr_char;
    
    char* nome_var[3] = {"preco", "nome", "id"};
    int tipo_var[3] = {TYPE_FLOAT, TYPE_CHAR, TYPE_INT};
    int var_tam[3] = {1, 20, 1};

    char* nome_cls = "produto";
    char* nome_obj = "beatles";

    FILE* file_ptr = abrir_arq("my_orlas_db.bin");

    //inicia_db(file_ptr, 2);

    //criar_classe(file_ptr, nome_cls, 3, nome_var, tipo_var, var_tam, 3);
    //criar_objeto(file_ptr, nome_cls, nome_obj);

    //editar_var_por_nome(file_ptr, nome_cls, nome_obj, "preco", ptr_float, 1);
    //editar_var_por_nome(file_ptr, nome_cls, nome_obj, "nome", ptr_void_2, 4);
    editar_var_por_nome(file_ptr, nome_cls, nome_obj, "id", ptr_void_3, 1);

    busca_var_por_nome(pact, file_ptr, nome_cls, nome_obj, nome_var[0]);
    busca_var_por_ID(pact, file_ptr, 0, 1, nome_var[0]);
    busca_var_por_nome(pact, file_ptr, nome_cls, nome_obj, nome_var[1]);
    print_cls_db(file_ptr, nome_cls);
}
