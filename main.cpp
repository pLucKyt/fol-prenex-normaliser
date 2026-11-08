#include <bits/stdc++.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

const wchar_t simbol_negacija = L'¬';
const wchar_t simbol_i = L'∧';
const wchar_t simbol_ili = L'∨';
const wchar_t simbol_implikacija = L'⇒';
const wchar_t simbol_ekvivalencija = L'⇔';
const wchar_t simbol_svako = L'∀';
const wchar_t simbol_postoji = L'∃';
const wchar_t simbol_tacno = L'⊤';
const wchar_t simbol_netacno = L'⊥';

stack<wstring> letStack;
stack<wchar_t> opStack;
stack<bool> negStack;
map<wchar_t, wstring> mapa_funkcija;
map<wchar_t, wstring> mapa_kvantifikatora;

void print_iskaz(stack<wstring> stk)
{
    stack<wstring> res;
    wcout << "let stack ";
    while(!stk.empty())
    {
        res.push(stk.top());
        stk.pop();
    }
    while(!res.empty())
    {
        wcout << res.top() << " ";
        res.pop();
    }
    wcout << endl;
}

void print_op(stack<wchar_t> stk)
{
    stack<wchar_t> res;
    wcout << "op stack ";
    while(!stk.empty())
    {
        res.push(stk.top());
        stk.pop();
    }
    while(!res.empty())
    {
        wcout << res.top() << " ";
        res.pop();
    }
    wcout << endl;
}

void print_neg(stack<bool> stk)
{
    stack<bool> res;
    wcout << "neg stack ";
    while(!stk.empty())
    {
        res.push(stk.top());
        stk.pop();
    }
    while(!res.empty())
    {
        wcout << res.top() << " ";
        res.pop();
    }
    wcout << endl;
}

void print()
{
    print_iskaz(letStack);
    print_op(opStack);
    print_neg(negStack);
}

bool da_li_moze_na_stack(wchar_t op)
{
    wcout << L"proveri da li operator" << op << L"može na stack" << endl;
    if(op == simbol_ekvivalencija || op == simbol_implikacija)
    {
        if(opStack.empty())
        {
            return true;
        }
        else if(opStack.top() == L'(')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(op == simbol_i || op == simbol_ili)
    {
        if(opStack.empty())
        {
            return true;
        }
        else if(opStack.top() == L'(')
        {
            return true;
        }
        else if(opStack.top() == simbol_ekvivalencija || opStack.top() == simbol_implikacija)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

wstring izracunaj(wchar_t op, wstring a, wstring b)
{
    wstring x;
    if(op == simbol_implikacija)
    {
        x += simbol_negacija;
        x += L'(';
        x += a;
        x += L')';
        x += simbol_ili;
        x += L'(';
        x += b;
        x += L')';
        return x;
    }
    else if(op == simbol_ekvivalencija)
    {
        x += L'(';
        x += L'(';
        x += a;
        x += L')';
        x += simbol_implikacija;
        x += L'(';
        x += b;
        x += L')';
        x += L')';
        x += simbol_i;
        x += L'(';
        x += L'(';
        x += b;
        x += L')';
        x += simbol_implikacija;
        x += L'(';
        x += a;
        x += L')';
        x += L')';
        return x;
    }
    else if(op == simbol_i)
    {
        x += L'(';
        x += a;
        x += L')';
        x += simbol_i;
        x += L'(';
        x += b;
        x += L')';
        return x;
    }
    else if(op == simbol_ili)
    {
        x += L'(';
        x += a;
        x += L')';
        x += simbol_ili;
        x += L'(';
        x += b;
        x += L')';
        return x;
    }
}

void pozovi_sa_stacka_dva_top()
{
    wstring b = letStack.top();
    letStack.pop();
    wstring a = letStack.top();
    letStack.pop();
    wchar_t op = opStack.top();
    opStack.pop();
    wstring res = izracunaj(op, a, b);
    letStack.push(res);
    wcout << L"Pozovi računanje:" << endl;
    print();
}

bool uprosceno_do_kraja(wstring formula)
{
    bool da_li_je_uprosceno = true;
    for(int i = 0; i < formula.size(); i++)
    {
        if(formula[i] == simbol_negacija)
        {
            if(formula[i + 1] == L'(')
            {
                da_li_je_uprosceno = false;
                break;
            }
        }
        else if(formula[i] == simbol_implikacija)
        {
            da_li_je_uprosceno = false;
            break;
        }
        else if(formula[i] == simbol_ekvivalencija)
        {
            da_li_je_uprosceno = false;
            break;
        }
    }
    return da_li_je_uprosceno;
}

void uprosti_logicki_izraz(wstring &formula)
{
    while(!uprosceno_do_kraja(formula))
    {
    for(int i = 0; i < formula.size(); i++)
    {
        if(formula[i] >= L'a' && formula[i] <= L'z')
        {
            wstring sub(1, formula[i]);
            letStack.push(sub);
            wcout << "Gurni slovo u stack" << endl;
            print();
        }
        else if(formula[i] == simbol_negacija)
        {
            i++;
            if(formula[i] == L'(')
            {
                // ¬(
                opStack.push(formula[i]);
                negStack.push(true);
                wcout << "Gurni T u stack" << endl;
                wcout << "Gurni negaciono ( u stack" << endl;
                print();
            }
            else if(formula[i] == simbol_negacija)
            {
                // ¬¬(...)
                continue;
            }
            else
            {
                // ¬p
                wstring pom;
                pom += formula[i - 1];
                pom += formula[i];
                letStack.push(pom);
                wcout << "Gurni negaciono slovo u stack" << endl;
                print();
            }
        }
        else if(formula[i] == L'(')
        {
            opStack.push(formula[i]);
            negStack.push(false);
            wcout << "Gurni nT u stack" << endl;
            wcout << "Gurni ( u stack" << endl;
            print();
        }
        else if(formula[i] == simbol_ekvivalencija || formula[i] == simbol_implikacija)
        {
            while(!da_li_moze_na_stack(formula[i]))
            {
                pozovi_sa_stacka_dva_top();
            }
            opStack.push(formula[i]);
            wcout << L"Gurni operator ⇔ ⇒ na stack" << endl;
            print();
        }
        else if(formula[i] == simbol_i || formula[i] == simbol_ili)
        {
            while(!da_li_moze_na_stack(formula[i]))
            {
                pozovi_sa_stacka_dva_top();
            }
            opStack.push(formula[i]);
            wcout << L"Gurni operator ∧ ∨ na stack" << endl;
            print();
        }
        else if(formula[i] == L')')
        {
            while(opStack.top() != L'(')
            {
                pozovi_sa_stacka_dva_top();
            }
            opStack.pop();
            if(negStack.top())
            {
                // DeMorganovi zakoni
                wstring vrednost = letStack.top();
                letStack.pop();
                wstring negirana_vrednost;
                for(int j = 0; j < vrednost.size(); j++)
                {
                    if(vrednost[j] == L'(')
                    {
                        // (...)
                        int broj_otvorenih = 1;
                        int broj_zatvorenih = 0;
                        negirana_vrednost += simbol_negacija;
                        negirana_vrednost += vrednost[j];
                        j++;
                        while(broj_otvorenih != broj_zatvorenih)
                        {
                            if(vrednost[j] == L'(')
                            {
                                broj_otvorenih++;
                            }
                            else if(vrednost[j] == L')')
                            {
                                broj_zatvorenih++;
                            }
                            negirana_vrednost += vrednost[j];
                            j++;
                        }
                        j--;
                    }
                    else if(vrednost[j] >= L'a' && vrednost[j] <= L'z')
                    {
                        // p
                        negirana_vrednost += simbol_negacija;
                        negirana_vrednost += vrednost[j];
                    }
                    else if(vrednost[j] == simbol_negacija)
                    {
                        // ¬p
                        if(vrednost[j + 1] >= L'a' && vrednost[j + 1] <= L'z')
                        {
                            j++;
                            negirana_vrednost += vrednost[j];
                        }
                        // ¬(p) ili ¬(¬p)
                        else if(vrednost[j + 1] == L'(')
                        {
                            if(vrednost[j + 2] >= L'a' && vrednost[j + 2] <= L'z')
                            {
                                // ¬(p)
                                j += 2;
                                negirana_vrednost += vrednost[j];
                                j++;
                            }
                            else
                            {
                                // ¬(¬p)
                                j += 2;
                                negirana_vrednost += vrednost[j];
                                j++;
                                negirana_vrednost += vrednost[j];
                                j++;
                            }
                        }
                    }
                    j++;
                    if(vrednost[j] == simbol_i)
                    {
                        negirana_vrednost += simbol_ili;
                    }
                    else if(vrednost[j] == simbol_ili)
                    {
                        negirana_vrednost += simbol_i;
                    }
                }
                letStack.push(negirana_vrednost);
            }
            negStack.pop();
        }
    }
    wcout << L"Kraj stringa, gledaj da li je ostalo nešto u stacku" << endl;
    print();
    while(!opStack.empty())
    {
        pozovi_sa_stacka_dva_top();
    }
    wcout << L"Kraj svega, gledaj šta je ostalo" << endl;
    formula = letStack.top();
    letStack.pop();
    wcout << formula << endl;
    wcout << L"Ako treba ponovo cemo ovo proći kroz formulu" << endl;
    }
}

bool da_li_je_nestala_negacija_pre_kvantifikatora(wstring formula)
{
    bool nema_negacije = true;
    for(int i = 0; i < formula.size(); i++)
    {
        if(formula[i] == simbol_negacija)
        {
            if(formula[i + 1] == L'(')
            {
                // ¬(
                if(formula[i + 2] == simbol_svako || formula[i + 2] == simbol_postoji)
                {
                    // ¬(∀ || ¬(∃
                    nema_negacije = false;
                    break;
                }
            }
            else if(formula[i + 1] == simbol_svako || formula[i + 1] == simbol_postoji)
            {
                // ¬∀ || ¬∃
                nema_negacije = false;
                break;
            }
        }
    }
    return nema_negacije;
}

void provuci_negaciju_kroz_kvantifikatore(wstring &formula)
{
    while(!da_li_je_nestala_negacija_pre_kvantifikatora(formula))
    {
        for(int i = 0; i < formula.size(); i++)
        {
            wstring nova_formula;
            if(formula[i] == simbol_negacija)
            {
                for(int k = 0; k < i; k++)
                {
                    nova_formula += formula[k];
                }
                if(formula[i + 1] == L'(')
                {
                    // ¬(
                    if(formula[i + 2] == simbol_svako || formula[i + 2] == simbol_postoji)
                    {
                        // ¬(∀x... || ¬(∃x...
                        nova_formula += L'(';
                        if(formula[i + 2] == simbol_svako)
                        {
                            nova_formula += simbol_postoji;
                        }
                        else if(formula[i + 2] == simbol_postoji)
                        {
                            nova_formula += simbol_svako;
                        }
                        nova_formula += formula[i + 3];
                        nova_formula += simbol_negacija;
                        for(int j = i + 4; j < formula.size(); j++)
                        {
                            nova_formula += formula[j];
                        }
                    }
                    else if(formula[i + 2] == simbol_negacija && (formula[i + 3] == simbol_svako || formula[i + 3] == simbol_postoji))
                    {
                        // ¬(¬∀x...
                        for(int j = i + 3; j < formula.size(); j++)
                        {
                            nova_formula += formula[j];
                        }
                    }
                    else
                    {
                        for(int w = i; w < formula.size(); w++)
                        {
                            nova_formula += formula[w];
                        }
                    }
                }
                else if(formula[i + 1] == simbol_negacija && (formula[i + 2] == simbol_svako || formula[i + 2] == simbol_postoji))
                {
                    // ¬¬∀x...
                    for(int j = i + 2; j < formula.size(); j++)
                    {
                        nova_formula += formula[j];
                    }
                }
                else if(formula[i + 1] == simbol_svako || formula[i + 1] == simbol_postoji)
                {
                    // ¬∀x || ¬∃x
                    if(formula[i + 1] == simbol_svako)
                    {
                        nova_formula += simbol_postoji;
                    }
                    else if(formula[i + 1] == simbol_postoji)
                    {
                        nova_formula += simbol_svako;
                    }
                    nova_formula += formula[i + 2];
                    if(formula[i + 3] == L'(')
                    {
                        nova_formula += L'(';
                        nova_formula += simbol_negacija;
                        nova_formula += L'(';
                        int broj_otvorenih = 1;
                        int broj_zatvorenih = 0;
                        bool samo_jedanput_cemo_ovo_uraditi = false;
                        for(int j = i + 4; j < formula.size(); j++)
                        {
                            if(formula[j] == L'(')
                            {
                                broj_otvorenih++;
                            }
                            else if(formula[j] == L')')
                            {
                                broj_zatvorenih++;
                            }
                            nova_formula += formula[j];
                            if(broj_otvorenih == broj_zatvorenih && !samo_jedanput_cemo_ovo_uraditi)
                            {
                                nova_formula += L')';
                                samo_jedanput_cemo_ovo_uraditi = true;
                            }
                        }
                    }
                    else
                    {
                        nova_formula += simbol_negacija;
                        for(int j = i + 3; j < formula.size(); j++)
                        {
                            nova_formula += formula[j];
                        }
                    }
                }
                else
                {
                    for(int w = i; w < formula.size(); w++)
                    {
                        nova_formula += formula[w];
                    }
                }
            }
            else
            {
                nova_formula = formula;
            }
            wcout << nova_formula << endl;
            formula = nova_formula;
        }
    }
}

void sredi_iskazne_formule_unutar_kvantifikatora(wstring &formula)
{
    wstring resenje;
    for(int i = 0; i < formula.size(); i++)
    {
        resenje += formula[i];
        if((formula[i] == simbol_svako || formula[i] == simbol_postoji) && formula[i + 2] == L'(')
        {
            i++;
            resenje += formula[i];
            i++;
            resenje += formula[i];
            i++;
            int broj_otvorenih = 1;
            int broj_zatvorenih = 0;
            wstring iskazna_formula;
            wcout << resenje << endl;
            while(broj_otvorenih != broj_zatvorenih)
            {
                if(formula[i] == L'(')
                {
                    broj_otvorenih++;
                }
                else if(formula[i] == L')')
                {
                    broj_zatvorenih++;
                }
                iskazna_formula += formula[i];
                i++;
            }
            i--;
            iskazna_formula.pop_back();
            wcout << resenje << endl;
            wcout << iskazna_formula << endl;
            uprosti_logicki_izraz(iskazna_formula);
            resenje += iskazna_formula;
            resenje += formula[i]; //poslednja zatvorena zagrada
        }
        wcout << resenje << endl;
    }
    formula = resenje;
}

void uvedi_smenu_funkcija(wstring &formula)
{
    wstring nova_formula;
    wchar_t brojac = L'a';
    for(int i = 0; i < formula.size(); i++)
    {
        if((formula[i] >= L'a' && formula[i] <= L'z') && formula[i + 1] == L'(' && formula[i - 1] != simbol_svako && formula[i - 1] != simbol_postoji)
        {
            wstring string_funkcija;
            string_funkcija += formula[i];
            i++;
            string_funkcija += formula[i];
            i++; //sada smo prosli prvu zagradu f(...
            int broj_otvorenih = 1;
            int broj_zatvorenih = 0;
            while(broj_otvorenih != broj_zatvorenih)
            {
                if(formula[i] == L'(')
                {
                    broj_otvorenih++;
                }
                else if(formula[i] == L')')
                {
                    broj_zatvorenih++;
                }
                string_funkcija += formula[i];
                i++;
            }
            i--;
            nova_formula += brojac;
            mapa_funkcija[brojac] = string_funkcija;
            wchar_t pomoc = static_cast<wchar_t> (brojac + 1);
            brojac = pomoc;
        }
        else
        {
            nova_formula += formula[i];
        }
    }
    formula = nova_formula;
}

void uvedi_smenu_kvantifikatora(wstring &formula)
{
    wstring nova_formula;
    wchar_t brojac = L'a';
    for(int i = 0; i < formula.size(); i++)
    {
        if(formula[i] == simbol_svako || formula[i] == simbol_postoji)
        {
            wstring string_kvantifikator;
            int broj_otvorenih = 0;
            int broj_zatvorenih = 0;
            bool pusti_na_pocetku = true;
            while(broj_otvorenih != broj_zatvorenih || pusti_na_pocetku)
            {
                if(broj_otvorenih != 0)
                {
                    pusti_na_pocetku = false;
                }
                if(formula[i] == L'(')
                {
                    broj_otvorenih++;
                }
                else if(formula[i] == L')')
                {
                    broj_zatvorenih++;
                }
                string_kvantifikator += formula[i];
                i++;
            }
            i--;
            nova_formula += brojac;
            mapa_kvantifikatora[brojac] = string_kvantifikator;
            wchar_t pomoc = static_cast<wchar_t> (brojac + 1);
            brojac = pomoc;
        }
        else
        {
            nova_formula += formula[i];
        }
    }
    formula = nova_formula;
}

void vrati_smene(wstring &formula, map<wchar_t, wstring> mapica)
{
    wstring nova_formula;
    auto var = mapica;
    for(int i = 0; i < formula.size(); i++)
    {
        bool izadji_cim_nadje = true;
        for(auto &par : var)
        {
            if(formula[i] == par.first)
            {
                nova_formula += par.second;
                izadji_cim_nadje = false;
                break;
            }
        }
        if(izadji_cim_nadje)
        {
            nova_formula += formula[i];
        }
    }
    formula = nova_formula;
}

void vrati_smenu_kvantifikatora(wstring &formula)
{
    vrati_smene(formula, mapa_kvantifikatora);
}

void vrati_smenu_funkcija(wstring &formula)
{
    vrati_smene(formula, mapa_funkcija);
}

void smeni_ponavljajuce_promenljive_pored_kvantifikatora(wstring &formula)
{
    wstring nova_formula;
    vector<wchar_t> prethodna_pojavljivanja;
    wchar_t brojac = L'a';
    for(int i = 0; i < formula.size(); i++)
    {
        map<wchar_t, wchar_t> smene;
        if(formula[i] == simbol_svako || formula[i] == simbol_postoji)
        {
            int broj_otvorenih = 0;
            int broj_zatvorenih = 0;
            bool pusti_na_pocetku = true;
            while(broj_otvorenih != broj_zatvorenih || pusti_na_pocetku)
            {
                if(broj_otvorenih != 0)
                {
                    pusti_na_pocetku = false;
                }
                if(formula[i] == L'(')
                {
                    broj_otvorenih++;
                }
                if(formula[i] == L')')
                {
                    broj_zatvorenih++;
                }
                if(formula[i] >= L'a' && formula[i] <= L'z' && (formula[i - 1] == simbol_svako || formula[i - 1] == simbol_postoji))
                {
                    bool da_li_se_pojavio_samo_jednom = true;
                    for(auto help : prethodna_pojavljivanja)
                    {
                        if(formula[i] == help)
                        {
                            da_li_se_pojavio_samo_jednom = false;
                            break;
                        }
                    }
                    if(da_li_se_pojavio_samo_jednom)
                    {
                        prethodna_pojavljivanja.push_back(formula[i]);
                        nova_formula += formula[i];
                    }
                    else
                    {
                        smene[formula[i]] = brojac;
                        nova_formula += brojac;
                        wchar_t pomoc = static_cast<wchar_t> (brojac + 1);
                        brojac = pomoc;
                    }
                }
                else if(formula[i] >= L'a' && formula[i] <= L'z')
                {
                    bool da_li_nismo_zamenili_slovo = true;
                    for(auto &par : smene)
                    {
                        if(formula[i] == par.first)
                        {
                            nova_formula += par.second;
                            da_li_nismo_zamenili_slovo = false;
                            break;
                        }
                    }
                    if(da_li_nismo_zamenili_slovo)
                    {
                        nova_formula += formula[i];
                    }
                }
                else
                {
                    nova_formula += formula[i];
                }
                i++;
            }
            i--;
        }
        else
        {
            nova_formula += formula[i];
        }
    }
    formula = nova_formula;
}

void skolemizacija(wstring &formula)
{
    wstring nova_formula;
    vector<wstring> delovi_koje_ubacujem;
    for(int i = 0; i < formula.size(); i++)
    {
        if(formula[i] == simbol_svako || formula[i] == simbol_postoji)
        {
            nova_formula += formula[i];
        }
        else if(formula[i] >= L'a' && formula[i] <= L'z' && (formula[i - 1] == simbol_svako || formula[i - 1] == simbol_postoji))
        {
            nova_formula += formula[i];
        }
        else if(formula[i] == L'(' && (formula[i - 2] == simbol_svako || formula[i - 2] == simbol_postoji))
        {
            int broj_otvorenih = 0;
            int broj_zatvorenih = 0;
            wstring iskazna_formula;
            bool pusti_na_pocetku = true;
            while(broj_otvorenih != broj_zatvorenih || pusti_na_pocetku)
            {
                if(broj_otvorenih != 0)
                {
                    pusti_na_pocetku = false;
                }
                if(formula[i] == L'(')
                {
                    broj_otvorenih++;
                }
                else if(formula[i] == L')')
                {
                    broj_zatvorenih++;
                }
                iskazna_formula += formula[i];
                i++;
            }
            i--;
            delovi_koje_ubacujem.push_back(iskazna_formula);
        }
        else if(formula[i] == simbol_i || formula[i] == simbol_ili)
        {
            wstring pomoc;
            pomoc += formula[i];
            delovi_koje_ubacujem.push_back(pomoc);
        }
    }
    nova_formula += L'(';
    for(auto it : delovi_koje_ubacujem)
    {
        nova_formula += it;
    }
    nova_formula += L')';
    formula = nova_formula;
}

int main()
{
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << L"Unesite predikatsku rečenicu:" << endl;
    wstring formula;
    wcin >> formula;
    uvedi_smenu_funkcija(formula);
    provuci_negaciju_kroz_kvantifikatore(formula);
    sredi_iskazne_formule_unutar_kvantifikatora(formula);
    uvedi_smenu_kvantifikatora(formula);
    uprosti_logicki_izraz(formula);
    vrati_smenu_kvantifikatora(formula);
    provuci_negaciju_kroz_kvantifikatore(formula);
    sredi_iskazne_formule_unutar_kvantifikatora(formula);
    vrati_smenu_funkcija(formula);
    smeni_ponavljajuce_promenljive_pored_kvantifikatora(formula);
    skolemizacija(formula);
    wcout << formula << endl;
    return 0;
}
