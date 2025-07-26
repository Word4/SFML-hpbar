#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class HealthBar
{
public:
    HealthBar(int x, int y, int width, int height, int max_health = 100, int samall_segment_size = 25, int large_segment_size = 50);
    ~HealthBar() = default;

    void update(int new_hp);
    void draw(sf::RenderWindow& window);
    void set_max_health(int hp);

    int max_hp;
    int cur_hp;

private:
    void update_lines(int current_width);
    void update_decrease_animation();
    void update_increase_animation();

    sf::RectangleShape background;
    sf::RectangleShape health_bar;
    sf::RectangleShape faded_hp;

    std::vector<sf::RectangleShape> samall_lines;
    std::vector<sf::RectangleShape> large_lines;

    int small_lines_count;
    int large_lines_count;

    int samall_segment_size;
    int large_segment_size;

    int small_lines_distance;
    int large_lines_distance;

    const int small_lines_boldness = 1;
    const int large_lines_boldness = 2;
    const int background_outline_boldness = 3;

    sf::Clock decrease_animation_clock;
    const float decrease_animation_duration = 0.5f;
    int decrease_target_faded_width = 0;
    int decrease_start_faded_width = 0;
    bool decrease_is_animating = false;

    sf::Clock increase_animation_clock;
    const float increase_animation_duration = 0.2f;
    int increase_target_hp_width = 0;
    int increase_start_hp_width = 0;
    bool increase_is_animating = false;
};