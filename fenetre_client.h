#ifndef FENETRE_CLIENT_H
#define FENETRE_CLIENT_H

// fichiers par défaut
#include <QWidget>
#include <QtNetwork>
#include <QString>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QScrollArea>


class fenetre_client : public QWidget {
public:
    fenetre_client();

private slots:
    /*Renommer en français ! */
    void on_boutonConnexion_clicked(); // Activé quand on clique sur le bouton pour se connecter
    void on_boutonEnvoyer_clicked(); // Activé quand on clique sur le bouton pour envoyer un message
    void on_message_returnPressed(); /* Activé quand on appuie sur la touche entrée pour envoyer un message
                                       Problème, le signal qui est utilisé pour ça "editingFinished" s'active
                                       quand on appuie sur entrée, OU si le focus sur le champ de texte est perdu
                                       donc, si on s'amuse à cliquer sur autre chose ça envoie le message. C'est pas acceptable
                                       Il faudra songer à créer un signal exprès, mais là c'est pas le moment.*/

    void donneesRecues(); // Activé quand on reçoit un sous-paquet
    void connecte(); // Appelé lorsqu'on réussit à se conencter au serveur
    void deconnecte(); // Appelé lorsqu'on réussit à se déconencter du serveur
    void erreurSoccket(QAbstractSocket::SocketError erreur); // Appelé en cas d'erreur

private: //Widgets
    //Elements de l'interface
    //~~~ Identification et connexion
    QLineEdit *champ_pseudo;
    QLineEdit *champ_adresse;
    QLineEdit *champ_port;
    QPushButton *bouton_connexion;
    QPushButton *bouton_deconnexion;
    //~~~ Echange
    QLineEdit *champ_message;
    QTextEdit *historique_messages=nullptr;
    QPushButton *bouton_envoyer;
    //~~~ Layout
    QVBoxLayout *layout; /* Temporairement, l'interface sera une colonne*/
    //Elements de réseau
    QTcpSocket *socket; // Représente le serveur
    quint16 tailleMessage = 0;

};

#endif // FENETRE_CLIENT_H
/*
private:
    // Elements de l'interface
    QLabel *etatServeur;
    QPushButton *boutonQuitter;
    QVBoxLayout *layout;

    // Elements de réseau
    QTcpServer *serveur;
    QList<QTcpSocket *> clients;
    quint16 tailleMessage = 0;*/
