class Pessoa
{
public:
    int idade;
    bool maior;
    int desconto;

    void fazerAniversario()
    {
        idade = idade + 1;
    }

    void verificarMaioridade()
    {
        maior = idade >= 18;
    }
};

int main()
{
    Pessoa p;

    p.idade = 17;
    p.desconto = 0;
    p.fazerAniversario();
    p.verificarMaioridade();

    if (p.maior)
    {
        p.desconto = 1;
    }
}
