#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <thread>

using namespace std;

static string option;
const static string shopOp = "S";
const static string homeOp = "H";
const static string lineOfMachinesOp = "L";

bool getTutorialStage() {
    ifstream tutorialStage("tutorialPhase.txt");
    string tutorialBool;

    getline(tutorialStage, tutorialBool);

    tutorialStage.close();

    return (tutorialBool == "Yes.");
}

vector<string> getMacNames() {
    vector<string> names;
    ifstream namesFile("machineNames.txt");
    string name;

    while (getline(namesFile, name)) {
        names.push_back(name);
    }

    namesFile.close();
    return names;
}

vector<string> getMacLevels() {
    vector<string> levels;
    ifstream levelsFile("machineLevels.txt");
    string level;

    while (getline(levelsFile, level)) {
        levels.push_back(level);
    }

    levelsFile.close();
    return levels;
}

void resetFiles(){
    ofstream ofs;
    ofs.open("machineNames.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("machineLevels.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("balance.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void setupFiles(){
    resetFiles();
    ofstream balanceFile("balance.txt");

    balanceFile << 0 << endl;

}

void addPlayerMac(string machineName){
    ofstream levelsFile("machineLevels.txt");
    ofstream namesFile("machineNames.txt");

    namesFile << machineName << endl;
    levelsFile << 0 << endl;
}

void getShopPrices(vector<string>& shopPrices) {
    ifstream pricesReader("shopPrices.txt");
    string price;

    while (getline(pricesReader, price)) {
        shopPrices.push_back(price);
    }

    pricesReader.close();
}

void getShopElements(vector<string>& shopElements) {
    ifstream shopReader("shopElements.txt");
    string element;

    while (getline(shopReader, element)) {
        shopElements.push_back(element);
    }

    shopReader.close();
}

string getUserInput() {
    string decision;
    getline(cin, decision);
    return decision;
}

bool isInteger(const string& str) {
    try {
        stoi(str);
        return true;
    } catch (const exception&) {
        return false;
    }
}

bool verifyUserInput(string& option, string correctOption){
    if(option != correctOption){
        return false;
    }
    return true;
}

int getNumberOfMac(){
    ifstream machinesFile("machineNames.txt");

    int numberOfMachines = 0;
    string line;

    while(getline(machinesFile, line)){
        numberOfMachines ++;
    }

    return numberOfMachines;
}

int getMachineIndex(string machineName){
    ifstream machinesFile("machineNames.txt");

    int currentIndex = 0;
    string line;

    while(getline(machinesFile, line)){
        if(line == machineName){
            return currentIndex;
        }
        currentIndex++;
    }
}

void improperInputMessage(){
    cout << "Nope, that's not it. Don't add spaces, and make sure the letter is LIKE THIS." << endl;
}

void delayMessage(double seconds) {
    Sleep(static_cast<DWORD>(seconds * 1000));
}

void clearConsole() {
    system("cls");
}

void lineOfMachines(){
    vector<string> machineNames = getMacNames();
    vector<string> machineLevels = getMacLevels();

    cout << "=THE MACHINE LINE=" << endl;
    cout << "\n=-=-=-=-=-=-=-=-=" << endl;
    for(int i = 0; i < machineNames.size(); i++){
        cout << machineNames.at(i) << " | " << "LVL " << machineLevels.at(i) << endl;
    }
}

void outputShop(const vector<string>& shopElements, const vector<string>& shopPrices) {
    for (size_t i = 0; i < shopElements.size(); i++) {
        cout << "[" << (i + 1) << "] | " << shopElements[i] << " | $" << shopPrices[i] << endl;
    }
}

void updatePlayerBalance(int monetaryDiff, char typeOfUpdate) {
    ifstream balanceReader("balance.txt");
    string stringMoney;

    getline(balanceReader, stringMoney);
    balanceReader.close();

    int money = stoi(stringMoney);

    if (typeOfUpdate == 'A') {
        money += monetaryDiff;
    } else if (typeOfUpdate == 'S') {
        money -= monetaryDiff;
    }

    ofstream balanceUpdater("balance.txt");
    balanceUpdater << money;
    balanceUpdater.close();
}

int getPlayerBalance() {
    ifstream balanceReader("balance.txt");
    string stringMoney;

    balanceReader >> stringMoney;
    balanceReader.close();

    return stoi(stringMoney);
}

void wrongShopInputTutorial(const vector<string>& shopElements) {
    cout << "Improper input... Who taught you to write input like that?" << endl;
    delayMessage(2);
    clearConsole();

    if (getTutorialStage()) {
        cout << "Alright, look, I know it's a little tough but you have to pull through. \n Here, try again, and please, do better." << endl;
        vector<string> shopPrices;
        getShopPrices(shopPrices);
        outputShop(shopElements, shopPrices);
    }
}

void getShopInput();
void shop();

void handlePurchase(int optionNum, const vector<string>& shopElements, const vector<string>& shopPrices) {
    string machineChosen = shopElements[optionNum - 1];

    cout << "Very well, so you have chosen " << machineChosen << "!" << endl;

    if (getPlayerBalance() >= stoi(shopPrices[optionNum - 1])) {

        cout << "Adding machine..." << endl;
        addPlayerMac(machineChosen);
        delayMessage(3);

        updatePlayerBalance(stoi(shopPrices[optionNum - 1]), 'S');
        cout << shopElements[optionNum - 1] << " has been added to your inventory." << endl;

        cout << "Do you want to keep looking [S], or do you wish to leave[H]?" << endl;
        string option = getUserInput();

        if (verifyUserInput(option, shopOp)) {

            vector<string> updatedShopElements;
            getShopElements(updatedShopElements);
            vector<string> updatedShopPrices;
            getShopPrices(updatedShopPrices);

            outputShop(updatedShopElements, updatedShopPrices);
            getShopInput();
        } else if (verifyUserInput(option, homeOp)) {
            cout << "Going back home..." << endl;
        } else {
            wrongShopInputTutorial(shopElements);
            getShopInput();
        }
    } else {
        cout << "You don't have enough money for that machine! Price: $" << shopPrices[optionNum - 1] << endl;
        cout << "Do you want to keep looking [S], or do you wish to leave[H]?" << endl;
        string option = getUserInput();
        if (verifyUserInput(option, shopOp)) {

            vector<string> updatedShopElements;
            getShopElements(updatedShopElements);
            vector<string> updatedShopPrices;
            getShopPrices(updatedShopPrices);

            outputShop(updatedShopElements, updatedShopPrices);
            getShopInput();
        } else if (verifyUserInput(option, homeOp)) {
            vector<string> playerMachines = getMacNames();

            if(getTutorialStage() && playerMachines.size() == 0){
                cout << "You don't have a machine yet, silly." << endl;
                cout << "Let's give this another shot, I will boot the shop again for you. Loading..." << endl;
                delayMessage(2.5);
                clearConsole();
                shop();

            }
            cout << "Going back home..." << endl;
        } else {
            wrongShopInputTutorial(shopElements);
            getShopInput();
        }
    }
}

void getShopInput() {
    vector<string> shopElements;
    vector<string> shopPrices;

    getShopElements(shopElements);
    getShopPrices(shopPrices);

    cout << "Now, traveler. What do you wish to pick? \n[1-" << shopElements.size() << "]" << endl;
    cout << "Input: ";
    string option = getUserInput();

    if (isInteger(option)) {
        int optionNum = stoi(option);
        if (optionNum >= 1 && optionNum <= static_cast<int>(shopElements.size())) {
            handlePurchase(optionNum, shopElements, shopPrices);
        } else {
            wrongShopInputTutorial(shopElements);
            getShopInput();
        }
    } else {
        wrongShopInputTutorial(shopElements);
        getShopInput();
    }
}

void shop() {
    vector<string> shopElements;
    getShopElements(shopElements);

    cout << "Welcome to the shop!" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "\nBalance: $" << getPlayerBalance() << endl;
    vector<string> shopPrices;
    getShopPrices(shopPrices);
    outputShop(shopElements, shopPrices);
    getShopInput();
}



void offerTutorial() {
    cout << "[QUICK NOTE: If you leave the tutorial, everything you have done until then will reset.]\nIt seems like you don't have a machine yet. Allow me to help." << endl;
    delayMessage(3);
    updatePlayerBalance(100, 'A');

    cout << "I have just allotted $100 in your balance. That is just enough to buy a machine." << endl;
    delayMessage(3);

    do {
        cout << "Go ahead. [S] for shop. You have to get acquainted with the inputs around here, after all." << endl;
        option = getUserInput();

        if (option != "S") {
        }
    } while (option != "S");

    cout << "Good stuff. I will be loading the shop for you. As before, just insert what you wish to purchase,\n this time only with the number." << endl;
    delayMessage(2);
    shop();

    cout << "Loading..." << endl;
    delayMessage(2.5);
    clearConsole();

    do{
        cout << "Great. Now you have a machine." << endl;
        cout << "Let's now go to the line of machines. [" << lineOfMachinesOp << "]" << endl;
        getline(cin, option);
        if(verifyUserInput(option, lineOfMachinesOp)){
            cout << "Loading..." << endl;
            delayMessage(2.5);
            clearConsole();
            lineOfMachines();
        }else{
            improperInputMessage();
        }
    }while(option != lineOfMachinesOp);
}

void startGame() {
    vector<string> names = getMacNames();
    vector<string> levels = getMacLevels();

    if (getTutorialStage()) {
        setupFiles();
        offerTutorial();
    }
}

bool verifyMenuInput(string decision) {
    if (decision == "1" || decision == "2") {
        return true;
    }

    return false;
}

void startMenu() {
    cout << "MACHINETYCOON 1.9" << endl;
    cout << "-----------------" << endl;
    cout << "| [1] | Boot Game" << endl;
    cout << "| [2] | Exit Game" << endl;
    cout << "-----------------" << endl;
    cout << "Input: ";
    string decision = getUserInput();

    if (!verifyMenuInput(decision)) {
        clearConsole();
        cout << "Invalid input. Please try again." << endl;
        startMenu();
    }

    if (decision == "1") {
        clearConsole();
        startGame();
    } else if (decision == "2"){
        cout << "Exiting game..." << endl;
    }else{                            
        startMenu();
    }
}
int main() {
    HANDLE colorHandler = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(colorHandler, 3);
	startMenu();
	return 0;
}