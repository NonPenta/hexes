#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "context.hpp"
#include "entity.hpp"

class InputMode : public sf::Drawable {
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
public:
  virtual void updateContext(Context context) = 0;
  virtual std::unique_ptr<InputMode> eventHandler(Context &context, sf::Event event) = 0;
  virtual ~ InputMode() = default;
};

class SelectionMode : public InputMode {
public:
  SelectionMode(Context context);

  void updateContext(Context context);

  std::unique_ptr<InputMode> eventHandler(Context &context, sf::Event event);

private:
  sf::Vector2i mousePos;

  std::unique_ptr<InputMode> mouseHandler(Context &context, sf::Event event);
  std::unique_ptr<InputMode> handleLeftClick(Context &context);

  std::unique_ptr<InputMode> keyPressHandler(Context &context, sf::Event event);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

class MovementMode : public InputMode {
public:
  MovementMode(Context context, std::string selectedName);

  void updateContext(Context context);

  std::unique_ptr<InputMode> eventHandler(Context &context, sf::Event event);

private:
  sf::Vector2i mouseDelta;
  Entity preview;

  std::unique_ptr<InputMode> mouseHandler(Context &context, sf::Event event);
  std::unique_ptr<InputMode> handleLeftClick(Context &context);

  std::unique_ptr<InputMode> keyPressHandler(Context &context, sf::Event event);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
