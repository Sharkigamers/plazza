/*
** EPITECH PROJECT, 2020
** Kitchen
** File description:
** Kitchen
*/

#include <iostream>

#include "Kitchen.hpp"

Plazza::Kitchen::Kitchen(int serverFd, int chefNumber, int serverPort)
{
    Plazza::Food ingrediant;

    _ingrediant[0] =  ingrediant;
    ingrediant.setName("tomato");
    _ingrediant[1] =  ingrediant;
    ingrediant.setName("gruyere");
    _ingrediant[2] =  ingrediant;
    ingrediant.setName("ham");
    _ingrediant[3] =  ingrediant;
    ingrediant.setName("mushroom");
    _ingrediant[4] =  ingrediant;
    ingrediant.setName("steak");
    _ingrediant[5] =  ingrediant;
    ingrediant.setName("eggplant");
    _ingrediant[6] =  ingrediant;
    ingrediant.setName("goatCheese");
    _ingrediant[7] =  ingrediant;
    ingrediant.setName("chiefLove");
    _ingrediant[8] =  ingrediant;
    _cookNumber = chefNumber;
    initRecipe();
    _client = new ServerClient(serverFd, serverPort);
}

Plazza::Kitchen::~Kitchen()
{
}

Plazza::Cooks::PizzaType Plazza::Kitchen::unpack(char *pizza)
{
    if (strcasecmp(pizza, "Regina") == 0)
        return Plazza::Cooks::Regina;
    else if (strcasecmp(pizza, "Margarita") == 0)
        return Plazza::Cooks::Margarita;
    else if (strcasecmp(pizza, "Americana") == 0)
        return Plazza::Cooks::Americana;
    else if (strcasecmp(pizza, "Fantasia") == 0)
        return Plazza::Cooks::Fantasia;
    return (Plazza::Cooks::PizzaType)0;
}

std::string Plazza::Kitchen::pack(Plazza::Cooks::PizzaType pizza)
{
    if (pizza == Plazza::Cooks::Regina)
        return "Regina";
    else if (pizza == Plazza::Cooks::Margarita)
        return "Margarita";
    else if (pizza == Plazza::Cooks::Americana)
        return "Americana";
    else if (pizza == Plazza::Cooks::Fantasia)
        return "Fantasia";
    return "Unknown";
}

bool Plazza::Kitchen::acceptPizza(Plazza::Cooks::PizzaType pizza)
{
    _locker.lock();
    if (_queue.size() < 2 * _cookNumber) {
        for (const auto &recipeContent: _recipe)
            if (pizza == recipeContent.first) {
                _queue.push_back(pizza);
                _locker.unlock();
                return true;
            }
    }
    _locker.unlock();
    return false;
}

void Plazza::Kitchen::parseContent(std::string file_content)
{
    std::string firstColumn;
    std::string secondColumn;
    int id;
    std::string str = "";
    std::map<std::string, int> recipe;

    if (file_content == "")
        return;
    for (auto &character: file_content) {
        if (character == '\n') {
            firstColumn = str.substr(0, str.find(":"));
            secondColumn = str.substr(str.find(":") + 1, str.length());
            if (firstColumn == "id")
                id = std::stoi(secondColumn);
            else
                recipe[firstColumn] = std::stoi(secondColumn);
            str = "";
        } else
            str += character;
    }
    _recipe[(Plazza::Cooks::PizzaType)id] = recipe;
}

void Plazza::Kitchen::parseFile(std::string path)
{
    std::ifstream infile(path);
    std::string line = "";
    std::string file_content;

    while (std::getline(infile, line))
        file_content += line + "\n";
    parseContent(file_content);
}

void Plazza::Kitchen::file_treatement(const char *path, struct dirent *dirt)
{
    const static std::string verify_file = ".pizza";
    bool valid_file = false;

    for (int i = strlen(dirt->d_name), j = verify_file.length();
        i >= 0 && j >= 0; --i, --j) {
            valid_file = true;
            if (dirt->d_name[i] != verify_file[j]) {
                valid_file = false;
                break;
            }
        }
    if (valid_file) {
        std::string file_path = dirt->d_name;
        parseFile(path + file_path);
    }
}

void Plazza::Kitchen::initRecipe(const char *path)
{
    DIR *dir = opendir(path);
    struct dirent *dirt;
    std::string file_path;

    if (!dir) {
        perror("directory");
        exit(84);
    }
    while ((dirt = readdir(dir))) {
        if (dirt->d_name)
            file_treatement(path, dirt);
    }
}

bool Plazza::Kitchen::isWorking() const
{
    if (_cooks->getListAvailable() == _cookNumber)
        return false;
    return true;
}

void Plazza::Kitchen::regenFood()
{
    for (auto &ingredient: _ingrediant)
        ingredient.refill(1);
}

std::map<Plazza::Cooks::PizzaType, std::map<std::string, int>> Plazza::Kitchen::getRecipe() const
{
    return _recipe;
}

int Plazza::Kitchen::takePizza()
{
    int pizza = 1; 

    pizza = (int)_queue.front();
    _queue.erase(_queue.begin());
    return pizza;
}

bool Plazza::Kitchen::takeIngredient(std::string ingredient, int quantity)
{
    _locker.lock();
    for (auto &food: _ingrediant) {
        if (food.getName().compare(ingredient) == 0) {
            if (food.getQuantity() >= quantity) {
                food.take(quantity);
                _locker.unlock();
                return true;
            }
        }
    }
    _locker.unlock();
    return false;
}

unsigned short Plazza::Kitchen::getCookNumber() const
{
    return _cookNumber;
}

unsigned short Plazza::Kitchen::getFreeCooks()
{
    return _cooks->getListAvailable();
}

bool Plazza::Kitchen::actionFromMessage(Plazza::message_t message, pid_t pid)
{
    Plazza::message_t *sendMessage;
    char stringMessage[1] = "";
    unsigned short nb = 0;
    char *cookStatus;

    if (message.type == PIZZA_REQUEST) {
        if (acceptPizza(static_cast<Plazza::Cooks::PizzaType>(message.value)) == true) {
            sendMessage = createMessage(ACCEPT_PIZZA, stringMessage, pid, message.value);
            try {
                _cooks->runTask(this);
                getClient()->sendResponse(sendMessage);
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        } else {
            sendMessage = createMessage(REFUSE_PIZZA, stringMessage, pid, message.value);
            try {
                getClient()->sendResponse(sendMessage);
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    else if (message.type == REQUEST_COOKS_STATUS) {
        nb = getFreeCooks();
        std::string freeCookNumber = "There are " + std::to_string(nb) + " available cooks.";
        cookStatus = strdup(freeCookNumber.c_str());
        sendMessage = createMessage(RESPONSE_STATUS, cookStatus, pid, std::atoi(cookStatus));
        try {
            getClient()->sendResponse(sendMessage);
        } catch (const std::exception &e) {
            throw e;
        }
        free(cookStatus);
    }
    else if (message.type == EXIT_KITCHEN)
        return false;
    return true;
}

void Plazza::Kitchen::kitchenWorking()
{
    int time_working = std::time(nullptr);
    int regen_time = std::time(nullptr);
    Plazza::message_t receiveMessage;
    this->initThreadPool();

    while ((std::time(nullptr) - time_working) < 5) {
        receiveMessage = getClient()->pollEvent();
        if (receiveMessage.type != NONE)
            actionFromMessage(receiveMessage, getpid());
        if (std::time(nullptr) - regen_time >= _cookNumber) {
            regenFood();
            regen_time = std::time(nullptr);
        }
        if (isWorking())
            time_working = std::time(nullptr);
    }
    _client->sendResponse(createMessage(Plazza::messageType::EXIT_KITCHEN, EXIT_MESSAGE, getpid(), 0));
}