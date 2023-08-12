#include "Tilemap.h"
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <string>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>

void Tilemap::CreateMap(std::unique_ptr<Registry>& registry, const std::string& map_data_file) {
  // Load tilemap data from given filepath
  std::fstream tilemap(map_data_file);
  if (tilemap.is_open()) {
    std::string filedata;
    int row = 0;
    // For every line in tilemap data
    while(std::getline(tilemap, filedata)) {
      std::stringstream tilemapRow(filedata);
      std::string rowdata;
      int col = 0;
      // For every comma seperated value within that row
      while(std::getline(tilemapRow, rowdata, ',')) {
       int tiledata = stoi(rowdata);
       // Create a new tile entity
       Entity tile = registry -> CreateEntity();
       // Create a Sprite based off the tilemap, rendering only a tile_size square at on the given row and column based on the found tiledata
       tile.AddComponent<SpriteComponent>(asset_name, tile_size, tile_size, 0, (tiledata%10) * tile_size, (tiledata/10) * tile_size);
       // Set the location of the entity based on where the tiledate was located in the file
       tile.AddComponent<TransformComponent>(glm::vec2(col*tile_size, row*tile_size), glm::vec2(1.0,1.0), 0.0);
       col++;
     }
      row++;
    }
  }
  else {
    Logger::Err("Cannot Load map data");
  }
  tilemap.close();
}
