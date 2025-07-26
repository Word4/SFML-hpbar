#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <thread>
#include <random>
#include "HealthBar.h"

using namespace std;

vector<string> split_string(const string& input)
{
    istringstream iss(input);
    vector<string> tokens;
    string token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void console_input_handler(bool& running, HealthBar& healthbar)
{
    string input;

    while (running)
    {
        cout << "Health Bar Test Commands:\n"
            << "  hp value - Set current HP\n"
            << "  maxhp value - Set max HP\n"
            << "  random - Set random HP (0-max)\n"
            << "  exit - Close program\n"
            << "> ";
        getline(cin, input);
        system("cls");

        vector<string> tokens = split_string(input);

        if (tokens.empty()) continue;

        if (tokens[0] == "exit")
        {
            running = false;
        }
        else if (tokens[0] == "hp" && tokens.size() >= 2)
        {
            try {
                int hp = stoi(tokens[1]);
                healthbar.update(hp);
                cout << "HP set to: " << hp << endl;
            }
            catch (...) {
                cout << "Invalid HP value\n";
            }
        }
        else if (tokens[0] == "maxhp" && tokens.size() >= 2)
        {
            try {
                int maxhp = stoi(tokens[1]);
                healthbar.set_max_health(maxhp);
                cout << "Max HP set to: " << maxhp << endl;
            }
            catch (...) {
                cout << "Invalid Max HP value\n";
            }
        }
        else if (tokens[0] == "random")
        {
            int maxhp = healthbar.max_hp;
            int random_hp = rand() % (maxhp + 1);
            healthbar.update(random_hp);
            cout << "Random HP set to: " << random_hp << endl;
        }
        else
        {
            cout << "Unknown command\n";
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 500, 200 }), "Health Bar Test");
    window.setFramerateLimit(60);

    HealthBar healthbar(150, 90, 200, 20, 200);

    bool running = true;
    thread consoleThread(console_input_handler, ref(running), ref(healthbar));

    while (running && window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(50, 50, 50));
        healthbar.draw(window);
        window.display();
    }

    if (consoleThread.joinable())
        consoleThread.join();

    return 0;
}