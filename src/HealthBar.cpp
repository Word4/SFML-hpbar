#include "HealthBar.h"

HealthBar::HealthBar(int x, int y, int width, int height, int max_health, int samall_segment_size, int large_segment_size)
    : max_hp(max_health), cur_hp(max_health), samall_segment_size(samall_segment_size), large_segment_size(large_segment_size)
{
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(sf::Vector2f(x, y));
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineThickness(background_outline_boldness);
    background.setOutlineColor(sf::Color(0, 0, 0));

    health_bar.setSize(sf::Vector2f(width, height));
    health_bar.setPosition(sf::Vector2f(x, y));
    health_bar.setFillColor(sf::Color(255, 0, 0));

    update(max_health);
}

void HealthBar::update(int new_hp)
{
    cur_hp = new_hp;

    int percent_hp = (cur_hp * 100) / max_hp;
    int hp_bar_width = (background.getSize().x * percent_hp) / 100;

    if (hp_bar_width < health_bar.getSize().x)
    {
        int old_hp_width = health_bar.getSize().x;
        int faded_hp_width = old_hp_width - hp_bar_width;
        int faded_hp_x = hp_bar_width + health_bar.getPosition().x;
        faded_hp.setPosition(sf::Vector2f(faded_hp_x, health_bar.getPosition().y));
        faded_hp.setSize(sf::Vector2f(faded_hp_width, health_bar.getSize().y));

        decrease_start_faded_width = faded_hp_width;
        decrease_animation_clock.restart();
        decrease_is_animating = true;

        health_bar.setSize(sf::Vector2f(hp_bar_width, background.getSize().y));
        update_lines(hp_bar_width);
    }
    if (hp_bar_width >= health_bar.getSize().x)
    {
        increase_target_hp_width = hp_bar_width;
        increase_start_hp_width = health_bar.getSize().x;
        increase_animation_clock.restart();
        increase_is_animating = true;

        update_lines(hp_bar_width);
    }
}

void HealthBar::update_lines(int current_width)
{
    samall_lines.clear();
    large_lines.clear();

    small_lines_count = max_hp / samall_segment_size;
    large_lines_count = max_hp / large_segment_size;

    if (small_lines_count > 0)
        small_lines_distance = background.getSize().x / small_lines_count;

    if (large_lines_count > 0)
        large_lines_distance = background.getSize().x / large_lines_count;

    for (int i = 1; i < small_lines_count; ++i)
    {
        float line_pos = background.getPosition().x + i * small_lines_distance;

        if (line_pos <= background.getPosition().x + current_width)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(small_lines_boldness, background.getSize().y));
            line.setPosition(sf::Vector2f(line_pos, background.getPosition().y));
            line.setFillColor(sf::Color(0, 0, 0));
            samall_lines.push_back(line);
        }
    }

    for (int i = 1; i < large_lines_count; ++i)
    {
        float line_pos = background.getPosition().x + i * large_lines_distance;

        if (line_pos <= background.getPosition().x + current_width)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(large_lines_boldness, background.getSize().y));
            line.setPosition(sf::Vector2f(line_pos, background.getPosition().y));
            line.setFillColor(sf::Color(0, 0, 0));
            large_lines.push_back(line);
        }
    }
}

void HealthBar::draw(sf::RenderWindow& window)
{
    update_decrease_animation();
    update_increase_animation();

    window.draw(background);
    window.draw(faded_hp);
    window.draw(health_bar);

    for (sf::RectangleShape small_line : samall_lines)
        window.draw(small_line);
    for (sf::RectangleShape large_line : large_lines)
        window.draw(large_line);
}

void HealthBar::set_max_health(int hp)
{
    max_hp = hp;
    if (cur_hp > max_hp)
        cur_hp = max_hp;
    update(cur_hp);
}

void HealthBar::update_decrease_animation()
{
    if (!decrease_is_animating) return;

    float elapsed = decrease_animation_clock.getElapsedTime().asSeconds();
    float progress = elapsed / decrease_animation_duration;

    progress = 1.f - (1.f - progress) * (1.f - progress);

    if (progress >= 0.999f)
    {
        progress = 1.f;
        decrease_is_animating = false;
    }

    int cur_faded_width = decrease_start_faded_width + (decrease_target_faded_width - decrease_start_faded_width) * progress;
    faded_hp.setSize(sf::Vector2f(cur_faded_width, faded_hp.getSize().y));
}

void HealthBar::update_increase_animation()
{
    if (!increase_is_animating) return;

    float elapsed = increase_animation_clock.getElapsedTime().asSeconds();
    float progress = elapsed / increase_animation_duration;

    progress = 1.f - (1.f - progress) * (1.f - progress);

    if (progress >= 0.990f)
    {
        progress = 1.f;
        increase_is_animating = false;
    }

    int cur_hp_width = increase_start_hp_width + (increase_target_hp_width - increase_start_hp_width) * progress;
    health_bar.setSize(sf::Vector2f(cur_hp_width, health_bar.getSize().y));
}