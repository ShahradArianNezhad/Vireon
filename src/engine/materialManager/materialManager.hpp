#pragma once
#include "./material/material.hpp"
#include "engine/glyphManager/glyphMap/glyphMap.hpp"
#include "engine/meshManager/meshManager.hpp"
#include "utils/allocator/allocator.hpp"
#include "utils/idManager/idManager.hpp"
#include "utils/logger/logger.hpp"
#include <unordered_map>

using MaterialID = unsigned int;
using MaterialSignature = uint64_t;

class MaterialManager {
private:
  IDManager idManager;
  std::unordered_map<MaterialID, Material> materials;
  std::unordered_map<MaterialSignature,MaterialID> materialCache;

public:
  MaterialManager() {};
  Material &get(MaterialID id) {
    if(!materials.contains(id)) LOG_FATAL("Get material called on non existing id: {}",id);
    return materials[id]; 
  };

  MaterialID newMat() {
    Hasher64 hasher;
    auto sig = hasher.digest();
    if(materialCache.contains(sig)) return materialCache[sig];
    auto id = idManager.get();
    materials.emplace(id,Material{});
#ifdef DEBUG_VERBOSE
    LOG_DEBUG("Material created: {{id:{},{}}}",materials[id],id);
#endif
    materialCache[sig]=id;
    return id;
  }

  MaterialID newMat(std::string texPath) {
    Hasher64 hasher;
    hasher.combine(texPath.c_str(),texPath.size());
    auto sig = hasher.digest();
    if(materialCache.contains(sig)) return materialCache[sig];
    auto id = idManager.get();
    materials.try_emplace(id,texPath);
#ifdef DEBUG_VERBOSE
    LOG_DEBUG("Material created: {{id:{},{}}}",id,materials[id]);
#endif
    materialCache[sig]=id;
    return id;
  }


  MaterialID newMat(GLuint texId){
    Hasher64 hasher;
    hasher.combine(&texId,4);
    auto sig = hasher.digest();
    if(materialCache.contains(sig)) return materialCache[sig];
    auto id = idManager.get();
    materials.try_emplace(id,texId);
#ifdef DEBUG_VERBOSE
    LOG_DEBUG("Material created: {{id:{},{}}}",id,materials[id]);
#endif
    materialCache[sig]=id;
    return id;
  }
};
