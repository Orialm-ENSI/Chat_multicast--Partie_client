#include "fenetre_client.h"

fenetre_client::fenetre_client()
{
interface:
{
    //~~~ Identification et connexion
    champ_pseudo = new QLineEdit(this);
    {
        champ_pseudo->setPlaceholderText("pseudo");
        champ_pseudo->setInputMask("AAAaaaaaaa");
    }

    champ_adresse = new QLineEdit(this);
    {
        champ_adresse->setPlaceholderText("adresse serveur");
        champ_adresse->setInputMask("000.000.000.000");
    }

    champ_port = new QLineEdit(this);
    {
        champ_port->setPlaceholderText("port serveur");
        champ_port->setInputMask("9999000000");
    }

    bouton_connexion = new QPushButton("connexion", this);
    {
        QObject::connect(bouton_connexion, &QPushButton::clicked, this, &fenetre_client::on_boutonConnexion_clicked);
    }

    bouton_deconnexion = new QPushButton("déconnexion", this);
    {
        bouton_deconnexion->setEnabled(false);
        QObject::connect(bouton_deconnexion, &QPushButton::clicked, this, &fenetre_client::deconnecte);
    }

    //~~~ Echange
    champ_message = new QLineEdit(this);
    {
        champ_message->setReadOnly(true);
        champ_message->setPlaceholderText("Ecrivez un message...");
    }

    historique_messages=new QTextEdit(this);
    {
        historique_messages->setReadOnly(true);
        //for(int i = 40; i>=1;i--){historique_messages->append(QString("ligne %1").arg(i));}
    }

    bouton_envoyer = new QPushButton("Envoyer", this);
    {
        bouton_envoyer->setEnabled(false);
        QObject::connect(bouton_envoyer, &QPushButton::clicked, this, &fenetre_client::on_boutonEnvoyer_clicked);
    }

    //~~~ Layout
    layout = new QVBoxLayout(this); /* Temporairement, l'interface sera une colonne*/
    {
        layout->addWidget(champ_pseudo);
        layout->addWidget(champ_adresse);
        layout->addWidget(champ_port);
        layout->addWidget(bouton_connexion);
        layout->addWidget(bouton_deconnexion);
        layout->addWidget(historique_messages);
        layout->addWidget(champ_message);
        layout->addWidget(bouton_envoyer);
        setWindowTitle("Fenetre du client");
    }
}

reseau:
{
    socket = new QTcpSocket(this);
    QObject::connect(socket, &QTcpSocket::readyRead, this, &fenetre_client::donneesRecues);
    QObject::connect(socket, &QTcpSocket::connected, this, &fenetre_client::connecte);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &fenetre_client::deconnecte);
    //QObject::connect(socket, &QTcpSocket::error, this, &fenetre_client::erreurSoccket);
}

}


// Slots
void fenetre_client::on_boutonConnexion_clicked()
{
    historique_messages->append("Tentative de connexion...");
    socket->abort(); // Désactivation des éventuelles précédentes connexion
    socket->connectToHost(champ_adresse->text(), champ_port->text().toUInt()); // Connexion

    bouton_connexion->setEnabled(false);
    bouton_deconnexion->setEnabled(true);
    bouton_envoyer->setEnabled(true);
    champ_adresse->setReadOnly(true);
    champ_port->setReadOnly(true);
    champ_pseudo->setReadOnly(true); // Pour empêcher ces 3 éléments de changer en cours de communication
    champ_message->setReadOnly(false);
}

void fenetre_client::on_boutonEnvoyer_clicked(){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QString messageAEnvoyer = champ_pseudo->text() + " : " + champ_message->text();

    out << (quint16) 0;
    out << messageAEnvoyer;
    out.device()->seek(0);
    out << (quint16)(paquet.size() - sizeof(quint16));
    socket->write(paquet);
    champ_message->clear();
    champ_message->setFocus();
}

void fenetre_client::donneesRecues()
{
    // Recuperation du message
    QDataStream in(socket); /*rendre les if plus propres */
    if(tailleMessage == 0){ // Si on a pas déjà la taille du message

        if(socket->bytesAvailable() < (int)sizeof(quint16)){ // Si on a pas au moins un int, alors on a pas encore reçu le message en entier
            return;
        }
        in >> tailleMessage; // Si on au moins un entier, alors on a la taille du message
    }

    if(socket->bytesAvailable() < tailleMessage) {return;} // Si on a pas encore le message entier, on attend

    QString messageRecu;
    in >> messageRecu; // On vide entièrement in dans message;
    historique_messages->append(messageRecu + " Recu "); // On l'envoi à tout le monde;
    tailleMessage = 0; // On se rend prêt à recevoir un nouveau message;
}

void fenetre_client::connecte()
{
    historique_messages->append("Vous êtes connecté sous le pseudo"+champ_pseudo->text());
}

void fenetre_client::deconnecte()
{
    historique_messages->append("Vous êtes déconnecté du serveur");
    bouton_connexion->setEnabled(true);
    bouton_deconnexion->setEnabled(false);
    bouton_envoyer->setEnabled(false);
    champ_adresse->setReadOnly(false);
    champ_port->setReadOnly(false);
    champ_pseudo->setReadOnly(false);
    champ_message->setReadOnly(true);
}



// QObject::connect(serveur, &QTcpServer::newConnection, this, &fenetre_serveur::nouvelleConnexion);
