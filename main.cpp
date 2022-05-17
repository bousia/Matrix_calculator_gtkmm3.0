
/*  main.cpp
 *
 *  application de calculs matriciels
 *
 *  Réalisé par Fangquing XIAO et Yanis VIOLA
 *
 *  compilation :             $ make
 *  lancement :               $ ./main
 *  compilation + lancement : $ ./run
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#include <gtkmm/application.h>

#include "VueG.hpp"
#include "Modele.hpp"
#include "Controleur.hpp"

////////////////////////////////////////////////////////////////////
// APPLICATION                                                    //
////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // creation de l'application
	auto app = Gtk::Application::create(argc, argv);

  // creation du modele, de la vue et du controleur
 	Modele *m = new Modele();
  VueG *vg = new VueG();
  Controleur *c = new Controleur(m, vg);  
  	
  // pour eviter le warning : "warning: unused variable ‘c’ "
  // l'utilisation de ‘c’ est faite dans la classe vue au travers des boutons
  c=c; 

  // association modele et observer abstrait
  m->addObserver(vg);
  
  return app->run(*vg);
  
}

