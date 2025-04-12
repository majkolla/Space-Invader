# Space Invader


## Installation

Kör följande i terminalen för att kompilera och starta spelet.

```terminal
make
./play
```

Notera att om [SFML](https://www.sfml-dev.org/index.php) inte är installerat globalt,
måste sökvägen till SFML nerladdningen anges i make-filen under variabeln `SF_ROOT`.
Detta ska inte behövas om datorerna på universitetet används.
Till exempel kan det se ut så här:

```makefile
SF_ROOT = C:/.../SFML
```

# Klassdiagram och CRC

Diagrammen nedan är skapade med [mermaid](https://mermaid-js.github.io/mermaid/#/).
Metoder och data markerade med `+` är public, medans de markerade med `-` är private
och `#` innebär att dem är protected. Kursiva metoder är virtuella. Understrukna
funktioner är statiska. `xxxxVector` är typedef till `std::vector<xxxx*>`, där xxxx
är GameObject, Button eller Sprite. På likande sätt är `xxxxMap` typefed till 
`std::map<std::string, xxxx>`.


## 1. Game, GameConfiguration och Context

<center>

```mermaid
classDiagram
    direction LR
    class Context {
        -m_delta: sf::Time
        -m_next_state: GameState*
        -m_objects: ObjectVector
        -m_new_objects: ObjectVector
        -m_window: const RenderWindow&
        -m_quit: bool
        -m_player: const Player*
        ...
        Getters and setters for all datamembers
        ...
        +spawn_object(GameObject* object)
    }

    class Game {
        +run()
        +set_state(GameState* new_state)
        +set_size(uint width, uint height)
        -handle(Event& event)
        -handle_context(Context& context)
        -switch_state(GameState* state)
        -m_current_state: GameState*
        -m_window: sf::RenderWindow
        -m_config: GameConfiguration
    }

    class GameConfiguration {
        +get_window_size() Vector2u
        +update_window_size(const Vector2u &window_size)
        +from_file(const String& path)$ GameConfiguration
        -m_resolution: Vector2u
        -m_fps: int
        ...More later...
    }

    Context "1" o-- "0.*" GameObject
    Game "1" o-- "1" GameConfiguration
    Context "1" o-- "1" GameState
    Game "1" o-- "1" GameState
```

</center>

- `Game` - klassen ansvarar för spelets huvud loop samt initialisera ett fönster.

- `Context` - en klass med all data som olika GameObjects och/eller GameStates kan
              komma att behöva. Utöver de listade datamedlemmarna i klassen, har Context dessutom getters och setters för datan.

- `GameConfiguration` - en klass som innehåller spelets konfiguration. Syftet med
                        klassen är att konfigurationen kan skapas genom att läsa
                        in en fil och därefter användas vid behov. Framförallt
                        när diverse GameStates ska initialiseras.

## 2. GameState

<center>

```mermaid
classDiagram
    direction TB
    class GameState {
        <<Abstract>>
        +render(RenderWindow& context)*
        +update(Context& context)*
        +handle(const Event& event, Context& context)*
        +init(const GameConfiguration& gc)*
    }

    class Menu {
        <<Abstract>>
        +render(...)
        +update(...)
        +handle(...)
        +init(...)*
        #add_button(Button* button)
        #add_sprite(Sprite* sprite)
        #m_buttons: ButtonVector
        #m_sprites: SpriteVector
    }

    GameState <-- Menu

    class EndScreen {
        +render(...)
        +update(...)
        +handle(...)
        +init(...)
    }

    class MainMenu {
        +render(...)
        +update(...)
        +handle(...)
        +init(...)
    }

    class PauseMenu {
        +render(...)
        +update(...)
        +handle(...)
        +init(...)
        #m_previous_state: GameMode*
    }

    Menu <-- EndScreen
    Menu <-- MainMenu
    Menu <-- PauseMenu

    class GameMode {
        <<Abstract>>
        +render(...)
        +update(...)
        +handle(...)
        +init(...)*
        #set_player(Player* player)
        #spawn_object(GameObject* object)
        #m_player: const Player*
        #m_objects: ObjectVector
        #m_clock: sf::Clock
    }

    class NormalMode {
        +render(...)
        +update(...)
        +handle(...)
        +init(...)
    }

    class BossMode {
        +render(...)
        +update(...)
        +handle(...)
        +init(...)
        #m_boss: const EnemyBoss*
        #m_previous_mode: NormalMode*
    }

    GameMode <|-- NormalMode
    GameMode <|-- BossMode
    GameState <|-- GameMode

    BossMode "1" o-- "1" EnemyBoss
    GameMode "1" o-- "0..*" GameObject
    GameMode "1" o-- "1" Player
    Menu "1" o-- "0..*" Button
    Menu "1" o-- "0..*" Sprite

```

</center>

- `GameState` - en abstrakt (pure virtual) klass som beskriver publika gränssnittet
för ett GameState. Syftet med GameState är att enkelt kunna dela in spelet i separata och eventuellt oberoende delar
och därefter byta mellan dem vid behov.

- `Menu` - en klass som utökar GameState med data och funktionalitet som är gemensamt för de olika menyerna i spelet.

- `MainMenu/PausMenu/EnsScreen` - tre olika menyer. MainMenu visas när spelet startas och PausMenu visas när spelet pausas. EndScreen visas när spelet förloras.

- `GameMode` - en klass som utökar GameState med data och funktionalitet som är gemensamt för de olika spellägen i spelet.

- `NormalMode/BossMode` - två olika spellägen.

## 3. GameObject

<center>

```mermaid
classDiagram
    class GameObject {
        <<Abstract>>
        +render(RenderWindow& window)*
        +update(Context& context)*
        +handle(const Event& event, Context& context)* bool
        +collides(const GameObject* other) bool
        +bounds()* FloatRect
        +collision(const GameObject* other)*
        +remove() bool
        +is_removed() bool
        #bool: removed
    }

    class Ship {
        <<Abstract>>
        +render(...)
        +update(...)*
        +handle(...)*
        +bounds() FloatRect
        +collision(...)
        #m_health: int
        #m_sprite: sf::Sprite
    }

    class Player {
        +update(...)
        +handle(...) bool
        +bounds() FloatRect
        +collision(...)
    }
    
    class Enemy {
        <<Abstract>>
        +update(...)*
        +handle(...)* bool
        +bounds() FloatRect
        +collision(...)
    }

    class EnemyMinion {
        +update(...)
        +handle(...) bool
        +bounds() FloatRect
        +collision(...)
    }

    class EnemyBoss {
        +update(...)
        +handle(...) bool
        +bounds() FloatRect
        +collision(...)
    }

    Enemy <|-- EnemyMinion
    Enemy <|-- EnemyBoss
    
    Ship <|-- Enemy
    Ship <|-- Player

    class Projectile {
        <<Abstract>>
        +render(...)*
        +update(...)*
        +handle(...)* bool
        +bounds()* FloatRect
        +collision(...)*
        +is_friendly() bool
        #m_sprite: sf::Sprite
        #m_speed: float
        #m_damage: int
        #friendly: bool
    }

    class BasicProjectile {
        +render(...)
        +update(...)
        +handle(...) bool
        +bounds() FloatRect
        +collision(...)
        -m_direction: sf::Vector2
        -m_speed: float
    }

    Projectile <|-- BasicProjectile

    class PowerUp {
        <<Abstract>>
        +render(...)*
        +update(...)*
        +handle(...)* bool
        +bounds(...) FloatRect
        +collision(...)* FloatRect
        #m_sprite: sf::Sprite
    }
    
    class Repair {
        +render(...)
        +update(...)
        +handle(...) bool
        +bounds(...) FloatRect
        +collision(...)
    }

    class Speed {
        +render(...)
        +update(...)
        +handle(...) bool
        +bounds(...) FloatRect
        +collision(...)
    }

    PowerUp <|-- Repair
    PowerUp <|-- Speed
    
    GameObject <|-- Ship
    GameObject <|-- Projectile
    GameObject <|-- PowerUp
```

</center>

- `GameObject` - en abstrakt klass som beskriver publika gränssnittet för ett GameObject.
Genom att ha en abstrakt GameObject klass som samtliga spelobjekt ärver ifrån och implementerar,
kan spelobjekten grupperas i t.ex. en array utan problem, vilket leder till enklare programmering.

## 4. UI

<center>

```mermaid
classDiagram
    class UI {
        <<Abstract>>
        +render(RenderWindow& window)*
    }

    class  DynamicUI {
        <<Abstract>>
        +render(RenderWindow& window)*
        +update(Context& context)*
    }

    class Button {
        +render(...)
        +update(...)
        +handle_click(const Event& event, Context& context) bool
        #m_button_sprite: Sprite
        #m_image: Texture
        #m_hover_image: Texture
        #m_hoverable: bool
        #click_function: Function*
    }

    UI <-- DynamicUI
    DynamicUI <-- Button

```

</center>

- `UI` - en abstrakt klass för användargränssnitt.

- `DynamicUI` - utökar UI med en update metod så den kan uppdateras varje frame.

- `Button` - en klass som beskriver en knapp som går att trycka på med musen.
             Kan exekvera en given funktion när knappen blir tryckt.

## 5. ResourceManager

<center>

```mermaid
classDiagram
class ResourceManager {
    +load_texture(const std::string &path)$ Texture
    +load_sound(const std::string &path)$ SoundBuffer
    +load_font(const std::string &path)$ Font
    -Textures TextureMap$
    -Fonts FontMap$
    -Sounds SoundBufferMap$
    }

```

</center>

- `ResourceManager` - en resurshanterare, som laddar in texturer, ljud och teckensnitt,
                      och sparar dem i en std::map så de endast laddas in en gång.
                      Inladdade resurser har dessutom statisk livslängd.
