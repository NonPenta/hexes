#include <iostream>
#include "events_handlers.hpp"
#include "hex_utils.hpp"
#include "filehandling.hpp"

// ---------------------------- SelectionMode ----------------------------

SelectionMode::SelectionMode(Context context) : mousePos{sf::Mouse::getPosition(context.window)} {}

std::unique_ptr<InputMode> SelectionMode::eventHandler(Context &context, sf::Event event) {
  switch (event.type) {

    case sf::Event::MouseButtonPressed:
      return mouseHandler(context, event);

    case sf::Event::KeyPressed:
      return keyPressHandler(context, event);

    case sf::Event::Closed:
      context.window.close();
      return nullptr;

    default:
      return nullptr;
  }
}

std::unique_ptr<InputMode> SelectionMode::mouseHandler(Context &context, sf::Event event) {
  switch (event.mouseButton.button) {
    case sf::Mouse::Left:
      return handleLeftClick(context);
    default:
      return nullptr;
  }
}
std::unique_ptr<InputMode> SelectionMode::handleLeftClick(Context &context) {
  sf::Vector2i mouseHexPos = hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  // position of the mouse on the window converted to world pos converted to hex coordinates
  for (auto &ePair : context.entities) {
    if (ePair.second.hoveredByPos(mouseHexPos)) {
      return std::make_unique<MovementMode>(context, ePair.first);
    }
  }
  return nullptr; // TODO : return unique_ptr to CreationMode but first make it work
}

std::unique_ptr<InputMode> SelectionMode::keyPressHandler(Context &context, sf::Event event) {
  switch (event.key.code) {
    case sf::Keyboard::Escape:
      save(context.map, context.entities, "test_0");
      context.window.close();  // Save and close the window
      return nullptr;
    default:
      return nullptr;
  }
}

void SelectionMode::updateContext(Context context) { mousePos = hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window))); }
void SelectionMode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(Hex{mousePos.x, mousePos.y, "mousePointer"}, states);
}

// ---------------------------- MovementMode ----------------------------

MovementMode::MovementMode(Context context, std::string selectedName)
  : mouseDelta(context.entities[selectedName].getPos()-hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)))),
    preview(context.entities[selectedName].getPreviewAt(hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window))))) {}

std::unique_ptr<InputMode> MovementMode::eventHandler(Context &context, sf::Event event) {
  switch (event.type) {

    case sf::Event::MouseButtonPressed:
      return mouseHandler(context, event);

    case sf::Event::KeyPressed:
      return keyPressHandler(context, event);

    default:
      return nullptr;
  }
}

std::unique_ptr<InputMode> MovementMode::mouseHandler(Context &context, sf::Event event) {
  switch (event.mouseButton.button) {
    case sf::Mouse::Left:
      return handleLeftClick(context);
    case sf::Mouse::Right:
      return std::make_unique<SelectionMode>(context);
    default:
      return nullptr;
  }
}
std::unique_ptr<InputMode> MovementMode::handleLeftClick(Context &context) {
  sf::Vector2i mouseHexPos = hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  if (context.entities[preview.getName()].hoveredByPos(mouseHexPos)) {
    return std::make_unique<SelectionMode>(context);
  } else {
    context.entities[preview.getName()].moveTo(mouseHexPos+mouseDelta);
    return std::make_unique<SelectionMode>(context);
  }
}

std::unique_ptr<InputMode> MovementMode::keyPressHandler(Context &context, sf::Event event) {
  switch (event.key.code) {
    case sf::Keyboard::Escape:
      return std::make_unique<SelectionMode>(context);
    default:
      return nullptr;
  }
}

void MovementMode::updateContext(Context context) {
  preview.moveTo(hex_from_pix(context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)))+mouseDelta);
  preview.setView(context.view);
}

void MovementMode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(preview, states);
}
