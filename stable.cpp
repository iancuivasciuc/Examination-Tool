#include <bits/stdc++.h>
using namespace std;

class Examen
{
protected:
    static int incr;
    const int ID;
    string denumire;
    float notaScris;
    bool trecut = false;
public:
    explicit Examen(string denumire = "", float notaScris = 0) : ID(incr),
                                                                 denumire(move(denumire)), notaScris(notaScris)
    {
        incr ++;
    }
    [[nodiscard]] string getDenumire() const
    {
        return denumire;
    }
    [[nodiscard]] bool getTrecut() const
    {
        return trecut;
    }
    virtual ~Examen() = 0;
    [[nodiscard]] virtual float notaFinala() const  = 0;
    virtual void citire() = 0;
    virtual void afisare() const = 0;
};
Examen::~Examen() = default;
int Examen::incr = 1;

class Partial : public Examen
{
protected:
    float notaOral;
public:
    explicit Partial(string denumire = "", float notaScris = 0, float notaOral = 0) :
            Examen(move(denumire), notaScris), notaOral(notaOral)
    {
        trecut = (roundf(notaScris) >= 5 and roundf(notaOral) >= 5);
    }
    ~Partial() override = default;
    friend istream &operator>>(istream &in, Partial &ob)
    {
        in >> ob.denumire >> ob.notaScris >> ob.notaOral;
        ob.trecut = (roundf(ob.notaScris) >= 5 and roundf(ob.notaOral) >= 5);
        return in;
    }
    friend ostream &operator<<(ostream &out, const Partial &ob)
    {
        out<<ob.denumire<<' '<<ob.notaFinala()<<'\n';
        return out;
    }
    [[nodiscard]] float notaFinala() const override
    {
        if(! trecut)
            return 1;
        else
            return (notaOral + notaScris) / 2;
    }
    void citire() override
    {
        cout<<"Partial.\nIntroduceti denumirea materiei, nota la scris(float) si nota la oral(float)!\n";
        cin >> denumire >> notaScris >> notaOral;
        trecut = (roundf(notaScris) >= 5 and roundf(notaOral) >= 5);
    }
    void afisare() const override
    {
        cout<<"Partial: "<<notaFinala()<<'\n';
    }
};

class ExamenFinal : public Examen
{
protected:
    float puncteExtra;
public:
    explicit ExamenFinal(string denumire = "", float notaScris = 0, float puncteExtra = 0) :
            Examen(move(denumire), notaScris), puncteExtra(puncteExtra)
    {
        trecut = (roundf(notaScris) >= 5);
    }
    ~ExamenFinal() override = default;
    friend istream &operator>>(istream &in, ExamenFinal &ob)
    {
        in >> ob.denumire >> ob.notaScris >> ob.puncteExtra;
        ob.trecut = (roundf(ob.notaScris) >= 5);
        return in;
    }
    friend ostream &operator<<(ostream &out, const ExamenFinal &ob)
    {
        out<<ob.denumire<<' '<<ob.notaFinala()<<'\n';
        return out;
    }
    [[nodiscard]] float notaFinala() const override
    {
        if(! trecut)
            return 1;
        else
            return min(float(10), (notaScris + puncteExtra));
    }
    void citire() override
    {
        cout<<"Examen final.\nIntroduceti denumirea materiei, nota la scris(float) si punctele extra(float)!\n";
        cin >> denumire >> notaScris >> puncteExtra;
        trecut = (roundf(notaScris) >= 5);
    }
    void afisare() const override
    {
        cout<<"Examen final: "<<notaFinala()<<'\n';
    }
};

class Quiz : public Examen
{
protected:
    int grila;
public:
    explicit Quiz(string denumire = "", float notaScris = 0, int grila = 0) :
            Examen(move(denumire), notaScris), grila(grila)
    {
        trecut = true;
    }
    ~Quiz() override = default;
    friend istream &operator>>(istream &in, Quiz &ob)
    {
        in >> ob.denumire >> ob.notaScris >> ob.grila;
        ob.trecut = (roundf(ob.notaScris) >= 5);
        return in;
    }
    friend ostream &operator<<(ostream &out, const Quiz &ob)
    {
        out<<ob.denumire<<' '<<ob.notaFinala()<<'\n';
        return out;
    }
    [[nodiscard]] float notaFinala() const override
    {
        return notaScris;
    }
    void citire() override
    {
        cout<<"Quiz.\nIntroduceti denumirea materiei, nota la scris(float) si nr de intrebari de la grila!\n";
        cin >> denumire >> notaScris >> grila;
        trecut = (roundf(notaScris) >= 5);
    }
    void afisare() const override
    {
        cout<<"Quiz: "<<notaFinala()<<'\n';
    }
};

template<class type>
class CatalogIndividual
{
protected:
    static int incr;
    static unordered_map <string, vector<int>> picat;
    const int nrMatricol;
    string nume;
    int nr;
    unordered_map<string, type[3]> mp;
public:
    explicit CatalogIndividual(string nume = "", int nr = 0) :
            nrMatricol(incr), nume(move(nume)), nr(nr)
    {
        incr ++;
    }
    ~CatalogIndividual()
    {
        for(const auto &it : mp)
            for(auto elem : it.second)
                if(elem != nullptr)
                    delete elem;
    }
    void citire()
    {
        cout<<"Introduceti numele studentului si nr de examene la care a participat:\n";
        cin >> nume >> nr;
        while(nr --)
        {
            cout<<"1->Partial\n2->ExamenFinal\n3->Quiz\n";
            int optiune;
            type ob = nullptr;
            cin >> optiune;
            if(optiune == 1)
                ob = new Partial;
            else if(optiune == 2)
                ob = new ExamenFinal;
            else if(optiune == 3)
                ob = new Quiz;
            else
                cout<<"Optiunea aleasa nu este disponibila. Incearca din nou!\n";
            if(ob != nullptr)
            {
                ob->citire();
                string denumire = ob->getDenumire();
                if(typeid(*ob) == typeid(Partial))
                {
                    if(mp[denumire][0] == nullptr)
                        mp[denumire][0] = ob;
                    else
                        cout<<"Studentul a dat deja partialul la aceasta materie!\n";
                }
                else if(typeid(*ob) == typeid(ExamenFinal))
                {
                    if(mp[denumire][1] == nullptr)
                    {
                        mp[denumire][1] = ob;
                        bool trecutPartial = false;
                        if(mp[denumire][0] != nullptr)
                        {
                            if(mp[denumire][0]->getTrecut())
                                trecutPartial = true;
                            else
                                delete mp[denumire][0];
                        }

                        if(! trecutPartial)
                        {
                            cout<<"Studentul nu a trecut partialul la aceasta materia pana acum.\n";
                            cout<<"Introduceti nota scrisa si nota orala de la noul partial!\n";
                            float notaScris, notaOral;
                            cin >> notaScris >> notaOral;
                            type p;
                            p = new Partial(denumire, notaScris, notaOral);
                            mp[denumire][0] = p;
                        }
                    }
                    else
                        cout<<"Studentul a dat deja examenul final la aceasta materie!\n";
                }
                else if(typeid(*ob) == typeid(Quiz))
                {
                    if(mp[denumire][2] == nullptr)
                        mp[denumire][2] = ob;
                    else
                        cout<<"Studentul a dat deja quizul la aceasta materie!\n";
                }
            }
        }
    }
    void afisare() const
    {
        cout<<"Studentul "<<nume<<" a obtinut urmatoarele note la examene:\n";
        for(const auto &it : mp)
        {
            cout << it.first << ":\n";
            float aux[3] = {0, 0, 0};
            for(int i = 0; i < 3; i ++)
                if(it.second[i] != nullptr)
                {
                    cout << '\t';
                    it.second[i]->afisare();
                    aux[i] = it.second[i]->notaFinala();
                }
            if(aux[0] < 5 or aux[1] < 5)
                cout<<"\tMaterie picata!\n";
            else
            {
                //  partial: 50%, examen final: 50%, quiz: 10%
                float zece = 10;
                float nota = min(zece, (aux[0] / 2) + (aux[1] / 2) + (aux[2] / 10));
                cout<<"\tMaterie trecuta cu nota: "<<nota<<'\n';
            }
        }
        cout<<'\n';
    }
    void operator+=(const string &denumire)
    {
        cout<<"0->Examenul nu a fost dat\n1->Examenul a fost dat\n\n";
        int optiune;
        type p;
        cout<<"Partial(1 sau 0): \n";
        p = nullptr;
        cin >> optiune;
        if(optiune == 1)
        {
            float n1, n2;
            cout<<"Nota scrisa si nota de la oral: \n";
            cin >> n1 >> n2;
            p = new Partial(denumire, n1, n2);
            nr ++;
        }
        mp[denumire][0] = p;
        cout<<"Examen final(1 sau 0): \n";
        p = nullptr;
        cin >> optiune;
        if(optiune == 1)
        {
            float n1, n2;
            cout<<"Nota scrisa si punctajul extra: \n";
            cin >> n1 >> n2;
            p = new ExamenFinal(denumire, n1, n2);
            nr ++;
        }
        mp[denumire][1] = p;
        cout<<"Quiz(1 sau 0): \n";
        p = nullptr;
        cin >> optiune;
        if(optiune == 1)
        {
            float n;
            int g;
            cout<<"Nota scrisa si nr de probleme tip grila: \n";
            cin >> n >> g;
            p = new Quiz(denumire, n, g);
            nr ++;
        }
        mp[denumire][2] = p;
    }
    void updatePicat()
    {
        for(const auto &it : mp)
            if (it.second[0] == nullptr or it.second[0]->notaFinala() < 5 or it.second[1] == nullptr or it.second[1]->notaFinala() < 5)
                picat[it.first].push_back(nrMatricol);
    }
    static void afisarePicat()
    {
        cout<<"Studentii care au restanta: \n";
        for(const auto &it : picat)
        {
            cout<<it.first<<": ";
            for(auto elem : it.second)
                cout<<elem<<' ';
            cout<<'\n';
        }
        cout<<'\n';
    }
};
template<class type> int CatalogIndividual<type>::incr = 1;
template<class type> unordered_map <string, vector<int>> CatalogIndividual<type>::picat;

//Design Pattern: Singleton
class Catalog
{
    int n;
    vector<CatalogIndividual<Examen *>> v;

    static Catalog *instance;
    explicit Catalog(int n) : n(n) {}
public:
    Catalog(const Catalog &ob) = delete;
    Catalog &operator=(const Catalog &ob) = delete;
    static Catalog *getInstance(int n = 0)
    {
        if(instance == nullptr)
            instance = new Catalog(n);
        return instance;
    }
    void citire()
    {
        cout<<"Nr studenti: \n";
        cin >> n;
        v.resize(n);
        for(int i = 0; i < n; i ++)
        {
            v[i].citire();
            v[i].updatePicat();
        }
    }
    void afisareStudent(const int &i) const
    {
        if(i >= 0 and i < n)
            v[i].afisare();
    }
    void adaugaMaterie(const int &i, const string &materie)
    {
        if(i >= 0 and i < n)
        {
            v[i] += materie;
            v[i].updatePicat();
        }
    }
};
Catalog *Catalog::instance = nullptr;

int main()
{
    Catalog *c = Catalog::getInstance();
    c->citire();
    while(true)
    {
        cout<<"1->Afiseaza student\n2->Adauga o materie noua de examen pentru un student\n3->Afiseaza ID studentilor de au ramas restanti\n4->Out\n";
        int optiune;
        cin >> optiune;
        if(optiune == 1)
        {
            cout<<"Introduceti pozitia: \n";
            int poz; cin >> poz;
            c->afisareStudent(poz);
        }
        else if(optiune == 2)
        {
            cout<<"Intoduceti pozitia si materia: \n";
            int poz;
            string materie;
            cin >> poz >> materie;
            c->adaugaMaterie(poz, materie);
        }
        else if(optiune == 3)
            CatalogIndividual<Examen *>::afisarePicat();
        else break;
    }
    return 0;
}