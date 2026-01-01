// ===============================
// Projet POO : Gestion d’un réseau social
// Niveau : 3IIR (simple, propre, conforme au cahier des charges)
// ===============================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;

// ===============================
// Classe Publication
// ===============================
class Publication {
private:
    string contenu;
    string date;

public:
    Publication(const string& contenu, const string& date)
        : contenu(contenu), date(date) {}

    friend ostream& operator<<(ostream& os, const Publication& p) {
        os << "[" << p.date << "] " << p.contenu;
        return os;
    }
};

// ===============================
// Classe Utilisateur
// ===============================
class Utilisateur {
protected:
    string nom;
    vector<string> amis;
    vector<Publication> publications;

public:
    Utilisateur(const string& nom) : nom(nom) {}

    const string& getNom() const {
        return nom;
    }

    Utilisateur& operator+=(const string& ami) {
        if (find(amis.begin(), amis.end(), ami) == amis.end()) {
            amis.push_back(ami);
        }
        return *this;
    }

    void ajouterPublication(const Publication& p) {
        publications.push_back(p);
    }

    virtual void afficher(ostream& os) const {
        os << "Utilisateur : " << nom << "\n";
        afficherAmisEtPublications(os);
    }

    friend ostream& operator<<(ostream& os, const Utilisateur& u) {
        u.afficher(os);
        return os;
    }

protected:
    void afficherAmisEtPublications(ostream& os) const {
        os << "  Amis : ";
        if (amis.empty()) {
            os << "Aucun";
        } else {
            for (const auto& ami : amis) {
                os << ami << " ";
            }
        }
        os << "\n";

        os << "  Publications :\n";
        if (publications.empty()) {
            os << "    Aucune publication\n";
        } else {
            for (const auto& p : publications) {
                os << "    - " << p << "\n";
            }
        }
    }
};

// ===============================
// Classe UtilisateurPremium
// ===============================
class UtilisateurPremium : public Utilisateur {
private:
    int maxAmis;

public:
    UtilisateurPremium(const string& nom, int maxAmis)
        : Utilisateur(nom), maxAmis(maxAmis) {}

    bool peutAjouterAmi() const {
        return amis.size() < static_cast<size_t>(maxAmis);
    }

    void ajouterAmiPremium(const string& ami) {
        if (peutAjouterAmi()) {
            *this += ami;
        } else {
            cout << "Limite d'amis atteinte pour " << nom << " !" << endl;
        }
    }

    void afficher(ostream& os) const override {
        os << "Utilisateur Premium : " << nom << "\n";
        os << "  Limite d'amis : " << maxAmis << "\n";
        afficherAmisEtPublications(os);
    }
};

// ===============================
// Classe ReseauSocial
// ===============================
class ReseauSocial {
private:
    vector<unique_ptr<Utilisateur>> utilisateurs;

public:
    void ajouterUtilisateur(unique_ptr<Utilisateur> u) {
        utilisateurs.push_back(move(u));
    }

    void afficherUtilisateurs() const {
        cout << "===== Utilisateurs =====\n";
        for (const auto& u : utilisateurs) {
            cout << *u << endl;
        }
    }
};

// ===============================
// Fonction main
// ===============================
int main() {
    ReseauSocial reseau;

    // Publications
    Publication p1("Bonjour tout le monde !", "01/01/2025");
    Publication p2("C++ c'est cool", "02/01/2025");
    Publication p3("Utilisateur premium ici", "03/01/2025");

    // Utilisateurs standards
    auto u1 = make_unique<Utilisateur>("Alice");
    auto u2 = make_unique<Utilisateur>("Bob");

    *u1 += "Bob";
    u1->ajouterPublication(p1);

    *u2 += "Alice";
    u2->ajouterPublication(p2);

    // Utilisateur premium
    auto up1 = make_unique<UtilisateurPremium>("Charlie", 2);
    up1->ajouterAmiPremium("Alice");
    up1->ajouterAmiPremium("Bob");
    up1->ajouterAmiPremium("David"); // dépassement
    up1->ajouterPublication(p3);

    // Ajout au réseau
    reseau.ajouterUtilisateur(move(u1));
    reseau.ajouterUtilisateur(move(u2));
    reseau.ajouterUtilisateur(move(up1));

    // Affichage
    reseau.afficherUtilisateurs();

    return 0;
}
