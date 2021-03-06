//
// Created by zacomo on 19/09/17.
//

#include "../include/Item.h"
#include <fstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

using namespace std;

Item::Item() {
    name ="";
    type = -1;
    LP = 0;
    MP = 0;
    DEF = 0;
    ATK = 0;
    LUCK = 0;
    position.x = -1;
    position.y = -1;
    symbol = '?';
    visible = false;
}

Item::Item(string name, int type, int LP, int MP, int DEF, int ATK, int LUCK) {
    this->name = name;

    if (type > -1 && type <= 5) //type va da 0 a 5
        this->type = type;

    this->LP = LP;
    this->MP = MP;
    this->DEF = DEF;
    this->ATK = ATK;
    this->LUCK = LUCK;
    position.x = -1;
    position.y = -1;
    symbol = '?';   //  E' il simbolo che indica un Item sulla mappa
    visible = false;

}

string Item::getName() {
    return this->name;
}

int Item::getType(){
    return this->type;
}

int Item::getLP() {
    return this->LP;
}

int Item::getMP() {
    return this->MP;
}

int Item::getDEF() {
    return this->DEF;
}

int Item::getATK() {
    return this->ATK;
}

int Item::getLuck() {
    return this->LUCK;
}

string Item::getId() {
    return this->id;
}

bool operator==(Item i1, Item i2) {
    return i1.id == i2.id;
}

bool operator!=(Item i1, Item i2) {
    return i1.id != i2.id;
}

char Item::getSymbol(){
    return this->symbol;
}

Point Item::getPosition() {
    return this->position;
}

Point Item::setPosition(int x, int y) {
    this->position.x = x;
    this->position.y = y;
}

bool Item::isVisible() {
    return this->visible;
}

void Item::setVisible(bool b) {
    this->visible = b;
}

void Item::setId(string id) {
    this->id = id;
}

namespace std
{
    size_t hash<Item>::operator()(Item i) const
    {
        return hash<string>()(i.getId());
    }
    // Hash function for the class Item
}

void retrieveItems(std::vector<Item>& set)
{
    using namespace boost;
    using namespace std;
    string name,line;
    int type,LP,MP,DEF,ATK,LUCK;
    char_separator<char> sep(",");
    ifstream itemsFile("resources/items.txt");

    while(itemsFile)
    {
        getline(itemsFile,line);
        if(line != "")
        {
            tokenizer<char_separator<char>> tokens(line,sep);
            tokenizer<char_separator<char>>::iterator it = tokens.begin();
            name = *it;
            ++it;
            type = stoi(*it);
            ++it;
            LP = stoi(*it);
            ++it;
            MP = stoi(*it);
            ++it;
            DEF = stoi(*it);
            ++it;
            ATK = stoi(*it);
            ++it;
            LUCK = stoi(*it);
            set.push_back(Item(name,type,LP,MP,DEF,ATK,LUCK));
        }
    }
}
