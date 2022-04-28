// fichiers par défaut
#include <QtWidgets/QApplication>

// fichiers perso
#include "fenetre_client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    fenetre_client fenetre;
    fenetre.show();

    return app.exec();
}
