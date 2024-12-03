//
// Created by spaske on 12/2/24.
//
#include <glad/glad.h>
#include <engine/resources/Texture.hpp>
#include <engine/util/Utils.hpp>
#include <engine/util/Errors.hpp>
#include <stb_image.h>
#include <spdlog/spdlog.h>
#include <format>

namespace rg {

    std::string_view texture_type_to_string(TextureType type) {
        switch (type) {
        case TextureType::Diffuse: return "Diffuse";
        case TextureType::Specular: return "Specular";
        case TextureType::Normal: return "Normal";
        case TextureType::Height: return "Height";
        default: RG_SHOULD_NOT_REACH_HERE("Unknown TextureType");
        }
    }

    void Texture::destroy() {
        glDeleteTextures(1, &m_id);
    }

    std::string_view Texture::uniform_name_convention(TextureType type) {
        switch (type) {
        case TextureType::Diffuse: return "texture_diffuse";
        case TextureType::Specular: return "texture_specular";
        case TextureType::Normal: return "texture_normal";
        case TextureType::Height: return "texture_height";
        default: RG_SHOULD_NOT_REACH_HERE("Unhandled TextureType");
        }
    }

}