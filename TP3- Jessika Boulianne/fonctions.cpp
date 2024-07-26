#include "fonctions.h"
#include <iostream>
#include "structures.h"
#include <regex>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

void positionnerCurseur(int x, int y)
{
	HANDLE manipConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordonnees;
	coordonnees.X = x;
	coordonnees.Y = y;
	SetConsoleCursorPosition(manipConsole, coordonnees);
}

void afficherMenu(bool siEffacerEcran)
{
	if (siEffacerEcran)
	{
		system("cls");
	}

	positionnerCurseur(20, 3);
	cout << "Menu principal";
	positionnerCurseur(25, 5);
	cout << "1 - Ajout d'un article en inventaire";
	positionnerCurseur(25, 7);
	cout << "2 - Ajout d'un nouveau client";
	positionnerCurseur(25, 9);
	cout << "3 - Liste des articles de l'inventaire";
	positionnerCurseur(25, 11);
	cout << "4 - Liste des clients";
	positionnerCurseur(25, 13);
	cout << "5 - Rechercher un article dans l'inventaire";
	positionnerCurseur(25, 15);
	cout << "6 - Rechercher un client";
	positionnerCurseur(25, 17);
	cout << "7 - Suppression d'un client";
	positionnerCurseur(25, 19);
	cout << "8 - Modifier le type d'escompte pour un client";
	positionnerCurseur(25, 21);
	cout << "9 - Achat (creer une facture)";
	positionnerCurseur(25, 27);
	cout << "99 - Pour terminer";
	positionnerCurseur(20, 30);
	cout << "Votre choix: ";

}

bool siChoixMenuValide(int choixMenu)
{
	bool siValid;
	switch (choixMenu)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 99:
		siValid = true;
		break;
	default:
		siValid = false;
		break;
	}
	return siValid;
}

void afficherMsgErreur(string msg)
{
	system("cls");
	HANDLE manipConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(manipConsole, 12);
	positionnerCurseur(1, 1);
	cout << msg;
	SetConsoleTextAttribute(manipConsole, 7);
}

void ajoutArticle()
{
	Article article;
	bool isValid_codeArticle = false;
	bool isValid_coutUnitaire = false;
	string msgErreur = "";

	do
	{
		system("cls");
		if (!isValid_codeArticle || !isValid_coutUnitaire)
		{
			afficherMsgErreur(msgErreur);
		}

		//Saisie des données
		positionnerCurseur(15, 7);
		cout << "Code de l'article : ";
		if (article.codeArticle == "" || !isValid_codeArticle)
		{
			cin >> article.codeArticle;
			isValid_codeArticle = siCodeArticleValide(article.codeArticle);
			if (!isValid_codeArticle)
			{
				afficherMsgErreur("ERREUR - Le code de l'article (" + article.codeArticle + ") n'est pas valide.");
			}
		}
		else {
			cout << article.codeArticle;
		}

		if (isValid_codeArticle)
		{
			//DESCRIPTION
			positionnerCurseur(15, 9);
			cout << "Description : ";
			if (article.description == "")
			{
				cin.ignore();
				getline(cin, article.description);
			}
			else {
				cout << article.description;
			}


			//COUT UNITAIRE
			positionnerCurseur(15, 11);
			cout << "Cout unitaire : ";

			if (article.coutUnitaire < 1.0f || !isValid_coutUnitaire)
			{
				cin >> article.coutUnitaire;
				isValid_coutUnitaire = siCoutUnitaireValide(article.coutUnitaire);
				if (!isValid_coutUnitaire)
				{
					msgErreur = "ERREUR - Le cout unitaire (" + to_string(article.coutUnitaire) + ") n'est pas valide.";

				}
			}
			//PRIX DE VENTE
			article.prixDeVente = article.coutUnitaire * 1.4f;

			//QUANTITÉ EN STOCK
			positionnerCurseur(15, 13);
			cout << "Quantite en stock : ";
			cin >> article.qteStock;

			//SEUIL DE COMMANDE
			positionnerCurseur(15, 15);
			cout << "Seuil de commande: ";
			cin >> article.seuil;

			//CODE DE VALIDITÉ
			if (isValid_codeArticle && isValid_coutUnitaire)
			{
				article.codeValidite = 1;
			}
		}
		if (isValid_codeArticle && isValid_coutUnitaire)
		{
			positionnerCurseur(15, 20);
			cout << "Voici les informations de l'article : "
				<< article.codeArticle << " | "
				<< article.description << " | "
				<< article.coutUnitaire << " | "
				<< article.prixDeVente << " | "
				<< article.qteStock << " | "
				<< article.seuil
				<< endl << endl;

			positionnerCurseur(15, 22);
			string reponse = "";
			cout << "Voulez-vous ajouter ces informations dans le fichier ? : ";
			cin >> reponse;
			regex regexOui("[oO]|oui|Oui|OUI");
			if (regex_match(reponse, regexOui))
			{
				//Ouverture du fichier inventaire.dat en mode append
				ofstream fichierInventaire("inventaire.dat", ios::app);

				//ECRITURE CSV --> Séparateur = ; (donc espace POSSIBLE)
				fichierInventaire << article.codeArticle << ";"
					<< article.description << ";"
					<< article.coutUnitaire << ";"
					<< article.prixDeVente << ";"
					<< article.qteStock << ";"
					<< article.seuil << ";"
					<< article.codeValidite << endl;

				//Fermeture du fichier
				fichierInventaire.close();

				positionnerCurseur(15, 25);
				system("pause");
			}
		}
	} while (!isValid_codeArticle || !isValid_coutUnitaire);

}

bool siCodeArticleValide(string codeArticle)
{
	bool isValid = false;
	regex formatCodeArticle("[a-zA-Z][0-9]{3}");

	if (regex_match(codeArticle, formatCodeArticle))
	{
		string nombre = codeArticle.substr(1, 3);
		int codeArticleInt = stoi(nombre);
		if (codeArticleInt >= 301 && codeArticleInt <= 999)
		{
			isValid = true;
		}
	}

	return isValid;
}

bool siCodeClientValide(string codeClient)
{
	bool isValid = false;
	regex formatCodeClient("[a-z]{2}[0-9]{4}");

	if (regex_match(codeClient, formatCodeClient))
	{
		string nombre = codeClient.substr(2, 5);
		int codeClientInt = stoi(nombre);
		if (codeClientInt >= 0)
		{
			isValid = true;
		}
	}

	return isValid;
}

void ajoutClient()
{
	Client client;
	bool isValid_codeClient = true;
	bool isValid_courriel = true;
	string lectureClavier = "";
	regex regexOui("[oO] | oui | Oui | OUI");
	string msgErreur = "";

	do
	{
		system("cls");
		if (!isValid_codeClient || !isValid_courriel)
		{
			afficherMsgErreur(msgErreur);
		}

		//Saisie des données
		positionnerCurseur(15, 7);
		cout << "Code du client : ";
		if (client.codeClient == "" || !isValid_codeClient)
		{
			cin >> client.codeClient;
			isValid_codeClient = siCodeClientValide(client.codeClient);
			if (!isValid_codeClient)
			{
				msgErreur = "ERREUR - Le code du client (" + client.codeClient + ") n'est pas valide.";
			}
		}
		else {
			cout << client.codeClient;
		}

		if (isValid_codeClient)
		{
			//NOM DU CLIENT

			do
			{
				positionnerCurseur(15, 9);
				cout << "Nom du client : ";
				if (client.nomClient == "")
				{
					cin.ignore();
					getline(cin, client.nomClient);
				}
				else {
					cout << client.nomClient;
				}
			} while (client.nomClient == "");



			//COURRIEL DE CLIENT
			regex regexCourriel("[a-z]+[0-9a-z.\\-_]*[0-9a-z]+@[a-z]+[0-9a-z]*(.[a-z]{2,})+");
			string courriel;
			bool lectureValide = false;
			do
			{
				positionnerCurseur(15, 11);
				cout << "Courriel du client : ";
				cin >> courriel;
				if (regex_match(courriel, regexCourriel))
				{
					lectureValide = true;
					client.courriel = courriel;
				}
				else
				{
					cout << "ERREUR - Le type de client (" + courriel + ") n'est pas valide." << endl;
					cout << "Veuillez recommencer" << endl;
					system("pause");
					system("cls");
				}
			} while (!lectureValide);



			//TYPE DE CLIENT
			regex regexTypeClient("Bronze|Argent|Or");
			string typeClient;

			lectureValide = false;
			do
			{
				positionnerCurseur(15, 13);
				cout << "Type de client : ";
				cin >> typeClient;
				if (regex_match(typeClient, regexTypeClient))
				{
					lectureValide = true;
					client.typeClient = typeClient;
				}
				else
				{
					cout << "ERREUR - Le type de client (" + client.typeClient + ") n'est pas valide.";
					cout << "Veuillez recommencer" << endl;
					system("pause");
					system("cls");
				}
			} while (!lectureValide);


			//DATE D'ADHÉSION
			string date;
			regex regexDate("[1-9][0-9][0-9]{2}-([0][1-9]|[1][0-2])-([1-2][0-9]|[0][1-9]|[3][0-1])");
			lectureValide = false;
			do
			{
				positionnerCurseur(15, 15);
				cout << "Date adhesion : ";
				cin >> date;
				cout << endl;
				if (regex_match(date, regexDate))
				{
					lectureValide = true;
					client.dateAdhesion = date;
				}
				else
				{
					cout << "ERREUR - La date (" + client.dateAdhesion + ") n'est pas valide." << endl;
					cout << "Veuillez recommencer" << endl;
					system("pause");
					system("cls");
				}
			} while (!lectureValide);


			//TYPE D'ESCOMPTE

			string typeEscompte;
			regex regex123("1|2|3");
			lectureValide = false;
			do
			{
				positionnerCurseur(15, 17);
				cout << "Type escompte : ";
				cin >> typeEscompte;
				if (regex_match(typeEscompte, regex123))
				{
					lectureValide = true;
					client.typeEscompte = typeEscompte[0];
				}
				else
				{
					cout << "ERREUR - Le type d'escompte (" << client.typeEscompte << ") n'est pas valide.";
					cout << "Veuillez recommencer." << endl;
					system("pause");
					system("cls");
				}
			} while (!lectureValide);




			//CODE DE VALIDITÉ
			if (isValid_codeClient && isValid_courriel)
			{
				client.codeValidite = 1;
			}
		}
		if (isValid_codeClient && isValid_courriel)
		{
			positionnerCurseur(15, 20);
			cout << "Voici les informations du client : "
				<< client.codeClient << " | "
				<< client.nomClient << " | "
				<< client.courriel << " | "
				<< client.typeClient << " | "
				<< client.dateAdhesion << " | "
				<< client.typeEscompte << " | "
				<< client.codeValidite
				<< endl << endl;

			positionnerCurseur(15, 22);
			string reponse = "";
			cout << "Voulez-vous ajouter ces informations dans le fichier ? : ";
			cin >> reponse;
			regex regexOui("[oO]|oui|Oui|OUI");
			if (regex_match(reponse, regexOui))
			{
				//Ouverture du fichier inventaire.dat en mode append
				ofstream fichierClient("client.dat", ios::app);

				//ECRITURE CSV --> Séparateur = ; (donc espace POSSIBLE)
				fichierClient << client.codeClient << ";"
					<< client.nomClient << ";"
					<< client.courriel << ";"
					<< client.typeClient << ";"
					<< client.dateAdhesion << ";"
					<< client.typeEscompte << ";"
					<< client.codeValidite << endl;

				//Fermeture du fichier
				fichierClient.close();

				positionnerCurseur(15, 25);
				system("pause");
			}
		}
	} while (!isValid_codeClient || !isValid_courriel);

}

bool siCoutUnitaireValide(float coutUnitaire)
{
	bool isValid = false;

	if (coutUnitaire >= 1.0f)
	{
		isValid = true;
	}
	return isValid;
}

void listeArticles()
{
	system("cls");
	Article article;
	int positionX = 10;
	int positionY = 7;
	string nomFichier = "inventaire.dat";
	ifstream fichierLecture(nomFichier, ios::in);

	if (fichierLecture)
	{
		positionnerCurseur(positionX, positionY);
		cout << "Le contenu du fichier INVENTAIRE";

		positionY += 2;
		positionnerCurseur(positionX, positionY);

		cout << setw(5) << "Code"
			<< setw(30) << "Description"
			<< setw(10) << "C.U."
			<< setw(10) << "Prix"
			<< setw(8) << "Stock"
			<< setw(8) << "Seuil"
			<< setw(10) << "Valide";

		positionY++;
		positionnerCurseur(positionX, positionY);

		cout << setw(5) << "----"
			<< setw(30) << "----------"
			<< setw(10) << "----"
			<< setw(10) << "----"
			<< setw(8) << "-----"
			<< setw(8) << "------"
			<< setw(10) << "------";

		positionY++;
		string ligneLue;
		string champLu;
		stringstream ligneStream;



		do
		{
			getline(fichierLecture, ligneLue); //Extraire une ligne du fichier vers -ligneLue-
			if (!fichierLecture.eof())
			{
				ligneStream << ligneLue; // Convertir la ligne lue vers un flux

				getline(ligneStream, article.codeArticle, ';');
				getline(ligneStream, article.description, ';');
				getline(ligneStream, champLu, ';');
				article.coutUnitaire = stof(champLu);
				getline(ligneStream, champLu, ';');
				article.prixDeVente = stof(champLu);
				getline(ligneStream, champLu, ';');
				article.qteStock = stoi(champLu);
				getline(ligneStream, champLu, ';');
				article.seuil = stoi(champLu);
				getline(ligneStream, champLu, ';');
				article.codeValidite = stoi(champLu);
				ligneStream.clear();


				positionnerCurseur(positionX, positionY);
				cout << setw(5) << article.codeArticle
					<< setw(30) << article.description
					<< setw(10) << article.coutUnitaire
					<< setw(10) << article.prixDeVente
					<< setw(6) << article.qteStock
					<< setw(8) << article.seuil
					<< setw(10) << article.codeValidite;
				positionY++;
			}
		} while (!fichierLecture.eof());

		fichierLecture.close();

	}
	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	positionnerCurseur(positionX, positionY + 2);
	system("pause");
}

void listeClients()
{
	system("cls");
	Client client;
	int positionX = 10;
	int positionY = 7;
	string nomFichier = "client.dat";
	ifstream fichierLecture(nomFichier, ios::in);

	if (fichierLecture)
	{
		positionnerCurseur(positionX, positionY);
		cout << "Le contenu du fichier CLIENT";

		positionY += 2;
		positionnerCurseur(positionX, positionY);

		cout << setw(5) << "Code"
			<< setw(30) << "Nom client"
			<< setw(15) << "Courriel"
			<< setw(15) << "Type client"
			<< setw(20) << "Date adhesion"
			<< setw(10) << "Escompte"
			<< setw(10) << "Validite";

		positionY++;
		positionnerCurseur(positionX, positionY);

		cout << setw(5) << "----"
			<< setw(30) << "--------"
			<< setw(15) << "-------"
			<< setw(15) << "--------"
			<< setw(20) << "-----------"
			<< setw(10) << "-------"
			<< setw(10) << "-------";

		positionY++;
		string ligneLue;
		string champLu;
		stringstream ligneStream;

		do
		{
			getline(fichierLecture, ligneLue); //Extraire une ligne du fichier vers -ligneLue-
			if (!fichierLecture.eof())
			{
				ligneStream << ligneLue; // Convertir la ligne lue vers un flux

				getline(ligneStream, client.codeClient, ';');
				getline(ligneStream, client.nomClient, ';');
				getline(ligneStream, client.courriel, ';');
				getline(ligneStream, client.typeClient, ';');
				getline(ligneStream, client.dateAdhesion, ';');
				getline(ligneStream, champLu, ';');
				client.typeEscompte = champLu[0];
				getline(ligneStream, champLu, ';');
				client.codeValidite = stoi(champLu);

				ligneStream.clear();

				if (client.codeValidite == 1)
				{
					positionnerCurseur(positionX, positionY);
					cout << setw(5) << client.codeClient
						<< setw(30) << client.nomClient
						<< setw(15) << client.courriel
						<< setw(10) << client.typeClient
						<< setw(20) << client.dateAdhesion
						<< setw(10) << client.typeEscompte
						<< setw(10) << client.codeValidite;
					positionY++;
				}

			}
		} while (!fichierLecture.eof());

		fichierLecture.close();


	}
	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	positionnerCurseur(positionX, positionY + 2);
	system("pause");

}

void rechercherArticle()
{
	system("cls");
	int positionX = 10;
	int positionY = 7;
	Article article;
	string nomFichier = "inventaire.dat";
	ifstream fichierLecture(nomFichier, ios::in);

	if (fichierLecture) // Si "FichierLecture" contient quelque chose --> Ouverture réussi.
	{
		string choixCodeRecherche = "";
		bool isSuccess = false;

		positionnerCurseur(42, 5);
		cout << "RECHERCHE ARTICLE";
		positionnerCurseur(42, 6);
		cout << "=================";
		positionnerCurseur(10, 9);
		cout << "Entrez le code de l'article cible : ";
		cin.ignore();
		getline(cin, choixCodeRecherche);

		string ligneLue;
		string champLu;
		stringstream ligneStream;

		do
		{
			getline(fichierLecture, ligneLue);
			if (!fichierLecture.eof())
			{

				ligneStream << ligneLue;
				getline(ligneStream, article.codeArticle, ';');

				if (choixCodeRecherche == article.codeArticle)
				{
					isSuccess = true;

					positionY += 2;
					positionnerCurseur(positionX, positionY);
					cout << setw(5) << "Code"
						<< setw(30) << "Description"
						<< setw(10) << "C.U."
						<< setw(10) << "Prix"
						<< setw(8) << "Stock"
						<< setw(8) << "Seuil"
						<< setw(10) << "Valide";

					positionY++;
					positionnerCurseur(positionX, positionY);
					cout << setw(5) << "----"
						<< setw(30) << "----------"
						<< setw(10) << "----"
						<< setw(10) << "----"
						<< setw(8) << "-----"
						<< setw(8) << "------"
						<< setw(10) << "------";
					positionY++;


					getline(ligneStream, article.description, ';');
					getline(ligneStream, champLu, ';');
					article.coutUnitaire = stof(champLu);
					getline(ligneStream, champLu, ';');
					article.prixDeVente = stof(champLu);
					getline(ligneStream, champLu, ';');
					article.qteStock = stoi(champLu);
					getline(ligneStream, champLu, ';');
					article.seuil = stoi(champLu);
					getline(ligneStream, champLu, ';');
					article.codeValidite = stoi(champLu);
					ligneStream.clear();

					positionnerCurseur(positionX, positionY);
					cout << setw(5) << article.codeArticle
						<< setw(30) << article.description
						<< setw(10) << article.coutUnitaire
						<< setw(10) << article.prixDeVente
						<< setw(6) << article.qteStock
						<< setw(8) << article.seuil
						<< setw(10) << article.codeValidite;
					positionY++;
				}
				ligneStream.clear();
				ligneStream.str("");
			}

			else if (choixCodeRecherche != article.codeArticle)
			{
				cout << endl << setw(31) << "Le code d'article << " << choixCodeRecherche << " >> nexiste pas.";
			}
		} while (!isSuccess && !fichierLecture.eof());

		fichierLecture.close();
	}
	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	positionnerCurseur(10, 22);
	system("pause");

}

void rechercherClient()
{
	system("cls");
	int positionX = 10;
	int positionY = 7;
	Client client;
	string nomFichier = "client.dat";
	ifstream fichierLecture(nomFichier, ios::in);

	if (fichierLecture) // Si "FichierLecture" contient quelque chose --> Ouverture réussi.
	{
		string choixCodeRecherche = "";
		bool isSuccess = false;

		positionnerCurseur(42, 5);
		cout << "RECHERCHE CLIENT";
		positionnerCurseur(42, 6);
		cout << "=================";
		positionnerCurseur(10, 9);
		cout << "Entrez le code du client cible : ";
		cin.ignore();
		getline(cin, choixCodeRecherche);

		string ligneLue;
		string champLu;
		stringstream ligneStream;

		do
		{
			getline(fichierLecture, ligneLue);
			if (!fichierLecture.eof())
			{

				ligneStream << ligneLue;
				getline(ligneStream, client.codeClient, ';');

				if (choixCodeRecherche == client.codeClient)
				{
					isSuccess = true;

					positionY += 2;
					positionnerCurseur(positionX, positionY);
					cout << setw(8) << "Code"
						<< setw(25) << "Nom client"
						<< setw(25) << "Courriel"
						<< setw(15) << "Type client"
						<< setw(15) << "Date adhesion"
						<< setw(10) << "Escompte"
						<< setw(10) << "Validite";

					positionY++;
					positionnerCurseur(positionX, positionY);
					cout << setw(8) << "----"
						<< setw(25) << "--------"
						<< setw(25) << "-------"
						<< setw(15) << "--------"
						<< setw(15) << "----------"
						<< setw(10) << "-------"
						<< setw(10) << "-------";
					positionY++;


					getline(ligneStream, client.nomClient, ';');
					getline(ligneStream, client.courriel, ';');
					getline(ligneStream, client.typeClient, ';');
					getline(ligneStream, client.dateAdhesion, ';');
					getline(ligneStream, champLu, ';');
					client.typeEscompte = champLu[0];
					getline(ligneStream, champLu, ';');
					client.codeValidite = stoi(champLu);
					ligneStream.clear();

					positionnerCurseur(positionX, positionY);
					cout << setw(8) << client.codeClient
						<< setw(25) << client.nomClient
						<< setw(25) << client.courriel
						<< setw(10) << client.typeClient
						<< setw(20) << client.dateAdhesion
						<< setw(10) << client.typeEscompte
						<< setw(10) << client.codeValidite;
					positionY++;
				}
				ligneStream.clear();
				ligneStream.str("");
			}

			else if (choixCodeRecherche != client.codeClient)
			{
				cout << endl << setw(31) << "Le code du client << " << choixCodeRecherche << " >> nexiste pas.";
			}
		} while (!isSuccess && !fichierLecture.eof());
		fichierLecture.close();
	}
	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	positionnerCurseur(10, 22);
	system("pause");

}

void supprimerClient()
{

	system("cls");
	int positionX = 10;
	int positionY = 7;
	Client client;
	string nomFichier = "client.dat";
	fstream fichierLecture(nomFichier, ios::in | ios::out);

	if (fichierLecture)
	{
		string choixCodeRecherche = "";
		bool isSuccess = false;

		positionnerCurseur(42, 5);
		cout << "SUPPRESSION CLIENT";
		positionnerCurseur(42, 6);
		cout << "=================";
		positionnerCurseur(10, 9);
		cout << "Entrez le numero du client a supprimer : ";
		cin.ignore();
		getline(cin, choixCodeRecherche);

		do
		{
			string ligneLue;
			string champLu;
			stringstream ligneStream;

			getline(fichierLecture, ligneLue);

			if (!fichierLecture.eof())
			{

				ligneStream << ligneLue;
				getline(ligneStream, client.codeClient, ';');

				if (choixCodeRecherche == client.codeClient)
				{
					isSuccess = true;

					positionY += 2;


					getline(ligneStream, client.nomClient, ';');
					getline(ligneStream, client.courriel, ';');
					getline(ligneStream, client.typeClient, ';');
					getline(ligneStream, client.dateAdhesion, ';');
					getline(ligneStream, champLu, ';');
					client.typeEscompte = champLu[0];
					getline(ligneStream, champLu, ';');
					client.codeValidite = stoi(champLu);
					ligneStream.clear();


				}
				ligneStream.clear();
				ligneStream.str("");
			}
		} while (!isSuccess && !fichierLecture.eof());

		if (isSuccess)
		{
			positionnerCurseur(positionX, positionY);
			cout << setw(8) << "Code"
				<< setw(25) << "Nom client"
				<< setw(25) << "Courriel"
				<< setw(15) << "Type client"
				<< setw(15) << "Date adhesion"
				<< setw(10) << "Escompte"
				<< setw(10) << "Validite";

			positionY++;
			positionnerCurseur(positionX, positionY);
			cout << setw(8) << "----"
				<< setw(25) << "--------"
				<< setw(25) << "-------"
				<< setw(15) << "--------"
				<< setw(15) << "----------"
				<< setw(10) << "-------"
				<< setw(10) << "-------";
			positionY++;

			positionnerCurseur(positionX, positionY);
			cout << setw(8) << client.codeClient
				<< setw(25) << client.nomClient
				<< setw(25) << client.courriel
				<< setw(10) << client.typeClient
				<< setw(20) << client.dateAdhesion
				<< setw(10) << client.typeEscompte
				<< setw(10) << client.codeValidite;
			positionY++;

			cout << endl << endl << "Voulez-vous vraiment le supprimer ? :  " << endl;
			string reponse = "";
			regex regex123("1|2|3");
			cin >> reponse;
			if (regex_match(reponse, regex123))
			{
				fichierLecture.seekp(-3, ios::cur);
				fichierLecture << reponse;
			}
		}
		else
		{
			positionnerCurseur(10, 11);
			cout << "Le code client << " << choixCodeRecherche << " >> n'existe pas.";

		}
		fichierLecture.close();
	}
	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	positionnerCurseur(10, 22);
	system("pause");
}

void modifierClient()
{
	system("cls");
	int positionX = 10;
	int positionY = 7;
	Client client;
	string nomFichier = "client.dat";
	fstream fichierLecture(nomFichier, ios::in | ios::out);

	if (fichierLecture)
	{
		string choixCodeRecherche = "";
		bool isSuccess = false;

		positionnerCurseur(42, 5);
		cout << "MODIFIER CLIENT";
		positionnerCurseur(42, 6);
		cout << "=================";
		positionnerCurseur(10, 9);
		cout << "Entrez le numero du client a modifier : ";
		cin.ignore();
		getline(cin, choixCodeRecherche);

		do
		{
			string ligneLue;
			string champLu;
			stringstream ligneStream;

			getline(fichierLecture, ligneLue);

			if (!fichierLecture.eof())
			{

				ligneStream << ligneLue;
				getline(ligneStream, client.codeClient, ';');

				if (choixCodeRecherche == client.codeClient)
				{
					isSuccess = true;

					positionY += 2;


					getline(ligneStream, client.nomClient, ';');
					getline(ligneStream, client.courriel, ';');
					getline(ligneStream, client.typeClient, ';');
					getline(ligneStream, client.dateAdhesion, ';');
					getline(ligneStream, champLu, ';');
					client.typeEscompte = champLu[0];
					getline(ligneStream, champLu, ';');
					client.codeValidite = stoi(champLu);
					ligneStream.clear();


				}
				ligneStream.clear();
				ligneStream.str("");
			}
		} while (!isSuccess && !fichierLecture.eof());

		if (isSuccess)
		{
			positionnerCurseur(positionX, positionY);
			cout << setw(8) << "Code"
				<< setw(25) << "Nom client"
				<< setw(25) << "Courriel"
				<< setw(15) << "Type client"
				<< setw(15) << "Date adhesion"
				<< setw(10) << "Escompte"
				<< setw(10) << "Validite";

			positionY++;
			positionnerCurseur(positionX, positionY);
			cout << setw(8) << "----"
				<< setw(25) << "--------"
				<< setw(25) << "-------"
				<< setw(15) << "--------"
				<< setw(15) << "----------"
				<< setw(10) << "-------"
				<< setw(10) << "-------";
			positionY++;

			positionnerCurseur(positionX, positionY);
			cout << setw(8) << client.codeClient
				<< setw(25) << client.nomClient
				<< setw(25) << client.courriel
				<< setw(10) << client.typeClient
				<< setw(20) << client.dateAdhesion
				<< setw(10) << client.typeEscompte
				<< setw(10) << client.codeValidite;
			positionY++;

			positionnerCurseur(positionX, positionY);
			cout << endl << endl << "Entrez un nouveau type descompte (1, 2 ou 3) ";
			string reponse = "";
			regex regex123("1|2|3");
			cin >> reponse;

			if (regex_match(reponse, regex123))
			{
				fichierLecture.seekp(-5, ios::end);
				fichierLecture << reponse;

				client.typeEscompte = reponse[0];

				positionY += 5;

				positionnerCurseur(positionX, positionY);
				cout << setw(8) << "Code"
					<< setw(25) << "Nom client"
					<< setw(25) << "Courriel"
					<< setw(15) << "Type client"
					<< setw(15) << "Date adhesion"
					<< setw(10) << "Escompte"
					<< setw(10) << "Validite";

				positionY++;
				positionnerCurseur(positionX, positionY);
				cout << setw(8) << "----"
					<< setw(25) << "--------"
					<< setw(25) << "-------"
					<< setw(15) << "--------"
					<< setw(15) << "----------"
					<< setw(10) << "-------"
					<< setw(10) << "-------";
				positionY++;

				positionnerCurseur(positionX, positionY);
				cout << setw(8) << client.codeClient
					<< setw(25) << client.nomClient
					<< setw(25) << client.courriel
					<< setw(10) << client.typeClient
					<< setw(20) << client.dateAdhesion
					<< setw(10) << client.typeEscompte
					<< setw(10) << client.codeValidite;
				positionY++;
				cout << endl;
				system("pause");
			}
		}
		else
		{
			positionnerCurseur(10, 11);
			cout << "Le code client << " << choixCodeRecherche << " >> n'existe pas." << endl;
			system("pause");

		}
		fichierLecture.close();
	}


}

void achat()
{
	system("cls");
	Client client;
	Article article;
	VenteArticle facture;

	bool isSuccess = false;
	string codeClientRecherche;
	string codeArticleRecherche;
	string dateTransaction_AAAA;
	string dateTransaction_MM;
	string dateTransaction_JJ;
	int qteVoulue = 0;
	float sousTotalArticle = 0.0f;
	float totalFacture = 0.0f;

	int positionX = 0;
	int positionY = 0;
	int cptrArticles = 0;

	//****************************************************

	positionX = 42;
	positionY = 2;
	positionnerCurseur(positionX, positionY);
	cout << "PRODUCTION DUNE FACTURE POUR UN CLIENT";
	positionnerCurseur(positionX, ++positionY);
	cout << "=======================================";

	//*****************************************************
	SYSTEMTIME Time; //POUR LIRE LA DATE DU JOUR DU SYSTEME
	GetLocalTime(&Time);
	dateTransaction_MM = to_string(Time.wMonth);
	dateTransaction_JJ = to_string(Time.wDay);
	dateTransaction_AAAA = to_string(Time.wYear);

	//S'assurer que les nombres contiennent exactement 2 chiffres (ajout d'un zéro pour 1)
	if (Time.wMonth <= 9)
	{
		dateTransaction_MM = "0" + dateTransaction_MM;
	}
	if (Time.wDay <= 9)
	{
		dateTransaction_JJ = "0" + dateTransaction_JJ;
	}

	positionX = 5;
	positionY += 2;
	positionnerCurseur(positionX, positionY);
	cout << "Date de la facture (AAAA MM JJ): ";
	cout << dateTransaction_AAAA << " " << dateTransaction_MM << " " << dateTransaction_JJ << endl;
	// *******************************************************

	do
	{

		positionnerCurseur(positionY, positionX + 2);
		cout << "Code de client (999 pour terminer) :";
		cin >> codeClientRecherche;
		isSuccess = getClient(codeClientRecherche, client);
		if (isSuccess == false && codeClientRecherche != "999")
		{
			afficherMsgClignotant("CODE CLIENT INEXISTANT", positionY + 2);
		}
	} while (isSuccess == false && codeClientRecherche != "999"); //Tant que la fonction échoue et que la recherche est différente de 999



	positionX = 8;
	positionY += 4;
	if (isSuccess)
	{
		facture.codeVente = dateTransaction_AAAA + dateTransaction_MM + dateTransaction_JJ + client.codeClient;

		positionY += 5;

		positionnerCurseur(positionX, positionY);
		cout << setw(15) << "Code facture"
			<< setw(15) << "Client"
			<< setw(15) << "Art."
			<< setw(15) << "Qte"
			<< setw(15) << "Prix"
			<< setw(10) << "S-tot.";

		positionY++;
		positionnerCurseur(positionX, positionY);
		cout << setw(15) << "---------"
			<< setw(15) << "--------"
			<< setw(15) << "-----"
			<< setw(15) << "------"
			<< setw(15) << "------"
			<< setw(10) << "-------";
		positionY++;


		do
		{
			positionnerCurseur(positionY, positionX + 2);
			cout << "Code de article (999 pour terminer) :";
			cin >> codeArticleRecherche;

			isSuccess = getArticle(codeArticleRecherche, article);
			if (!isSuccess && codeArticleRecherche != "999")
			{
				afficherMsgClignotant("CODE ARTICLE INEXISTANT", positionY);
			}

			if (codeArticleRecherche != "999" && isSuccess)
			{
				do
				{
					cout << "Saisir la quantite voulue : (" + to_string(article.qteStock) + " en stock)";
					cin >> qteVoulue;

					if (article.qteStock < qteVoulue)
					{
						afficherMsgClignotant("QUANTITE EN STOCK INSUFFISANTE", positionY);
						positionnerCurseur(positionX, positionY);
					}
				} while (qteVoulue != -1 && (qteVoulue <= 0 || qteVoulue > article.qteStock));

				if (qteVoulue != -1)
				{
					//Mise à jour des stocks ici ...
					article.qteStock -= qteVoulue;
					majStock(article);
					sousTotalArticle = qteVoulue * article.prixDeVente;
					facture.codeClient = client.codeClient;
					facture.codeArticle = article.codeArticle;
					facture.prixVente = article.prixDeVente;
					facture.qteAchetee = qteVoulue;
					facture.total = sousTotalArticle;
					facture.codeValidite = article.codeValidite;
					//Affichage de la ligne contenant l'achat ici ...
					totalFacture = facture.total;
					if (totalFacture > 0.0f)
					{
						ajoutFichierFacture(facture);
					}
				}
			}

		} while (codeArticleRecherche != "999");
	}

	positionX = 13;
	positionY += 7 + cptrArticles;
	positionnerCurseur(positionX, positionY);
	cout << "Montant total: " << totalFacture;
	positionnerCurseur(positionX, ++positionY);
	system("pause");
}

void afficherMsgClignotant(string msg, int positionY)
{
	int positionX = 66;
	HANDLE manipConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(manipConsole, 12);
	positionnerCurseur(positionX, positionY);
	cout << msg;
	this_thread::sleep_for(chrono::milliseconds(1000));
	SetConsoleTextAttribute(manipConsole, 7);
	positionnerCurseur(positionX, positionY);
	cout << msg << endl;
	this_thread::sleep_for(chrono::milliseconds(500));
}

bool getClient(string codeClientRecherche, Client& client)
{
	bool isSuccess = false;
	int positionY = 66;
	int positionX = 13;

	string nomFichier = "client.dat";
	ifstream fichierLecture(nomFichier, ios::in);



	if (fichierLecture) // Si "FichierLecture" contient quelque chose --> Ouverture réussi.
	{

		string ligneLue;
		string champLu;
		stringstream ligneStream;

		do
		{
			getline(fichierLecture, ligneLue);
			if (!fichierLecture.eof())
			{
				ligneStream << ligneLue;
				getline(ligneStream, client.codeClient, ';');

				if (codeClientRecherche == client.codeClient)
				{
					isSuccess = true;
					getline(ligneStream, client.nomClient, ';');
					getline(ligneStream, client.courriel, ';');
					getline(ligneStream, client.typeClient, ';');
					getline(ligneStream, client.dateAdhesion, ';');
					getline(ligneStream, champLu, ';');
					client.typeEscompte = champLu[0];
					getline(ligneStream, champLu, ';');
					client.codeValidite = stoi(champLu);

					ligneStream.clear();
					positionY++;
				}
				ligneStream.clear();
				ligneStream.str("");

			}

			else if (codeClientRecherche != client.codeClient)
			{
				isSuccess = false;
			}
		} while (!isSuccess && !fichierLecture.eof());

		fichierLecture.close();
	}

	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	return isSuccess;

	//Rendu ici, la variable -client- contiendra les infos du client
	//car elle a été passée à -getClient- par référence.
}

bool getArticle(string codeArticleRecherche, Article& article)
{
	bool isSuccess = false;
	int positionY = 66;
	int positionX = 13;

	string nomFichier = "inventaire.dat";
	ifstream fichierLecture(nomFichier, ios::in);



	if (fichierLecture) // Si "FichierLecture" contient quelque chose --> Ouverture réussi.
	{

		string ligneLue;
		string champLu;
		stringstream ligneStream;


		do
		{
			getline(fichierLecture, ligneLue);
			if (!fichierLecture.eof())
			{
				ligneStream << ligneLue;
				getline(ligneStream, article.codeArticle, ';');

				if (codeArticleRecherche == article.codeArticle)
				{
					isSuccess = true;
					getline(ligneStream, article.description, ';');
					getline(ligneStream, champLu, ';');
					article.coutUnitaire = stof(champLu);
					getline(ligneStream, champLu, ';');
					article.prixDeVente = stof(champLu);
					getline(ligneStream, champLu, ';');
					article.qteStock = stoi(champLu);
					getline(ligneStream, champLu, ';');
					article.seuil = stoi(champLu);
					getline(ligneStream, champLu, ';');
					article.codeValidite = stoi(champLu);
					ligneStream.clear();
					positionY++;
				}
				ligneStream.clear();
				ligneStream.str("");

			}
			else if (codeArticleRecherche != article.codeArticle)
			{
				isSuccess = false;
			}
		} while (!isSuccess && !fichierLecture.eof());


		fichierLecture.close();

	}



	else
	{
		afficherMsgErreur("ERREUR - Ouverture du fichier -" + nomFichier + "- impossible.");
	}

	return isSuccess;
}

void majStock(Article article)
{
	string nomFichier = "inventaire.dat";
	fstream fichierLecture(nomFichier, ios::in | ios::out);
	int positionX = 10;
	int positionY = 9;
	int nbrCharLigneTotale = 0;
	int cptrChar = 0;

	if (article.qteStock <= article.seuil)
	{
		afficherMsgClignotant("STOCK BAS --> A COMMANDER", 12);
	}

	if (fichierLecture) // Si "FichierLecture" contient quelque chose --> Ouverture réussi.
	{

		string ligneLue;
		string champLu;
		stringstream ligneStream;
		bool isSuccess = false;

		do
		{
			getline(fichierLecture, ligneLue);
			if (!fichierLecture.eof())
			{
				ligneStream << ligneLue;

				getline(ligneStream, champLu, ';');
				if (champLu == article.codeArticle)
				{
					nbrCharLigneTotale = ligneLue.length() + 2;
					cptrChar += champLu.length() + 1;				// ID

					getline(ligneStream, champLu, ';');				// Nom
					cptrChar += champLu.length() + 1;
					getline(ligneStream, champLu, ';');				// Cout unitaire
					cptrChar += champLu.length() + 1;
					getline(ligneStream, champLu, ';');				// Prix de vente
					cptrChar += champLu.length() + 1;

					isSuccess = true;
				}
			}
			ligneStream.clear();
			ligneStream.str("");

		} while (!fichierLecture.eof() && !isSuccess);

		cptrChar -= nbrCharLigneTotale;

		fichierLecture.seekp(cptrChar, ios::cur);
		fichierLecture << article.qteStock;

		fichierLecture.close();
	}
}

void ajoutFichierFacture(VenteArticle facture)
{
	bool isSuccess = false;

	do
	{
		positionnerCurseur(6, 16);
		cout << setw(15) << facture.codeVente
			<< setw(15) << facture.codeClient
			<< setw(15) << facture.codeArticle
			<< setw(15) << facture.prixVente
			<< setw(15) << facture.qteAchetee
			<< setw(10) << facture.total << endl;
		positionnerCurseur(15, 22);
		string reponse = "";
		cout << "Voulez-vous ajouter ces informations dans le fichier ? : ";
		cin >> reponse;
		regex regexOui("[oO]|oui|Oui|OUI");
		if (regex_match(reponse, regexOui))
		{
			//Ouverture du fichier inventaire.dat en mode append
			ofstream fichierFacture("facture.dat", ios::app);

			//ECRITURE CSV --> Séparateur = ; (donc espace POSSIBLE)
			fichierFacture << facture.codeVente << ";"
				<< facture.codeClient << ";"
				<< facture.codeArticle << ";"
				<< facture.prixVente << ";"
				<< facture.qteAchetee << ";"
				<< facture.total << ";"
				<< facture.codeValidite << endl;

			//Fermeture du fichier
			fichierFacture.close();

			system("pause");
			isSuccess = true;
		}
	} while (!isSuccess);
}