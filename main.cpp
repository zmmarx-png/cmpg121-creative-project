// Zandrie Marx
// 55838294
// Creative project

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Structs
struct Item {
    string name;
    int value;
};

struct Monster {
    string type;
    int strength;
};

struct Player {
    string name;
    int health;
    int xp;
    Item inventory[5];
};

// Function declarations
void displayIntro();
void createPlayer(Player &p);
void exploreRoom(Player &p);
void fightMonster(Player &p, Monster *m);
void collectItem(Player &p, Item *i);
void saveGame(Player *p);
void loadGame(Player &p);
void showInventory(Player &p);
void levelUp(Player &p);
string getRandomMonster();
Item getRandomItem();

int main() {
    srand(time(0));
    Player player;
    int choice;

    displayIntro();
    cout << "1. New Game\n2. Load Game\nChoose: ";
    cin >> choice;

    if (choice == 1)
        createPlayer(player);
    else
        loadGame(player);

    while (player.health > 0) {
        cout << "\n--- Dungeon Room ---\n";
        cout << "1. Explore\n2. Show Inventory\n3. Save Game\n4. Exit\nChoose: ";
        cin >> choice;

        switch (choice) {
            case 1: exploreRoom(player); break;
            case 2: showInventory(player); break;
            case 3: saveGame(&player); break;
            case 4: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }

    cout << "You have perished in the dungeon...\n";
    return 0;
}

// Function definitions
void displayIntro() {
    cout << R"(
    ============================
       DUNGEON EXPLORER
    ============================
    )" << endl;
}

void createPlayer(Player &p) {
    cout << "\nEnter your hero's name: ";
    cin.ignore();
    getline(cin, p.name);
    p.health = 100;
    p.xp = 0;
    for (int i = 0; i < 5; i++) p.inventory[i] = {"Empty", 0};
}

void exploreRoom(Player &p) {
    int event = rand() % 3;
    if (event == 0) {
        Monster m = {getRandomMonster(), rand() % 30 + 10};
        fightMonster(p, &m);
    } else if (event == 1) {
        Item i = getRandomItem();
        collectItem(p, &i);
    } else {
        cout << "The room is empty. You rest and recover 5 health.\n";
        p.health += 5;
        if (p.health > 100) p.health = 100;
    }
}

void fightMonster(Player &p, Monster *m) {
    cout << "A wild " << m->type << " appears with strength " << m->strength << "!\n";
    int damage = rand() % m->strength;
    p.health -= damage;
    cout << "You fight bravely and take " << damage << " damage.\n";
    p.xp += m->strength;
    levelUp(p);
}

void collectItem(Player &p, Item *i) {
    cout << "You found a " << i->name << " worth " << i->value << " gold!\n";
    for (int iSlot = 0; iSlot < 5; iSlot++) {
        if (p.inventory[iSlot].name == "Empty") {
            p.inventory[iSlot] = *i;
            return;
        }
    }
    cout << "\nInventory full! You leave the item behind.\n";
}

void showInventory(Player &p) {
    cout << "\nInventory:\n";
    for (int i = 0; i < 5; i++) {
        cout << "- " << p.inventory[i].name << " (" << p.inventory[i].value << " gold)\n";
    }
    cout << "Health: " << p.health << ", XP: " << p.xp << "\n";
}

void levelUp(Player &p) {
    if (p.xp >= 100) {
        cout << "\nLevel Up! Your health increases by 20.\n";
        p.health += 20;
        p.xp = 0;
    }
}

void saveGame(Player *p) {
    ofstream file("savegame.txt");
    file << p->name << "\n" << p->health << "\n" << p->xp << "\n";
    for (int i = 0; i < 5; i++)
        file << p->inventory[i].name << "," << p->inventory[i].value << "\n";
    file.close();
    cout << "Game saved!\n";
}

void loadGame(Player &p) {
    ifstream file("savegame.txt");
    getline(file, p.name);
    file >> p.health >> p.xp;
    file.ignore();
    for (int i = 0; i < 5; i++) {
        getline(file, p.inventory[i].name, ',');
        file >> p.inventory[i].value;
        file.ignore();
    }
    file.close();
    cout << "Game loaded for " << p.name << "!\n";
}

string getRandomMonster() {
    string monsters[] = {"Goblin", "Skeleton", "Orc", "Spider", "Zombie"};
    return monsters[rand() % 5];
}

Item getRandomItem() {
    string items[] = {"Sword", "Shield", "Potion", "Gem", "Scroll"};
    int values[] = {50, 40, 30, 100, 20};
    int index = rand() % 5;
    return {items[index], values[index]};
}
