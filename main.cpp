#include <iostream>
#include <string>
#include <sstream>

#include "exceptions.h"
#include "game_manager.h"
#include "CEnergyConsumer.h"
#include "CPeopleHaver.h"
#include "house.h"
#include "factory.h"
#include "generator.h"
#include "park.h"

void displayTopRightUI(const GameManager& game)
{
    std::string remaining = "Remaining Turns: " + std::to_string(game.getMaxTurns() - game.getCurrentTurn() + 1);
    std::string quota = "Quota: " + std::to_string(game.getMoneyNeededToWin()) + "$";

    const size_t width = 80;
    if (remaining.size() < width)
    {
        std::cout << std::string(width - remaining.size(), ' ');
    }
    std::cout << remaining << "\n";

    if (quota.size() < width)
    {
        std::cout << std::string(width - quota.size(), ' ');
    }
    std::cout << quota << "\n";
}

void displayResources(const GameManager& game)
{
    std::cout << "Money: " << game.getResource(ResourceType::Money) << "$  "
              << "Population: " << game.getResource(ResourceType::Population) << "  "
              << "Happiness: " << game.getResource(ResourceType::Happiness) << "  "
              << "Workers: " << game.getResource(ResourceType::Workers) << "\n";
}

void displayBuildings(const GameManager& game)
{
    const auto& buildings = game.getBuildings();

    int totalProduction = 0;
    int totalConsumption = 0;
    for (const Building* building : buildings) 
    {
        if (const Generator* gen = dynamic_cast<const Generator*>(building))
            totalProduction += gen->getEnergyProduction();
        
        const CEnergyConsumer* energyComp = building->getComponent<CEnergyConsumer>();
        if (energyComp != nullptr && building->isActive())
            totalConsumption += energyComp->getEnergyConsumption();
    }

    bool energyShortage = totalProduction < totalConsumption;

    // afisare cladiri
    for (int i = 0; i < static_cast<int>(buildings.size()); ++i) 
    {
        const Building* building = buildings[i];
        std::cout << (i + 1) << ". " << (*building);
        
        const CEnergyConsumer* energyComponent = building->getComponent<CEnergyConsumer>();
        int energyConsumption = 0;
        if (energyComponent != nullptr && building->isActive())
            energyConsumption = energyComponent->getEnergyConsumption();
        
        if (energyConsumption > 0)
            std::cout << "    Energy: " << energyConsumption;
        else if (dynamic_cast<const Generator*>(building) != nullptr)
            std::cout << "    Energy: +" << dynamic_cast<const Generator*>(building)->getEnergyProduction();

        std::string indicators;
        if (energyConsumption > 0 && energyShortage)
            indicators += " !";
        if (game.getResource(ResourceType::Happiness) < 0) 
        {
            const House* house = dynamic_cast<const House*>(building);
            if (house != nullptr) {
                const CPeopleHaver* peopleComp = house->getComponent<CPeopleHaver>();
                if (peopleComp != nullptr && peopleComp->getPeople() > 0) {
                    indicators += " :(";
                }
            }
        }
        
        std::cout << indicators;

        std::cout << "\n";
    }

    std::cout << "\nEnergy Production: " << totalProduction << "\n";
    std::cout << "Energy Consumption: " << totalConsumption << "  Pollution: " << game.getResource(ResourceType::Pollution) << "\n";
}

bool processCommand(GameManager& game)
{
    std::string line;
    std::cout << "\nIntrodu comanda: ";
    std::getline(std::cin, line);

    std::stringstream ss(line);
    std::string command;
    std::string argument;
    ss >> command >> argument;

    try
    {
        if (command == "build") 
        {
            if (argument == "house")
                game.buyBuilding(new House());
            else if (argument == "factory")
                game.buyBuilding(new Factory());
            else if (argument == "generator")
                game.buyBuilding(new Generator());
            else if (argument == "park")
                game.buyBuilding(new Park());
            else
                throw InvalidCommandException("Cladire necunoscuta: " + argument);
        } 
        else if (command == "next")
            game.endTurn();
        else if (command == "activate" || command == "deactivate") 
        {
            if (argument.empty())
                throw InvalidCommandException("Lipseste argumentul cladirii");
            int index = std::stoi(argument);
            const auto& buildings = game.getBuildings();
            if (index < 1 || index > static_cast<int>(buildings.size()))
                throw InvalidCommandException("Nu exista cladirea cu acel numar");

            const Building* building = buildings[index - 1];
            if (dynamic_cast<const Factory*>(building) == nullptr)
                throw InvalidCommandException("Doar fabricile pot fi activate/dezactivate");
            if (command == "activate")
                game.activateBuilding(index - 1);
            else
                game.disableBuilding(index - 1);
        } 
        else if (command == "exit")
            return false;
        else
            throw InvalidCommandException("Comanda necunoscuta: " + command);
    }
    catch (const GameException& ex)
    {
        std::cout << ex.what() << "\n";
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "Introduceti un argument valid\n";
    }
    catch (const std::exception&)
    {
        std::cout << "Eroare\n";
    }

    return true;
}

void clearConsole()
{
    for(int i = 0; i < 100; ++i) 
    {
        std::cout << "\n";
    }
}

int main()
{
    auto& game = GameManager::getInstance();
    game.initGameManager(200, 20, 10000);
    game.buyBuilding(new House());
    game.buyBuilding(new Generator());

    while (!game.victoryCheck() && game.getCurrentTurn() <= game.getMaxTurns()) 
    {
        game.updateWorkerAssignment();
        clearConsole();
        displayTopRightUI(game);
        displayResources(game);
        std::cout << "\n";
        displayBuildings(game);

        if (!processCommand(game))
            break;
    }

    if (game.victoryCheck())
    {
        clearConsole();
        std::cout << "Victory!!";
    }
    else
    {
        clearConsole();
        std::cout << "You lost!!";
    }

    return 0;
}