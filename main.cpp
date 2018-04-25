#include <iostream>

using namespace std;

class Vector {
public:
    int *a;


    Vector(){

    }

    ~Vector(){
        delete[] a;
    }
};

class Nr_Natural_Mare {
public:
    int nr_cif;
    Vector V;

    Nr_Natural_Mare() {
        nr_cif = 0;
    }

    Nr_Natural_Mare(int x) {
        nr_cif = x;
        V.a = new int[x];
    }

    Nr_Natural_Mare(const Nr_Natural_Mare& N) {
        nr_cif = N.nr_cif;
        V.a = new int[nr_cif];
        for (int i = 0; i < nr_cif; i++)
            V.a[i] = N.V.a[i];
    }

    friend ostream & operator << (ostream &out, const Nr_Natural_Mare &N);
    friend istream & operator >> (istream &in,  Nr_Natural_Mare &N);
    Nr_Natural_Mare &operator= (const Nr_Natural_Mare&);
    Nr_Natural_Mare operator+ (const Nr_Natural_Mare&v) const;
    Nr_Natural_Mare operator- (const Nr_Natural_Mare&v) const;
    Nr_Natural_Mare operator* (const Nr_Natural_Mare&v) const;
    Nr_Natural_Mare operator/ (const Nr_Natural_Mare&v) const;
};

ostream & operator << (ostream &out, const Nr_Natural_Mare &N) {
    out << N.nr_cif << endl;
    for (int i = N.nr_cif - 1; i >= 0; i--)
        out << N.V.a[i];
    return out;
}

istream & operator >> (istream &in,  Nr_Natural_Mare &N) {
    if (N.nr_cif != 0) delete[] N.V.a;
    N.V.a = new int[N.nr_cif];
    in >> N.nr_cif;
    for (int i = N.nr_cif - 1; i >= 0; i--)
        in >> N.V.a[i];
    return in;
}

Nr_Natural_Mare &Nr_Natural_Mare::operator=(const Nr_Natural_Mare &v)  {
    if (this->nr_cif != 0) delete[] this->V.a;
    this->nr_cif = v.nr_cif;
    this->V.a = new int[v.nr_cif];
    for (int i = 0; i < v.nr_cif; i++)
        this->V.a[i] = v.V.a[i];
    return *this;
}

Nr_Natural_Mare Nr_Natural_Mare::operator+(const Nr_Natural_Mare& v) const{
    Nr_Natural_Mare Z;

    int lg = max(this->nr_cif, v.nr_cif);
    Z.V.a = new int[lg+1];
    Z.nr_cif = lg;
    for (int i = 0; i < lg; i++) {
        Z.V.a[i] = 0;
        if (i < this->nr_cif) Z.V.a[i] += this->V.a[i];
        if (i < v.nr_cif) Z.V.a[i] += v.V.a[i];
    }
    Z.V.a[lg] = 0;
    for (int i = 0; i < lg; i++)
        if (Z.V.a[i] > 9) {
            Z.V.a[i+1] += Z.V.a[i] / 10;
            Z.V.a[i] %= 10;
        }
    if (Z.V.a[lg] > 0)
        Z.nr_cif++;
    return Z;
}

Nr_Natural_Mare Nr_Natural_Mare::operator-(const Nr_Natural_Mare& v) const {
    Nr_Natural_Mare Z;
    if (v.nr_cif > this->nr_cif) throw "scad un nr mai mare dintr-un nr mai mic";
    if (v.nr_cif == this->nr_cif) {
        int egale = 1;
        for (int i = v.nr_cif - 1; i >= 0; i--)
            if (v.V.a[i] > this->V.a[i]) {
                egale = 0;
                throw "scad un nr mai mare dintr-un nr mai mic";
            }
            else if (v.V.a[i] < this->V.a[i]) {
                egale = 0;
                break;
            }
        if (egale) {
            Z.nr_cif = 1;
            Z.V.a = new int[1];
            Z.V.a[0] = 0;
            return Z;
        }
    }
    Z.nr_cif = this->nr_cif;
    Z.V.a = new int[Z.nr_cif];
    for (int i = 0; i < this->nr_cif; i++) {
        Z.V.a[i] = this->V.a[i];
    }

    for (int i = 0; i < v.nr_cif; i++) {
        Z.V.a[i] -= v.V.a[i];
        if (Z.V.a[i] < 0) {
            Z.V.a[i] += 10;
            Z.V.a[i+1]--;
        }
    }
    for (int i = nr_cif - 1; i >= 0; i--)
    if (Z.V.a[i] > 0) {
        Z.nr_cif = i + 1;
        break;
    }
    return Z;
}

Nr_Natural_Mare Nr_Natural_Mare::operator*(const Nr_Natural_Mare& N) const {
    Nr_Natural_Mare Z;
    Z.nr_cif = N.nr_cif * this->nr_cif + 1;
    Z.V.a = new int[Z.nr_cif];

    for (int i = 0; i < Z.nr_cif; i++)
        Z.V.a[i] = 0;

    for (int i = 0; i < this->nr_cif; i++)
        for (int j = 0; j < N.nr_cif; j++)
            Z.V.a[i+j] += this->V.a[i] * N.V.a[j];

    for (int i = 0; i < Z.nr_cif; i++)
        if (Z.V.a[i] > 9) {
            Z.V.a[i+1] += Z.V.a[i] / 10;
            Z.V.a[i] %= 10;
        }

    for (int i = Z.nr_cif - 1; i >= 0; i--)
    if (Z.V.a[i] > 0) {
        Z.nr_cif = i + 1;
        break;
    }

    return Z;
}

Nr_Natural_Mare Nr_Natural_Mare::operator/ (const Nr_Natural_Mare& N) const {
    Nr_Natural_Mare Z, T;
    Z.nr_cif = 1;
    Z.V.a = new int[1];
    Z.V.a[0] = 0;

    if (N.nr_cif == 1 && N.V.a[0] == 0) throw "IMPARTIRE LA 0";
    if (this->nr_cif < N.nr_cif) return Z;
    if (this->nr_cif == N.nr_cif) {
        int egale = 1;
        for (int i = N.nr_cif - 1; i >= 0; i--)
            if (N.V.a[i] > this->V.a[i])
                return Z;
            else if (N.V.a[i] < this->V.a[i]) {
                egale = 0;
                break;
            }
            if (egale) {
                Z.V.a[0] = 1;
                return Z;
            }
    }

    delete[] Z.V.a;
    Z.nr_cif = nr_cif;
    Z.V.a = new int[Z.nr_cif];
    T = *this;

    return Z;
}

class Numar_Intreg_Mare : Nr_Natural_Mare{
public:
    char semn;

    Numar_Intreg_Mare() {
        semn = '+';
    }

    Numar_Intreg_Mare(const Numar_Intreg_Mare& N) : Nr_Natural_Mare(N){
        semn = N.semn;
    }

    ~Numar_Intreg_Mare() {

    }

    friend class Vector_De_Numere_Intregi_Mari;
    friend ostream & operator << (ostream &out, const Numar_Intreg_Mare &N);
    friend istream & operator >> (istream &in,  Numar_Intreg_Mare &N);
    Numar_Intreg_Mare &operator= (const Numar_Intreg_Mare&);
    bool operator< (const Numar_Intreg_Mare&) const;
    bool operator== (const Numar_Intreg_Mare&) const;
    bool operator!= (const Numar_Intreg_Mare&) const;
    Numar_Intreg_Mare operator+ (const Numar_Intreg_Mare&v) const;
    Numar_Intreg_Mare operator- (const Numar_Intreg_Mare&v) const;
    Numar_Intreg_Mare operator* (const Numar_Intreg_Mare&v) const;
    Numar_Intreg_Mare operator/ (const Numar_Intreg_Mare&v) const;


    Numar_Intreg_Mare Max_Val_Abs(Numar_Intreg_Mare A) {
        if (A < *this) return *this;
        return A;
    }

    Numar_Intreg_Mare Partea_Intreaga_A_Radacinii_Patrate() {
        Numar_Intreg_Mare R;
        R.nr_cif = nr_cif;
        R.V.a = new int [R.nr_cif];
        R.V.a[0] = 1;
        Numar_Intreg_Mare Unu;
        Unu.nr_cif = 1;
        Unu.V.a = new int [1];
        Unu.V.a[0] = 1;
        Numar_Intreg_Mare Patrat;
        Patrat = R * R;
        while (Patrat < *this) {
            R = R + Unu;
            Patrat = R * R;
        }
        R = R - Unu;
        return R;
    }

};

ostream & operator << (ostream &out, const Numar_Intreg_Mare &N) {
    out << N.nr_cif << endl;
    if (N.semn == '-') out << N.semn;
    for (int i = N.nr_cif - 1; i >= 0; i--)
        out << N.V.a[i];
    return out;
}

istream & operator >> (istream &in,  Numar_Intreg_Mare &N) {
    if (N.nr_cif != 0) delete[] N.V.a;
    N.V.a = new int[N.nr_cif];
    in >> N.nr_cif >> N.semn;
    for (int i = N.nr_cif - 1; i >= 0; i--)
        in >> N.V.a[i];
    return in;
}

Numar_Intreg_Mare &Numar_Intreg_Mare::operator= (const Numar_Intreg_Mare &v)  {
    if (this->nr_cif != 0) delete[] this->V.a;
    this->nr_cif = v.nr_cif;
    this->semn = v.semn;
    this->V.a = new int[v.nr_cif];
    for (int i = 0; i < v.nr_cif; i++)
        this->V.a[i] = v.V.a[i];
    return *this;
}

Numar_Intreg_Mare Numar_Intreg_Mare::operator+ (const Numar_Intreg_Mare& v) const {
    if (semn == v.semn) {
        Numar_Intreg_Mare Z;
        int lg = max(this->nr_cif, v.nr_cif);
        Z.V.a = new int[lg+1];
        Z.nr_cif = lg;
        for (int i = 0; i < lg; i++) {
            Z.V.a[i] = 0;
            if (i < this->nr_cif) Z.V.a[i] += this->V.a[i];
            if (i < v.nr_cif) Z.V.a[i] += v.V.a[i];
        }
        Z.V.a[lg] = 0;
        for (int i = 0; i < lg; i++)
            if (Z.V.a[i] > 9) {
                Z.V.a[i+1] += Z.V.a[i] / 10;
                Z.V.a[i] %= 10;
            }
        if (Z.V.a[lg] > 0)
            Z.nr_cif++;
        Z.semn = semn;
        return Z;
    }
    else {
        if (*this == v) {
            Numar_Intreg_Mare Z;
            Z.nr_cif = 1;
            Z.V.a = new int[1];
            Z.semn = '+';
            Z.V.a[0] = 0;
            return Z;
        }
        if (*this < v) {
            Numar_Intreg_Mare Z;
            Z.semn = v.semn;
            Z.nr_cif = v.nr_cif;
            Z.V.a = new int[Z.nr_cif];
            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] = v.V.a[i];
            }

            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] -= this->V.a[i];
                if (Z.V.a[i] < 0) {
                    Z.V.a[i] += 10;
                    Z.V.a[i+1]--;
                }
            }
            for (int i = nr_cif - 1; i >= 0; i--)
            if (Z.V.a[i] > 0) {
                Z.nr_cif = i + 1;
                break;
            }
            return Z;
        }
        else {
            Numar_Intreg_Mare Z;
            Z.semn = semn;
            Z.nr_cif = v.nr_cif;
            Z.V.a = new int[Z.nr_cif];
            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] = this->V.a[i];
            }

            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] -= v.V.a[i];
                if (Z.V.a[i] < 0) {
                    Z.V.a[i] += 10;
                    Z.V.a[i+1]--;
                }
            }
            for (int i = nr_cif - 1; i >= 0; i--)
            if (Z.V.a[i] > 0) {
                Z.nr_cif = i + 1;
                break;
            }
            return Z;
        }
    }
}

Numar_Intreg_Mare Numar_Intreg_Mare::operator- (const Numar_Intreg_Mare& v) const {
    if (semn != v.semn) {
        Numar_Intreg_Mare Z;
        int lg = max(this->nr_cif, v.nr_cif);
        Z.V.a = new int[lg+1];
        Z.nr_cif = lg;
        for (int i = 0; i < lg; i++) {
            Z.V.a[i] = 0;
            if (i < this->nr_cif) Z.V.a[i] += this->V.a[i];
            if (i < v.nr_cif) Z.V.a[i] += v.V.a[i];
        }
        Z.V.a[lg] = 0;
        for (int i = 0; i < lg; i++)
            if (Z.V.a[i] > 9) {
                Z.V.a[i+1] += Z.V.a[i] / 10;
                Z.V.a[i] %= 10;
            }
        if (Z.V.a[lg] > 0)
            Z.nr_cif++;
        if (*this < v) Z.semn = v.semn;
        else Z.semn = semn;
        return Z;
    }
    else {
        if (*this == v) {
            Numar_Intreg_Mare Z;
            Z.nr_cif = 1;
            Z.V.a = new int[1];
            Z.semn = '+';
            Z.V.a[0] = 0;
            return Z;
        }
        if (*this < v) {
            Numar_Intreg_Mare Z;
            Z.semn = v.semn;
            Z.nr_cif = v.nr_cif;
            Z.V.a = new int[Z.nr_cif];
            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] = v.V.a[i];
            }

            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] -= this->V.a[i];
                if (Z.V.a[i] < 0) {
                    Z.V.a[i] += 10;
                    Z.V.a[i+1]--;
                }
            }
            for (int i = nr_cif - 1; i >= 0; i--)
            if (Z.V.a[i] > 0) {
                Z.nr_cif = i + 1;
                break;
            }
            return Z;
        }
        else {
            Numar_Intreg_Mare Z;
            Z.semn = semn;
            Z.nr_cif = v.nr_cif;
            Z.V.a = new int[Z.nr_cif];
            for (int i = 0; i < this->nr_cif; i++) {
                Z.V.a[i] = this->V.a[i];
            }

            for (int i = 0; i < v.nr_cif; i++) {
                Z.V.a[i] -= v.V.a[i];
                if (Z.V.a[i] < 0) {
                    Z.V.a[i] += 10;
                    Z.V.a[i+1]--;
                }
            }
            for (int i = nr_cif - 1; i >= 0; i--)
            if (Z.V.a[i] > 0) {
                Z.nr_cif = i + 1;
                break;
            }
            return Z;
        }
    }


}

Numar_Intreg_Mare Numar_Intreg_Mare::operator* (const Numar_Intreg_Mare& N) const {
    Numar_Intreg_Mare Z;
    Z.nr_cif = N.nr_cif * this->nr_cif + 1;
    Z.V.a = new int[Z.nr_cif];

    for (int i = 0; i < Z.nr_cif; i++)
        Z.V.a[i] = 0;

    for (int i = 0; i < this->nr_cif; i++)
        for (int j = 0; j < N.nr_cif; j++)
            Z.V.a[i+j] += this->V.a[i] * N.V.a[j];

    for (int i = 0; i < Z.nr_cif; i++)
        if (Z.V.a[i] > 9) {
            Z.V.a[i+1] += Z.V.a[i] / 10;
            Z.V.a[i] %= 10;
        }

    for (int i = Z.nr_cif - 1; i >= 0; i--)
    if (Z.V.a[i] > 0) {
        Z.nr_cif = i + 1;
        break;
    }

    if (semn == N.semn) Z.semn = '+';
    else Z.semn = '-';
    return Z;
}

bool Numar_Intreg_Mare::operator< (const Numar_Intreg_Mare& c) const {
    if (this->nr_cif < c.nr_cif) return 1;
    if (this->nr_cif > c.nr_cif) return 0;
    for (int i = nr_cif - 1; i >= 0; i--)
        if (c.V.a[i] > this->V.a[i]) return 1;
        else if (c.V.a[i] < this->V.a[i]) return 0;
    return 0;
}

bool Numar_Intreg_Mare::operator== (const Numar_Intreg_Mare& c) const {
    if (this->nr_cif != c.nr_cif) return 0;
    for (int i = 0; i < nr_cif; i++)
        if (V.a[i] != c.V.a[i])
            return 0;
    return 1;
}

bool Numar_Intreg_Mare::operator!= (const Numar_Intreg_Mare& c) const {
    if (this->nr_cif != c.nr_cif) return 1;
    for (int i = 0; i < nr_cif; i++)
        if (V.a[i] != c.V.a[i])
            return 1;
    return 0;
}

class Vector_De_Numere_Intregi_Mari {
public:
    int lg;
    Numar_Intreg_Mare *v;

    Vector_De_Numere_Intregi_Mari() {
        lg = 0;
    }

    ~Vector_De_Numere_Intregi_Mari(){
        delete[] v;
    }

    friend ostream & operator << (ostream &out, const Vector_De_Numere_Intregi_Mari &N);
    friend istream & operator >> (istream &in,  Vector_De_Numere_Intregi_Mari &N);
    Numar_Intreg_Mare operator* (const Vector_De_Numere_Intregi_Mari &N);
    Vector_De_Numere_Intregi_Mari &operator= (const Vector_De_Numere_Intregi_Mari &N);

    Numar_Intreg_Mare Val_Max_Din_Vector() {
        Numar_Intreg_Mare Maxi;
        Maxi = this->v[0];
        for (int i = 1; i < lg; i++)
            Maxi = Maxi.Max_Val_Abs(v[i]);
        return Maxi;
    }
};

ostream & operator << (ostream &out, const Vector_De_Numere_Intregi_Mari &N) {
    out << N.lg << '\n';
    for (int i = 0; i < N.lg; i++)
        out << N.v[i] << '\n';
    return out;
}

istream & operator >> (istream &in, Vector_De_Numere_Intregi_Mari &N) {
    in >> N.lg;
    N.v = new Numar_Intreg_Mare[N.lg];
    for (int i = 0; i < N.lg; i++)
        in >> N.v[i];
    return in;
}

Numar_Intreg_Mare Vector_De_Numere_Intregi_Mari::operator*(const Vector_De_Numere_Intregi_Mari &N) {
    Numar_Intreg_Mare Z;
    Z.nr_cif = 1;
    Z.V.a = new int [1];
    Z.V.a[0] = 0;

    for (int i = 0; i < N.lg; i++)
        Z = Z + this->v[i] * N.v[i];

    return Z;
}

Vector_De_Numere_Intregi_Mari &Vector_De_Numere_Intregi_Mari::operator=(const Vector_De_Numere_Intregi_Mari &N) {
    if (lg > 0) delete[] v;
    lg = N.lg;
    v = new Numar_Intreg_Mare[lg];
    for (int i = 0; i < lg; i++)
        v[i] = N.v[i];
    return *this;
}

int main()
{
    //Nr_Natural_Mare A;
    //cin >> A;
    //Nr_Natural_Mare B;
    //B = A;
    //cout << B << '\n';
    //B = B + A;
    //cout << B << '\n';
    //Nr_Natural_Mare C;
    //C = B - A;
    //cout << C << '\n';
    //B = A * A;
    //cout << B << '\n';
    //Numar_Intreg_Mare N;
    //cin >> N;
    //cout << N << '\n';
    //Numar_Intreg_Mare M;
    //M = N;
    //cout << M << '\n';
    //Numar_Intreg_Mare P(M);
    //cout << P << '\n';
    //cin >> N;
    //M = M - N;
    //cout << M << '\n';
    //M = M * M;
    //cout << M << '\n';
    //Numar_Intreg_Mare Q;
    //cin >> Q;
    //cout << Q.Partea_Intreaga_A_Radacinii_Patrate() << '\n';

    Vector_De_Numere_Intregi_Mari V;
    cin >> V;
    //cout << V;

    Vector_De_Numere_Intregi_Mari W;
    W = V;
    cout << W;
    return 0;
}
