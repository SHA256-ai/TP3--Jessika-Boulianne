#include "fonctions.h"
#include <iostream>

int main()
{
    int choixMenu = -1;
	bool siValide = true;
	do
	{
		if (!siValide) {
			afficherMsgErreur("Choix invalide");
		}
		afficherMenu(siValide);
		positionnerCurseur(33, 30);
		cin >> choixMenu;
		siValide = siChoixMenuValide(choixMenu);
		if (siValide)
		{
			switch (choixMenu)
			{
			case 1:
				ajoutArticle();
				break;
			case 2:
				ajoutClient();
				break;
			case 3:
				listeArticles();
				break;
			case 4:
				listeClients();
				break;
			case 5:
				rechercherArticle();
				break;
			case 6:
				rechercherClient();
				break;
			case 7:
				supprimerClient();
				break;
			case 8: 
				modifierClient();
				break;
			case 9: 
				achat();
				break;
			}

		}

	} while (choixMenu != 99);
}
