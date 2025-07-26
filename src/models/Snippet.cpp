#include <models/Snippet.h>

namespace model {

Snippet Snippet::fromJson(const nlohmann::json& j) {
  return j.get<Snippet>();  // Uses adl_serializer defined in header
}

nlohmann::json Snippet::toJson() const {
  return nlohmann::json(*this);  // Uses adl_serializer defined in header
}

}  // namespace model
