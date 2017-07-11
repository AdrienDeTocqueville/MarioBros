#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


using vec = sf::Vector2f;
using ivec = sf::Vector2i;
using uvec = sf::Vector2u;


enum CursorMode {GE_FREE, GE_CAPTURE};

class Input
{
    public:
            static void init(sf::RenderWindow* _window);
            static void destroy();

            static void update();

            static void setWindowSize(uvec _size);

        /// Methods (public)
            static bool hasFocus();
            static bool isOpen();

            static bool textIsChar();
            static bool textIsNum();

        /// Setters
            static void setCursorMode(CursorMode _mode);
            static void setMousePosition(ivec _pos);

            static void setKeyState(sf::Keyboard::Key _key, bool _state);

        /// Getters
            static CursorMode getCursorMode();
            static ivec getMouseDelta();
            static int getMouseWheelDelta();

            static const sf::Event& getEvent();
            static sf::RenderWindow* getWindow();

            static uvec getWindowSize();

            static bool getKeyDown(sf::Keyboard::Key _key);
            static bool getKeyPressed (sf::Keyboard::Key _key);
            static bool getKeyReleased(sf::Keyboard::Key _key);

            static ivec getMousePosition(bool openGLSpace = false);

            static bool getMouseDown(sf::Mouse::Button _button);
            static bool getMousePressed(sf::Mouse::Button _button);
            static bool getMouseReleased(sf::Mouse::Button _button);

            static char getText();

            static sf::Uint32 unicode;

    private:
        /// Attributes
            static sf::RenderWindow* window;
            static sf::Event event;

            static uvec dim;
            static ivec center;

            static ivec delta;
            static int wheelDelta;

            static CursorMode mode;

            static bool focus;
            static bool close;

            static int stateIndex;

            static bool mouseState[2][sf::Mouse::ButtonCount];
            static bool keyboardState[2][sf::Keyboard::KeyCount];
};
