//
// Created by zacomo on 19/09/17.
//
//
// Created by zacomo
//


#include <unordered_set>
#include <vector>
#include "../include/Window.h"
#include "../include/Menu.h"
#include "../include/personaggiGiocabili.h"
using namespace std;

    PlayableCharacter::PlayableCharacter(){

        this->LP = -1;
        this->MP = -1;
        this->DEF = -1;
        this->ATK = -1;
        this->Name = "";
        this->LUCK = -1;

        for (int i = 0; i < 5; i++)
            equipment[i] = Item();
    }

    PlayableCharacter::PlayableCharacter(int LP, int MP, int DEF, int ATK, int LUCK, int Coins, std::string Name){
        this->LP = LP;
        this->LPMAX = LP;
        this->MP = MP;
        this->MPMAX = MP;
        this->DEF = DEF;
        this->ATK = ATK;
        this->LUCK = LUCK;
        this->LV = 1;
        this->Coins = Coins;
        this->Name = Name;
        for (int i = 0; i<5; i++){
            equipment [i] = Item();      //Inizializza il vettore di Item "equipment"
        }
        this->pieces = 0;
    }

    int PlayableCharacter::getLP(){
        return this->LP;
    }

    int PlayableCharacter::getLPMAX() {
        return this->LPMAX;
    }

    int PlayableCharacter::getMP(){
        return this->MP;
    }

    int PlayableCharacter::getMPMAX() {
        return this->MPMAX;
    }

    int PlayableCharacter::getDEF(){
        return min(this->DEF,90);   // La Difesa massima può arrivare fino a 90 (a 100 il pg è invincibile)
    }

    int PlayableCharacter::getATK(){
        return this->ATK;
    }

    Point PlayableCharacter::getPosition() {
        return this->POS;
    }

    int PlayableCharacter::getLuck(){
        return min(this->LUCK,30);  // La fortuna masssima può arrivare fino a 30
    }

    int PlayableCharacter::getLV(){
        return this->LV;
    }

    int PlayableCharacter::getCoins(){
        return this->Coins;
    }

    void PlayableCharacter::setLP(int LP){
        if (LP < LPMAX)
            this->LP = LP;
        else
            this->LP = LPMAX;
    }

    void PlayableCharacter::setMP(int MP){
        this->MP = max(MP, 0);
    }

    void PlayableCharacter::setDEF(int DEF){
        this->DEF = DEF;
    }

    void PlayableCharacter::setATK(int ATK){
        this->ATK = ATK;
    }

    void PlayableCharacter::setPosition(int x, int y){
        this->POS.x = x;
        this->POS.y = y;
    }

    bool PlayableCharacter::addCoins(int x) {
        if ((x + this->Coins) >= 0){
            this->Coins += x;
            return true;
        }

        return false;

    }

    void PlayableCharacter::setCoins(int Coins){
        if (Coins > 0)
            this->Coins = Coins;
        else
            this->Coins = 0;
    }

    std::string PlayableCharacter::getName(){
        return this->Name;
    }

    bool PlayableCharacter::useConsumable(Item sbobba){
        if (sbobba.getType() == 5){ //  I consumabili sono item con campo Type pari a 5
            if (LPMAX >= LP + sbobba.getLP())
                LP = LP + sbobba.getLP();
            else
                LP = LPMAX;

            if (MPMAX >= MP + sbobba.getMP())
                MP = MP + sbobba.getMP();
            else
                MP = MPMAX;

            DEF = DEF + sbobba.getDEF();

            if (LUCKMAX >= LUCK + sbobba.getLuck())
                LUCK = LUCKMAX;
            else
                LUCK = LUCK + sbobba.getLuck();

            ATK = ATK + sbobba.getATK();
            Inventory.erase(Inventory.find(sbobba));    //  Consumabile utilizzabile una sola volta, quindi rimosso poi

            return true;
        }
        return false;
    }

    bool PlayableCharacter::equip(Item ferraglia){  //  Item equipaggiabili hanno Type da 0 a 5 compresi
        if ((ferraglia.getType() > -1 && ferraglia.getType() < 5) && (equipment[ferraglia.getType()] == Item())){

            equipment[ferraglia.getType()] = ferraglia; //  L'Item va ad occupare la sua posizione nel vettore equip.

            DEF = DEF + ferraglia.getDEF();

            ATK = ATK + ferraglia.getATK();

            return true;
        }
        return false;
    }

    bool PlayableCharacter::unequip(Item ferraglia){
        if (equipment[ferraglia.getType()] != Item()){  //Controllo per vedere se un oggetto di quel tipo è nell'equip.
            DEF = DEF - ferraglia.getDEF();
            ATK = ATK - ferraglia.getATK();
            equipment[ferraglia.getType()] = Item(); //Rimuove l'oggetto dall'equip.
            return true;
        }

        return false;
    }


    bool PlayableCharacter::pickItem(Item thing){
        bool done = false;
        if (Inventory.size() < 20) {    // 20 è la dimensione dell'inventario
            Inventory.insert(thing);
            done = true;
        }
        return done;
    }

    bool PlayableCharacter::dropItem(Item thing){
        bool done = false;
        if (!Inventory.empty()) {

            Inventory.erase(thing);
            done = true;
        }
        return done;
    }

    void PlayableCharacter::LVLup() {
        this->LV++;
        int atk = 0, def = 0, lp = 0, mp = 0, luck = 0;
        if (this->Name == "Gaudenzio"){ //  Ogni PG Incrementa le statistiche in modo diverso
            atk = 1;
            def = 2;
            lp = 2;
            luck = 1;
        }
        else if (this->Name == "Peppino"){
            atk = 1;
            def = 1;
            lp = 1;
            mp = 3;
            luck = 1;
        }
        else if (this->Name == "Badore"){
            atk = 1;
            lp = 1;
            def = 1;
            luck = 3;
        }

        this->ATK += atk;
        this->DEF += def;
        this->MPMAX += mp ;
        this->LPMAX += lp ;
        this->LUCK += luck;
};

Item PlayableCharacter::getEquipmentAt(int index) {
    return equipment[index];
}

void PlayableCharacter::showInventory() {

    using namespace std;
    int choice = 0;
    int select;
    bool look= false;
    int iterator= 0;
    vector<Item> I_vector;
    vector<string> names;
    Item it;

    for(Item i : Inventory) // Inserisce i nomi nella lista delle entries e gli oggetti nel vettore degli items
    {
        if(i.getType() != 5 && equipment[i.getType()] != Item() && i.getId() == equipment[i.getType()].getId())
            names.push_back("*" + i.getName());
        else
            names.push_back(i.getName());
        I_vector.push_back(i);
        iterator++;
    }

    for (; iterator < 20; iterator++){               // riempie i vuoti se ce ne sono
        names.push_back("-VUOTO-");
    }
    
    names.push_back(string("BACK"));

    Menu inv (0, 0, names);  // Mostra le scelte possibili (Item nell'inventario)
    Window ItemInventory (inv.getX() + 20, inv.getY(), 21, 50);      // Crea una finestra per l'inventario
    ItemInventory.hide();

    choice = inv.handleChoice();

    while (!look){

        if(choice < names.size() - 1){
            // Se non si vuole tornare indietro e non si sceglie una casella vuota
            // mostra le caratteristiche dell'Item
            if(names[choice] !=  "-VUOTO-"){
                ItemInventory.show();
                ItemInventory.clean();
                it = I_vector[choice];
                ItemInventory.printLine("STATISTICHE:");
                ItemInventory.printLine("LP: " + to_string(it.getLP()));
                ItemInventory.printLine("MP: " + to_string(it.getMP()));
                ItemInventory.printLine("ATK: " + to_string(it.getATK()));
                ItemInventory.printLine("DEF: " + to_string(it.getDEF()));
                ItemInventory.printLine("LUCK: " + to_string(it.getLuck()));
                ItemInventory.separator();

                ItemInventory.printLine("Premi: ");

                if (it.getType() != 5){
                    ItemInventory.printLine("u) Equipaggia");
                    ItemInventory.printLine("i) Disequipaggia");
                }
                else if (it.getType() == 5)
                    ItemInventory.printLine("u) Usa consumabile");

                ItemInventory.printLine("q) Chiudi inventario");
                ItemInventory.printLine("d) Droppa oggeto");

                select= getch();

                switch(select)
                {
                    case 'u':
                        ItemInventory.clean();
                        if (it.getType() != 5) {
                            if (!equip(it))   // Se ritorna vero l'Item è equipaggiato
                            {
                                ItemInventory.printLine(it.getName() + " NON equipaggiat"+suffix(it.getType())+":");
                                ItemInventory.printLine("Stai usando un oggetto dello stesso tipo!");
                            }
                            else{
                                ItemInventory.printLine(it.getName() + " equipaggiat"+suffix(it.getType()));
                                look= true;
                            }
                        }
                        else if (it.getType() == 5){
                            if (!useConsumable(it)) // Se ritorna vero il consumabile viene utilizzato
                                ItemInventory.printLine(it.getName() + " NON usata");
                            else{
                                ItemInventory.printLine(it.getName() + " usata!");
                                look= true;
                            }
                        }
                        getch();
                        break;

                    case 'i':
                        ItemInventory.clean();
                        if (it.getType() == 5)
                            ItemInventory.printLine(it.getName() + " IMPOSSIBILE equipaggiare!");
                        else if (it.getType() != 5){
                            if (!unequip(it))   // Se ritorna vero l'Item viene disequipaggiato
                                ItemInventory.printLine("Che ne dici di equipaggiare l'oggetto prima?");
                            else{
                                ItemInventory.printLine(it.getName() + " disequipaggiat"+suffix(it.getType()));
                                look= true;
                            }
                        }
                        getch();
                        break;

                    case 'q':
                        ItemInventory.clean();
                        ItemInventory.hide();
                        choice = inv.handleChoice();
                        if(choice == names.size() - 1)
                            look = true;
                        break;

                    case 'd':
                        ItemInventory.clean();
                        if(it.getType() != 5 && equipment[it.getType()].getId() == it.getId())
                            unequip(it);
                        dropItem(it);
                        ItemInventory.printLine(it.getName() + " droppat"+suffix(it.getType())+"!");
                        getch();
                        ItemInventory.hide();
                        look = true;
                        break;
                }
            }
            else
            {
                choice = inv.handleChoice();
                ItemInventory.hide();
            }

        }
        else
            look= true;
    }
}

string suffix(int type){
    string sfx;
    switch (type){
        case 0:
        case 4:
            sfx = "o";
            break;
        case 1:
        case 3:
            sfx = "a";
            break;
        case 2:
            sfx = "i";
            break;
        }

    return sfx;
}

void PlayableCharacter::pickUpPiece()
{
    this->pieces++;
}

int PlayableCharacter::getPieces()
{
    return this->pieces;
}
