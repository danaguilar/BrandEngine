#pragma once

#include <string>
#include "../ECS/ECS.h"

class Tilemap {
  private:
    int tile_size;
    std::string asset_name;

  public: 
    Tilemap(const std::string& asset_name, int tile_size) {
      this->tile_size = tile_size;
      this->asset_name = asset_name;
    }
    void CreateMap(std::unique_ptr<Registry>& registry, const std::string& map_data_file);
};

