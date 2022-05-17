GTKMM_CF = `pkg-config gtkmm-3.0 --cflags`
GTKMM_LD = `pkg-config gtkmm-3.0 --libs`

CFLAGS = $(GTKMM_CF) -Wall  # les options du compilateur
LDFLAGS = $(GTKMM_LD)       # les options pour l’éditeur de liens

CC = g++  # le compilateur à utiliser

SRC = main.cpp VueG.cpp Matrice_Calc.cpp # les fichiers sources
PROG = main     # nom de l’exécutable
OBJS = $(SRC:.cpp=.o)  # les .o qui en découlent
.SUFFIXES: .cpp .o     # lien entre les suffixes

all: $(PROG)

# étapes de compilation et d’édition de liens
# $@ la cible $^ toutes les dépendances
$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	
main.o: VueG.hpp
main.o: Modele.hpp
main.o: Controleur.hpp
main.o: Command.hpp
main.o: History.hpp
main.o: Matrice.hpp
main.o: MatrixCom.hpp
main.o: Observable.hpp
main.o: Observer.hpp
main.o: parametres.hpp
main.o: Matrice_Calc.hpp
VueG.o: VueG.hpp
VueG.o: Controleur.hpp
VueG.o: parametres.hpp
Matrice_Calc.o: Matrice_Calc.hpp

# le lien entre .o et .c 
# $< dernière dépendance
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# pour faire propre
.PHONY: clean
clean: 
	rm -f *.o *~
