#include "game_over_system.hpp"
#include "../messages/game_over_message.hpp"
#include "../main/game_globals.hpp"

void game_over_system::configure() {
	subscribe<game_over_message>([this](game_over_message &msg) {
		pause_mode = PAUSED;
		this->loss_mode = msg.loss_type;
	});
}

void game_over_system::update(const double ms) {
	if (loss_mode > 0) {
		term(1)->clear();
		term(1)->print_center(10, "Game over, man!", rltk::colors::YELLOW);

		if (loss_mode == 1) {
			term(1)->print_center(12, "All of your settlers are dead! With nobody left to care for,");
			term(1)->print_center(13, "Cordex issues one last command: shutdown all systems.");
			term(1)->print_center(14, "Somewhere, a debug message reads: 'test 1: fail'");
		}
		else if (loss_mode == 2) {
			term(1)->print_center(12, "Cordex's battery backup has failed, leading to an abrupt - and");
			term(1)->print_center(13, "permanent - shutdown. You can safely assume that without Cordex's");
			term(1)->print_center(14, "guidance, everyone dies a miserable death.");
		}
		term(1)->print_center(20, "Press ESCAPE to quit.", rltk::colors::YELLOW);
	}

	if (is_window_focused() && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		quitting = true;
	}
}
