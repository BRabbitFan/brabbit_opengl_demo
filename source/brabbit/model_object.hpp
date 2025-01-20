#pragma once

#include <brabbit/model.hpp>
#include <brabbit/scene_object.hpp>

namespace brabbit {

  class ModelObject : public SceneObject {
   public:
    explicit ModelObject(std::unique_ptr<Model>& data);
    explicit ModelObject(Model* data);
    virtual ~ModelObject() override;

   public:
    auto data() const -> const Model*;
    auto setData(Model* data) -> void;

   protected:
    auto draw() -> void override;

   private:
    Model* data_{ nullptr };
    unsigned int vertex_vbo_{ 0 };
    unsigned int index_ebo_{ 0 };
    unsigned int normal_vbo_{ 0 };
  };

}  // namespace brabbit
