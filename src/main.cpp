#include "game.hpp"
#include "mainmenu.hpp"
#include "gameconfiguration.hpp"

int main()
{
    Game game {GameConfiguration::from_file("config.txt"), new MainMenu{}};
    game.run();
}