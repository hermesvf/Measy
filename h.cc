#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <string>
using namespace std;

typedef vector< pair<bool,int> > Horas; //int = posicio a possibilitats
typedef vector<Horas> Dias;

struct Moment {
  int dia;
  int hora_inici;
  int hora_fi;
  string aula;
};

struct Grup_assignatura {
  string nom;
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

void recursio(int k, const vector<Grup_assignatura>& Possibilitats, vector<Grup_assignatura>& Eleccio, vector<bool>& Ass_u, Dias& Hores_u, int& cont) {
  if (k == Eleccio.size()) {
    if (not saltos(Hores_u) or well_balanced(Hores_u) or quiero_dormir(Hores_u)) { 
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

string reanomena(const string& siglesAssig, char tipus) {
  int i = 5;
  vector<char> nouNom;
  while (i < siglesAssig.size()) {
    nouNom.push_back(siglesAssig[i]);
  }
  if (tipus != 'T') {
    nouNom.push_back('l');
    nouNom.push_back('a');
    nouNom.push_back('b');
  }
  return string(nouNom.begin(),nouNom.end());
}

bool asignaturaRegistrada(string nom, const vector<string>& Ass) {
  for (int i = 0; i < Ass.size(); ++i) if (Ass[i] == nom) return true;
  return false;
}

pair<bool,int> grupoEnPossibilitats(string nom, int grup, const vector<Grup_assignatura>& Possibilitats) {
  for (int i = 0; i < Possibilitats.size(); ++i) {
    if (Possibilitats[i].nom == nom and Possibilitats[i].grup == grup) return make_pair(true,i);
  }
  return make_pair(false,-1);
}

int omple(vector<string>& Assignatures, vector<Grup_assignatura>& Possibilitats) {
  int contadorAssignatures = 0; //Cuenta cuantas hay, diferenciando entre teoria y (lab,prob)
  string siglesAssig;
  while (cin >> siglesAssig) {
    string nom;
    int grup, dia;
    cin >> grup >> dia;
    string horaTxt;
    cin >> horaTxt;
    int hora = horaTxt[1] -'0'+ (horaTxt[0]-'0')*10;
    char tipo;
    cin >> tipo;
    string aula;
    cin >> aula;
    nom = reanomena(siglesAssig,tipo);
    if (not asignaturaRegistrada(nom,Assignatures)) {
      Assignatures.push_back(nom);
      ++contadorAssignatures;
      Grup_assignatura G;
      G.nom = nom;
      G.grup = grup;
      vector<Moment> VM;
      Moment M;
      M.dia = dia-1;
      M.hora_inici = hora;
      M.hora_fi = hora+1;
      M.aula = aula;
      VM.push_back(M);
      G.posicio = Assignatures.size()-1; //Porque acabo de hacer push_back en Assignatures
      G.Moments = VM;
      Possibilitats.push_back(G);
    }
    else { // La asignatura ya esta registrada en Assignatures
      pair<bool,int> grupEnPos = grupoEnPossibilitats(nom,grup,Possibilitats);
      if (not grupEnPos.first) {
        Grup_assignatura Gr;
        Gr.nom = nom;
        Gr.grup = grup;
        vector<Moment> VM1;
        Moment M1;
        M1.dia = dia-1;
        M1.hora_inici = hora;
        M1.hora_fi = hora+1;
        M1.aula = aula;
        VM1.push_back(M1);
        Gr.Moments = VM1;
        for (int j = 0; j < Assignatures.size(); ++j) if (Assignatures[j] == nom) Gr.posicio = j;
        Possibilitats.push_back(Gr);
      }
      else { //El grupo esta en possibilitats, hay que añadir la hora de la entrada current
        Moment M2;
        M2.dia = dia-1;
        M2.hora_inici = hora;
        M2.hora_fi = hora+1;
        M2.aula = aula;
        Possibilitats[grupEnPos.second].Moments.push_back(M2);
      }
    }
  }
  return contadorAssignatures;
}

int main() {
  vector<string> Assignatures;
  vector<Grup_assignatura> Possibilitats;
  int n_assignatures = omple(Assignatures,Possibilitats);
  cout << n_assignatures << endl;
  vector<Grup_assignatura> Eleccio(n_assignatures);
  vector<bool> Ass_u(n_assignatures,false);
  Dias Hores_u(5,Horas(13,make_pair(false,-1))); //5 dias lectivos, 13 huecos cada dia (8:00-21:00)
  int cont = 0;
  cout << "hi" << endl;
  recursio(0,Possibilitats,Eleccio,Ass_u,Hores_u,cont);
}