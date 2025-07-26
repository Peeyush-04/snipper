#ifndef MODELS_SNIPPET_H
#define MODELS_SNIPPET_H

#include <string>
#include <vector>
#include <external/json.hpp>

namespace model {

struct Snippet {
  std::string id;
  std::string title;
  std::string content;
  std::vector<std::string> tags;
  std::string createdAt;  // ISO 8601 format
  std::string updatedAt;

  // Default constructor needed for deserialization & testing
  Snippet() = default;

  // Optional convenience methods
  static Snippet fromJson(const nlohmann::json& j);
  nlohmann::json toJson() const;
};

}  // namespace model

// Serializer support
namespace nlohmann {
  template <>
  struct adl_serializer<model::Snippet> {
    static void to_json(json& j, const model::Snippet& s) {
      j = json{
        {"id", s.id},
        {"title", s.title},
        {"content", s.content},
        {"tags", s.tags},
        {"createdAt", s.createdAt},
        {"updatedAt", s.updatedAt}
      };
    }

    static void from_json(const json& j, model::Snippet& s) {
      j.at("id").get_to(s.id);
      j.at("title").get_to(s.title);
      j.at("content").get_to(s.content);
      j.at("tags").get_to(s.tags);
      j.at("createdAt").get_to(s.createdAt);
      j.at("updatedAt").get_to(s.updatedAt);
    }
  };
}

#endif  // MODELS_SNIPPET_H
