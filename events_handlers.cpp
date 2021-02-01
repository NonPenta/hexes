#include "events_handlers.hpp"
#include "filehandling.hpp"
#include "hex_utils.hpp"
#include <iostream>

// ---------------------------- SelectionMode ----------------------------

SelectionMode::SelectionMode(Context context)
    : mousePos{sf::Mouse::getPosition(context.window)} {}

std::unique_ptr<InputMode> SelectionMode::handleEvent(Context &context,
                                                      sf::Event event) {
  switch (event.type) {

  case sf::Event::MouseButtonPressed:
    return handleMouseEvent(context, event);

  case sf::Event::KeyPressed:
    return handleKeyPress(context, event);

  case sf::Event::Closed:
    context.window.close();
    return nullptr;

  default:
    return nullptr;
  }
}

std::unique_ptr<InputMode> SelectionMode::handleMouseEvent(Context &context,
                                                           sf::Event event) {
  switch (event.mouseButton.button) {
  case sf::Mouse::Left:
    return handleLeftClick(context);
  default:
    return nullptr;
  }
}
std::unique_ptr<InputMode> SelectionMode::handleLeftClick(Context &context) {
  sf::Vector2i mouseHexPos = hex_from_pix(
      context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  // position of the mouse on the window converted to world pos converted to hex
  // coordinates
  for (auto &ePair : context.entities) {
    if (ePair.second.hoveredByPos(mouseHexPos)) {
      return std::make_unique<MovementMode>(context, ePair.first);
    }
  }
  return std::make_unique<CreationMode>(context);
}

std::unique_ptr<InputMode> SelectionMode::handleKeyPress(Context &context,
                                                         sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Escape:
    save(context.map, context.entities, "test_0");
    context.window.close(); // Save and close the window
    return nullptr;
  default:
    return nullptr;
  }
}

void SelectionMode::updateContext(Context context) {
  mousePos = hex_from_pix(
      context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
}
void SelectionMode::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const {
  target.draw(Hex{mousePos.x, mousePos.y, "mousePointer"}, states);
}

// ---------------------------- MovementMode ----------------------------

MovementMode::MovementMode(Context context, std::string selectedName)
    : mouseDelta(context.entities[selectedName].getPos() -
                 hex_from_pix(context.window.mapPixelToCoords(
                     sf::Mouse::getPosition(context.window)))),
      preview(context.entities[selectedName].getPreviewAt(
          hex_from_pix(context.window.mapPixelToCoords(
              sf::Mouse::getPosition(context.window))))) {}

std::unique_ptr<InputMode> MovementMode::handleEvent(Context &context,
                                                     sf::Event event) {
  switch (event.type) {

  case sf::Event::MouseButtonPressed:
    return handleMouseEvent(context, event);

  case sf::Event::KeyPressed:
    return handleKeyPress(context, event);

  default:
    return nullptr;
  }
}

std::unique_ptr<InputMode> MovementMode::handleMouseEvent(Context &context,
                                                          sf::Event event) {
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
  sf::Vector2i mouseHexPos = hex_from_pix(
      context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  if (context.entities[preview.getName()].hoveredByPos(mouseHexPos)) {
    return std::make_unique<SelectionMode>(context);
  } else {
    context.entities[preview.getName()].moveTo(mouseHexPos + mouseDelta);
    return std::make_unique<SelectionMode>(context);
  }
}

std::unique_ptr<InputMode> MovementMode::handleKeyPress(Context &context,
                                                        sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Escape:
    return std::make_unique<SelectionMode>(context);
  case sf::Keyboard::Delete:
    context.entities.erase(preview.getName());
    return std::make_unique<SelectionMode>(context);
  default:
    return nullptr;
  }
}

void MovementMode::updateContext(Context context) {
  preview.moveTo(hex_from_pix(context.window.mapPixelToCoords(
                     sf::Mouse::getPosition(context.window))) +
                 mouseDelta);
  preview.setView(context.view);
}

void MovementMode::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(preview, states);
}

// ---------------------------- CreationMode ----------------------------
CreationMode::CreationMode(Context context) {
  sf::Vector2i anchor = hex_from_pix(
      context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  preview = Entity(anchor.x, anchor.y, "previewplayer", 0, 0);
}

void CreationMode::updateContext(Context context) {
  sf::Vector2i sizeOrigin = hex_from_pix(
      context.window.mapPixelToCoords(sf::Mouse::getPosition(context.window)));
  sf::Vector2i ppos = preview.getPos();
  sf::Vector2i baseDelta;
  baseDelta.x = (sizeOrigin.x - ppos.x >= 0);
  baseDelta.y = (sizeOrigin.y - ppos.y >= 0);

  if ((baseDelta.x + sizeOrigin.x - ppos.x != preview.getWidth()) ||
      (baseDelta.y + sizeOrigin.y - ppos.y != preview.getHeight())) {
    preview = Entity(preview.getName(), ppos.x, ppos.y, preview.getType(),
                     baseDelta.x + sizeOrigin.x - ppos.x,
                     baseDelta.y + sizeOrigin.y -
                         ppos.y); // shit idk where that 1 is coming from
                                  // WAIT ik : when theyre the same : it should
                                  // be (1,1) so : (1,1)+(whatever)
  }
  preview.setView(context.view);
}

std::unique_ptr<InputMode> CreationMode::handleEvent(Context &context,
                                                     sf::Event event) {
  switch (event.type) {

  case sf::Event::MouseButtonPressed:
    return handleMouseEvent(context, event);

  case sf::Event::KeyPressed:
    return handleKeyPress(context, event);

  default:
    return nullptr;
  }
}

std::unique_ptr<InputMode> CreationMode::handleMouseEvent(Context &context,
                                                          sf::Event event) {
  switch (event.mouseButton.button) {
  case sf::Mouse::Left:
    updateContext(context);
    context.entities[preview.getName()] = preview.getEntityFromPreview();
    return std::make_unique<SelectionMode>(context);

  case sf::Mouse::Right:
    typeId = (typeId + 1) % 4;
    preview.setType(previewTypes[typeId]);
    return nullptr;
  default:
    return nullptr;
  }
}

std::unique_ptr<InputMode> CreationMode::handleKeyPress(Context &context,
                                                        sf::Event event) {
  switch (event.key.code) {
  case sf::Keyboard::Escape:
    return std::make_unique<SelectionMode>(context);
  default:
    return nullptr;
  }
}

void CreationMode::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  target.draw(preview, states);
}
