#include "events_handlers.hpp"
#include <iostream>

sf::Vector2i handleLeftClick(std::unordered_map<std::string, Entity> &entities,
                            std::unordered_map<std::string, Entity> &preview,
                            sf::Vector2i mouseHexPos) {
  bool previewMod = false;
  for (auto & entityPair : entities) {
    // --- If the mouse hovers over the entity :
    if (entityPair.second.hoveredByPos(mouseHexPos)) {
      // --- And the entity is selected :
      if (entityPair.second.isSelected()) {
        entityPair.second.unSelect(); // Unselect the entity
        preview.erase(entityPair.second.getName()); // Delete the preview (if the entity is selected, it means there was a preview)
        previewMod = true;  // Preview has been modified, so we won't treat it later
      } else {
        // The entity isnt selected
        entityPair.second.select(); // Select it
        for (auto & previewPair : preview) { // Unselect any selected entities
          std::string name = previewPair.second.getName();
          entities[name].unSelect();
        }
        preview.clear(); // Clear other preview
        // Create new preview
        preview[entityPair.second.getName()] = entityPair.second.getPreviewAt(mouseHexPos.x, mouseHexPos.y);
        return sf::Vector2i{entityPair.second.getPos().x, entityPair.second.getPos().y} - mouseHexPos;
      }
    }
  }

  if (previewMod == false) {
    for (auto & previewPair : preview) {
      std::string name = previewPair.second.getName();
      entities[name] = previewPair.second.getEntityFromPreview();
    }
    preview.clear();
  }
  return sf::Vector2i{0, 0};
}
