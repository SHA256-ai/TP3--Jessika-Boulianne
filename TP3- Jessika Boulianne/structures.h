#pragma once
#include <string>

using namespace std;

struct Article {
	string codeArticle = "";
	string description = "";
	float coutUnitaire = -1.0f;
	float prixDeVente = -1.0f;
	int qteStock = -1;
	int seuil = -1;
	int codeValidite = -1;
};

struct Client {
	string codeClient = "";
	string nomClient = "";
	string courriel = "";
	string typeClient = "";
	string dateAdhesion = "";
	char typeEscompte = ' ';
	int codeValidite = -1;
};

struct VenteArticle {
	string codeVente = "";
	string codeClient = "";
	string codeArticle = "";
	float prixVente = -1.0f;
	int qteAchetee = -1;
	float total = -1.0f;
	int codeValidite = -1;
};