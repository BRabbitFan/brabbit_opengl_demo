#pragma once

#include <brabbit/mesh.hpp>
#include <brabbit/scene_object.hpp>

namespace brabbit {

  class Model : public SceneObject {
   public:
    explicit Model(std::unique_ptr<Mesh>& mesh);
    explicit Model(Mesh* mesh);
    virtual ~Model() override;

   public:
    auto getMesh() const -> const Mesh*;

   protected:
    auto draw() -> void override;

   private:
    Mesh* mesh_{ nullptr };
    unsigned int vertex_vbo_{ 0 };
    unsigned int index_ebo_{ 0 };
    unsigned int normal_vbo_{ 0 };
  };

}  // namespace brabbit
