
#include <fstream>
#include <sstream>
#include <cstdint>

#include <tree_loader.hh>
#include <base/logger.hh>


namespace tester {

    auto tree_loader::load_from_file(const std::filesystem::path uri) -> base::gentree<int> {
        base::gentree<std::int32_t> tree{};

        // Aquí iría la lógica para leer el archivo y cargar los datos en el árbol.
        // Por ahora, simplemente devolvemos un árbol vacío.
        std::ifstream file{ uri };

        if (!file.is_open()) {
            LOG_ERROR("No se pudo abrir el archivo: " + uri.string());
            return tree;
        }

        std::string line{};

        bool reading_edges{ false };

        while (std::getline(file, line)) {

            // saltar lineas vacias
            // aunque no es algo que se espera
            if (line.empty()) {
                continue;
            }

            // las lineas con edges tienes 3 caracteres o más (ej: "1 2")
            reading_edges = line.size() >= 3;

            std::istringstream iss{ line };
            std::int32_t a{};
            std::int32_t b{};

            if (!reading_edges) {
                iss >> a;
                tree.add_vertex(a);
            } else if (iss >> a >> b) {
                tree.add_edge(a, b);
            }
        }

        return tree;
    }
}