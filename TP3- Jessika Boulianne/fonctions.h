#pragma once
#include <Windows.h>
#include <string>
#include <iomanip>
#include <sstream>
#include "structures.h"

using namespace std;

void positionnerCurseur(int x, int y);
void afficherMenu(bool siEffacerEcran);
bool siChoixMenuValide(int choixMenu);
void afficherMsgErreur(string msg);
void ajoutArticle();
bool siCodeArticleValide(string codeArticle);
void ajoutClient();
bool siCoutUnitaireValide(float coutUnitaire);
bool siCodeClientValide(string codeClient);
void listeArticles();
void listeClients();
void rechercherArticle();
void rechercherClient();
void supprimerClient();
void modifierClient();
void achat();
void afficherMsgClignotant(string msg, int positionY);
bool getClient(string codeClientRecherche, Client& client);
bool getArticle(string codeArticleRecherche, Article& article);
void majStock(Article article);
void ajoutFichierFacture(VenteArticle facture);