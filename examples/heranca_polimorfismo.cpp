class Animal
{
public:
    int idade;
    int som;

    void crescer()
    {
        idade = idade + 1;
    }

    void falar()
    {
        som = 1;
    }
};

class Cachorro : Animal
{
public:
    int peso;

    void falar()
    {
        som = 2;
    }
};

int main()
{
    Animal a;
    Cachorro c;

    a.idade = 1;
    a.som = 0;

    c.idade = 2;
    c.som = 0;
    c.peso = 10;

    a = c;
    a.crescer();
    a.falar();
}
