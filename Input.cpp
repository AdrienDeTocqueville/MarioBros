#include "Input.h"

sf::RenderWindow* Input::window = nullptr;
sf::Event Input::event;

uvec Input::dim(0, 0);
ivec Input::center(0, 0);

ivec Input::delta(0, 0);
int Input::wheelDelta(0);

CursorMode Input::mode = GE_FREE;

bool Input::focus = true;
bool Input::close = false;

int Input::stateIndex = 1;

bool Input::mouseState[2][sf::Mouse::ButtonCount] = {false};
bool Input::keyboardState[2][sf::Keyboard::KeyCount] = {false};

sf::Uint32 Input::unicode;

/// Methods (private)
void Input::init(sf::RenderWindow* _window)
{
    window = _window;

    dim = window->getSize();
    center = ivec(dim) / 2;

    focus = true;
    close = false;
}

void Input::destroy()
{
    window = nullptr;

    focus = true;
    close = true;
}

void Input::update()
{
    wheelDelta = 0;
    unicode = 0;

    while (window->pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                close = true;
            break;

            case sf::Event::MouseWheelMoved:
                wheelDelta = event.mouseWheel.delta;
            break;

            case sf::Event::LostFocus:
                focus = false;
            break;

            case sf::Event::GainedFocus:
                focus = true;
            break;

            case sf::Event::Resized:
                dim = uvec(event.size.width, event.size.height);
                center = ivec(dim) / 2;
            break;

            case sf::Event::TextEntered:
                unicode = event.text.unicode;
            break;

            default: break;
        }
    }

    stateIndex = 1-stateIndex;

    for (int i(0) ; i < sf::Mouse::ButtonCount ; i++)
        mouseState[stateIndex][i] = sf::Mouse::isButtonPressed( (sf::Mouse::Button)i );

    for (int i(0) ; i < sf::Keyboard::KeyCount ; i++)
        keyboardState[stateIndex][i] = sf::Keyboard::isKeyPressed( (sf::Keyboard::Key)i );


    if (mode == GE_CAPTURE)
    {
        delta = center - sf::Mouse::getPosition(*window);
        delta.y *= -1;

        sf::Mouse::setPosition(center, *window);
    }
}

void Input::setWindowSize(uvec _size)   // private
{
    window->setSize(_size);

    dim = _size;
    center = ivec(dim) / 2;
}

void Input::setKeyState(sf::Keyboard::Key _key, bool _state)
{
    keyboardState[stateIndex][_key] = _state;
}

/// Methods (public)
bool Input::hasFocus()
{
    return focus;
}
bool Input::isOpen()
{
    return !close && window->isOpen();
}

bool Input::textIsChar()
{
    return (unicode >= 32 && unicode < 128);
}

bool Input::textIsNum()
{
    return (unicode >= '0' && unicode <= '9');
}

/// Setter
void Input::setCursorMode(CursorMode _mode)
{
    if (mode == _mode)
        return;

    mode = _mode;

    delta = ivec(0, 0);
    sf::Mouse::setPosition(center, *window);

    if (mode == GE_FREE)
        window->setMouseCursorVisible(true);
    else
        window->setMouseCursorVisible(false);
}

void Input::setMousePosition(ivec _pos)
{
    sf::Mouse::setPosition(_pos, *window);
}

/// Getters
CursorMode Input::getCursorMode()
{
    return mode;
}

ivec Input::getMouseDelta()
{
    return delta;
}

int Input::getMouseWheelDelta()
{
    return wheelDelta;
}

const sf::Event& Input::getEvent()
{
    return event;
}

sf::RenderWindow* Input::getWindow()
{
    return window;
}

uvec Input::getWindowSize()
{
    return dim;
}

bool Input::getKeyDown(sf::Keyboard::Key _key)
{
    return keyboardState[stateIndex][_key];
}

bool Input::getKeyPressed(sf::Keyboard::Key _key)
{
    if (keyboardState[stateIndex][_key] == keyboardState[1-stateIndex][_key])
        return false;

    return keyboardState[stateIndex][_key];
}

bool Input::getKeyReleased(sf::Keyboard::Key _key)
{
    if (keyboardState[stateIndex][_key] == keyboardState[1-stateIndex][_key])
        return false;

    return !keyboardState[stateIndex][_key];
}

ivec Input::getMousePosition(bool openGLSpace)
{
    if (!openGLSpace)
        return sf::Mouse::getPosition(*window);

    ivec mPos(sf::Mouse::getPosition(*window));
    return ivec(mPos.x, window->getSize().y - mPos.y);
}

bool Input::getMouseDown(sf::Mouse::Button _button)
{
    return mouseState[stateIndex][_button];
}

bool Input::getMousePressed(sf::Mouse::Button _button)
{
    if (mouseState[stateIndex][_button] == mouseState[1-stateIndex][_button])
        return false;

    return mouseState[stateIndex][_button];
}

bool Input::getMouseReleased(sf::Mouse::Button _button)
{
    if (mouseState[stateIndex][_button] == mouseState[1-stateIndex][_button])
        return false;

    return !mouseState[stateIndex][_button];
}

char Input::getText()
{
    return static_cast<char>(unicode);
}
