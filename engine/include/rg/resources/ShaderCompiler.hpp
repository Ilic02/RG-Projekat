//
// Created by spaske on 12/5/24.
//

#ifndef SHADER_COMPILER_HPP
#define SHADER_COMPILER_HPP
#include <rg/graphics/OpenGL.hpp>
#include <rg/resources/Shader.hpp>
#include <filesystem>
#include <string>

namespace rg::resources {
    enum class ShaderType { Vertex, Fragment, Geometry };

    std::string_view to_string(ShaderType type);

    struct ShaderParsingResult {
        std::string vertex_shader;
        std::string fragment_shader;
        std::string geometry_shader;

        std::string *now_parsing(const std::string &line);
    };

    class ShaderCompiler {
    public:
        static Shader compile_from_source(std::string shader_name, std::string shader_source);

        static Shader compile_from_file(std::string shader_name, const std::filesystem::path &shader_path);

        std::string *now_parsing(ShaderParsingResult &result, const std::string &line);

        ShaderParsingResult parse_source();

        explicit ShaderCompiler(std::string shader_name, std::string shader_source) : m_shader_name(
                std::move(shader_name))
          , m_sources(std::move(shader_source)) {
        }

        graphics::OpenGL::ShaderProgramId compile(const ShaderParsingResult &shader_sources);

    private:
        int compile(const std::string &shader_source, ShaderType type);

        std::string m_shader_name;
        std::string m_sources;
    };
}
#endif //SHADER_COMPILER_HPP