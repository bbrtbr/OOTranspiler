class Pessoa
{
public:
    int idade;
    int pontos;
    bool maior;

    Pessoa(int inicial)
    {
        this.idade = inicial;
        this.pontos = 0;
        this.maior = false;
    }

    void adicionarPontos(int valor)
    {
        pontos = pontos + valor;
    }

    void atualizarIdade(int nova)
    {
        this.idade = nova;
    }

    void verificarMaioridade(int limite)
    {
        maior = idade >= limite;
    }
};

int main()
{
    Pessoa p(17);

    p.adicionarPontos(10);
    p.atualizarIdade(18);
    p.verificarMaioridade(18);
}
