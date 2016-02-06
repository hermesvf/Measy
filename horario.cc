#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
using namespace std;

typedef vector< pair<bool,int> > Horas; //int = posicio a possibilitats
typedef vector<Horas> Dias;

struct Moment {
  int dia;
  int hora_inici;
  int hora_fi;
};

struct Grup_assignatura {
  string nom; //Si son lab, llevarán "lab" al final. Ejemplo: SOlab
  int grup;
  vector<Moment> Moments;
  int posicio; //posició al vector Assignatures
};

void mostra_temporal(const vector<Grup_assignatura>& Possibilitats, const Dias& Hores_u) {
  for (int i = 0; i < Hores_u.size(); ++i) {
    if (i == 0) cout << "Dilluns: " << endl;
    else if (i == 1) cout << "Dimarts: " << endl;
    else if (i == 2) cout << "Dimecres:" << endl;
    else if (i == 3) cout << "Dijous:" << endl;
    else cout << "Divendres:" << endl;
    for (int j = 0; j < Hores_u[i].size(); ++j) {
      if (Hores_u[i][j].first) {
	cout << j+8 << "-" << j+9 << " " << Possibilitats[Hores_u[i][j].second].nom << " ";
	cout << Possibilitats[Hores_u[i][j].second].grup << endl;
      }
    }
    cout << endl;
  }
}

bool conflicte_horari(const vector<Moment>& Moments, const Dias& Hores_u) {
  int n_inici, n_fi, n_dia;
  for (int k = 0; k < Moments.size(); ++k) {
      n_inici = Moments[k].hora_inici-8;
      n_fi = Moments[k].hora_fi-8;
      n_dia = Moments[k].dia;
      for (int i = n_inici; i < n_fi; ++i) {
	if (Hores_u[n_dia][i].first) return true;
      }
  }
  return false;
}

void ocupa_tiempo(const vector<Moment>& Moments, int pos, Dias& Hores_u) {
  int n_inici, n_fi, n_dia;
  for (int k = 0; k < Moments.size(); ++k) {
    n_inici = Moments[k].hora_inici-8;
    n_fi = Moments[k].hora_fi-8;
    n_dia = Moments[k].dia;
    for (int i = n_inici; i < n_fi; ++i) {
      Hores_u[n_dia][i].first = true;
      Hores_u[n_dia][i].second = pos;
    }
  }
}

void libera_tiempo(const vector<Moment>& Moments, Dias& Hores_u) {
  int n_inici, n_fi, n_dia;
  for (int k = 0; k < Moments.size(); ++k) {
    n_inici = Moments[k].hora_inici-8;
    n_fi = Moments[k].hora_fi-8;
    n_dia = Moments[k].dia;
    for (int i = n_inici; i < n_fi; ++i) {
      Hores_u[n_dia][i].first = false;
    }
  }
}

bool saltos_una_hora(const Dias& Horas_u) {
  for (int i = 0; i < Horas_u.size(); ++i) {
    for (int j = 0; j < Horas_u[i].size()-2; ++j) {
      if (Horas_u[i][j].first and (not Horas_u[i][j+1].first) and Horas_u[i][j+2].first) return true;
    }
  }
  return false;
}

bool saltos_dos_horas(const Dias& Horas_u) {
  for (int i = 0; i < Horas_u.size(); ++i) {
    for (int j = 0; j < Horas_u[i].size()-3; ++j) {
      if (Horas_u[i][j].first and (not Horas_u[i][j+1].first) and (not Horas_u[i][j+2].first) and Horas_u[i][j+3].first) return true;
    }
  }
  return false;
}

bool saltos(const Dias& Horas_u) {
  return (saltos_una_hora(Horas_u) or saltos_dos_horas(Horas_u));
}

bool well_balanced(const Dias& Horas_u) {
  int cont4 = 0;
  int cont5 = 0;
  for (int i = 0; i < Horas_u.size(); ++i) {
    int cont_dia = 0;
    for (int j = 0; j < Horas_u[i].size(); ++j) {
      if (Horas_u[i][j].first) ++cont_dia;
    }
    if (cont_dia == 4) ++cont4;
    else if (cont_dia == 5) ++cont5;
    else return false;
  }
  return (cont4 == 4 and cont5 == 1);
}

bool quiero_dormir(const Dias& Horas_u) {
  for (int i = 0; i < Horas_u.size(); ++i) {
    if (not Horas_u[i][0].first and not Horas_u[i][1].first) return true;
  }
  return false;
}

bool mateixa_asig(const string& A, const string& B) {
  if (A.size() > B.size()) {
    for (int i = 0; i < B.size(); ++i) {
      if (A[i] != B[i]) return false;
    }
    return true;
  }
  else {
    for (int i = 0; i < A.size(); ++i) {
      if (A[i] != B[i]) return false;
    }
    return true;
  }
}

bool conflicte_subgrup(const vector<Grup_assignatura>& Eleccio, const Grup_assignatura& A, int k) {
  for (int i = 0; i < k; ++i) {
    if (mateixa_asig(A.nom,Eleccio[i].nom) and (A.grup)/10 != (Eleccio[i].grup)/10) return true;
  }
  return false;
}



void recursio(int k, const vector<Grup_assignatura>& Possibilitats, vector<Grup_assignatura>& Eleccio, vector<bool>& Ass_u, Dias& Hores_u,int& cont) {
  if (k == Eleccio.size()) {
    if (not saltos(Hores_u) and well_balanced(Hores_u) and quiero_dormir(Hores_u)) {
      ++cont;
      cout << "================== Combinació #" << cont << " =================" << endl;
      mostra_temporal(Possibilitats, Hores_u);
      if (cont == 50) exit(0);
    }
  }
  else {
    for (int i = 0; i < Possibilitats.size(); ++i) {
      if (not Ass_u[Possibilitats[i].posicio] and not conflicte_horari(Possibilitats[i].Moments,Hores_u) and not conflicte_subgrup(Eleccio,Possibilitats[i],k)) {
	Ass_u[Possibilitats[i].posicio] = true;
	ocupa_tiempo(Possibilitats[i].Moments,i,Hores_u);
	Eleccio[k] = Possibilitats[i];
	recursio(k+1,Possibilitats,Eleccio,Ass_u,Hores_u,cont);
	Ass_u[Possibilitats[i].posicio] = false;
	libera_tiempo(Possibilitats[i].Moments,Hores_u);
      }
    }
  }
}

int main() {
  int n_assignatures;
  cin >> n_assignatures;
  vector<string> Assignatures(n_assignatures);
  for (int i = 0; i < n_assignatures; ++i) { 
    cin >> Assignatures[i];
  }
  vector<Grup_assignatura> Possibilitats;
  string sigles;
  while (cin >> sigles) {
    Grup_assignatura A;
    A.nom = sigles;
    cin >> A.grup;
    vector<Moment> Moments_asig;
    string dia;
    while (cin >> dia and dia != "-") {
      Moment M;
      if (dia == "dilluns")        M.dia = 0;
      else if (dia == "dimarts")   M.dia = 1;
      else if (dia == "dimecres")  M.dia = 2;
      else if (dia == "dijous")    M.dia = 3;
      else if (dia == "divendres") M.dia = 4;
      int hora_inici,hora_final;
      cin >> M.hora_inici;
      cin >> M.hora_fi;
      Moments_asig.push_back(M);
    }
    A.Moments = Moments_asig;
    for (int j = 0; j < Assignatures.size(); ++j) if (Assignatures[j] == sigles) A.posicio = j;
    Possibilitats.push_back(A);
  }
  
  vector<Grup_assignatura> Eleccio(n_assignatures);
  vector<bool> Ass_u(n_assignatures,false);
  Dias Hores_u(5,Horas(7,make_pair(false,-1)));
  int cont = 0;
  recursio(0,Possibilitats,Eleccio,Ass_u,Hores_u,cont);
}