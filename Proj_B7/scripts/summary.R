###############################################################
#  Analysis of BFS vs DFS Efficiency
#  Autor: Hugo Pelayo
#  Assignatura: Probabilitat i Estadística (FIB)
#  Data: Novembre 2025
#
#  Objectiu:
#    Analitzar i visualitzar l’eficiència relativa dels algorismes
#    de cerca Breadth-First Search (BFS) i Depth-First Search (DFS)
#    a partir de les dades recollides experimentalment.
#
#  Fitxer d'entrada:
#    test_report.csv — amb columnes:
#    Graph.Name, Density, Nodes, Edges,
#    DFS.Time.ms., BFS.Time.ms., DFS.Path.Length, BFS.Path.Length,
#    Start.Node, End.Node
#
#  Sortides:
#    - Estadístiques descriptives
#    - Test de comparació parellat (t-test)
#    - Models de regressió
#    - Gràfics comparatius i correlacionals
###############################################################
# ------------------------------------------------------------
# Instal·lació automàtica de llibreries
# ------------------------------------------------------------
install_if_missing <- function(pkg) {
  if (!requireNamespace(pkg, quietly = TRUE)) {
    message(sprintf("Instal·lant el paquet '%s'...", pkg))
    install.packages(pkg, dependencies = TRUE)
  }
}

for (p in c("ggplot2", "dplyr")) install_if_missing(p)
library(ggplot2)
library(dplyr)

# ------------------------------------------------------------
# Llegir i preparar les dades
# ------------------------------------------------------------
cat("Comprovant fitxer test_report.csv...\n")
if (!file.exists("test_report.csv")) {
  stop("ERROR: No s'ha trobat 'test_report.csv' al directori de treball. Usa getwd() per comprovar-ho.")
}

# Mantenim els noms del CSV per evitar problemes amb espais
data <- read.csv("test_report.csv",  check.names = FALSE, stringsAsFactors = FALSE)
cat("Fitxer carregat correctament. Nombre de registres:", nrow(data), "\n\n")

cat("Columnes detectades:\n")
print(colnames(data))

# Crear columna de diferència de temps (DFS - BFS)
data <- data %>%
  mutate(Diff = `DFS Time (ms)` - `BFS Time (ms)`)

# ------------------------------------------------------------
# Estadístiques descriptives
# ------------------------------------------------------------
cat("\n=== Estadístiques descriptives ===\n")
print(summary(select(data, Nodes, Edges, Density, `BFS Time (ms)`, `DFS Time (ms)`, Diff)))

# Percentatge de casos on BFS és més ràpid
faster_bfs <- mean(data$`BFS Time (ms)` < data$`DFS Time (ms)`, na.rm = TRUE) * 100
cat(sprintf("\nPercentatge de casos on BFS és més ràpid: %.1f%%\n", faster_bfs))

# ------------------------------------------------------------
# Comprovació de la densitat
# ------------------------------------------------------------
cat("\n=== Comprovació de la densitat ===\n")
data <- data %>%
  mutate(Density_check = Edges / (Nodes * (Nodes - 1)))

cor_density <- cor(data$Density, data$Density_check, use = "complete.obs")
cat(sprintf("Correlació entre densitat del CSV i densitat recalculada: %.6f\n", cor_density))

ggplot(data, aes(x = Density, y = Density_check)) +
  geom_point(color = "darkblue") +
  geom_abline(intercept = 0, slope = 1, color = "red", linetype = "dashed") +
  labs(
    title = "Comprovació de la densitat",
    x = "Densitat (al CSV)",
    y = "Densitat recalculada"
  ) +
  theme_minimal()

# ------------------------------------------------------------
# Distribució dels temps BFS i DFS
# ------------------------------------------------------------
ggplot(data, aes(x = `BFS Time (ms)`)) +
  geom_histogram(fill = "skyblue", color = "black", bins = 20) +
  labs(title = "Distribució del temps BFS", x = "Temps BFS (ms)", y = "Freqüència") +
  theme_minimal()

ggplot(data, aes(x = `DFS Time (ms)`)) +
  geom_histogram(fill = "salmon", color = "black", bins = 20) +
  labs(title = "Distribució del temps DFS", x = "Temps DFS (ms)", y = "Freqüència") +
  theme_minimal()

# ------------------------------------------------------------
# Comparació BFS vs DFS segons la densitat
# ------------------------------------------------------------
ggplot(data, aes(x = Density)) +
  geom_point(aes(y = `BFS Time (ms)`, color = "BFS")) +
  geom_point(aes(y = `DFS Time (ms)`, color = "DFS")) +
  labs(
    title = "Comparació BFS vs DFS segons la densitat",
    x = "Densitat del graf",
    y = "Temps d'execució (ms)",
    color = "Algorisme"
  ) +
  theme_minimal()

# ------------------------------------------------------------
# Diferència de temps
# ------------------------------------------------------------
ggplot(data, aes(x = Diff)) +
  geom_histogram(fill = "lightgreen", color = "black", bins = 20) +
  labs(
    title = "Distribució de la diferència de temps (DFS - BFS)",
    x = "Diferència (ms)",
    y = "Freqüència"
  ) +
  theme_minimal()

# ------------------------------------------------------------
# Relació entre nombre de nodes i diferència
# ------------------------------------------------------------
ggplot(data, aes(x = Nodes, y = Diff)) +
  geom_point(color = "purple") +
  geom_smooth(method = "lm", se = FALSE, color = "black") +
  labs(
    title = "Relació entre nombre de nodes i diferència (DFS - BFS)",
    x = "Nombre de nodes",
    y = "Diferència de temps (ms)"
  ) +
  theme_minimal()

# ------------------------------------------------------------
# Correlacions
# ------------------------------------------------------------
cat("\n=== Matriu de correlacions ===\n")
corr_matrix <- cor(select(data, Nodes, Edges, Density, `BFS Time (ms)`, `DFS Time (ms)`, Diff), use = "complete.obs")
print(round(corr_matrix, 4))

# ------------------------------------------------------------
# Test t parellat BFS vs DFS
# ------------------------------------------------------------
cat("\n=== Test t parellat BFS vs DFS ===\n")
test_result <- t.test(data$`DFS Time (ms)`, data$`BFS Time (ms)`, paired = TRUE)
print(test_result)

if (!is.na(test_result$p.value) && test_result$p.value < 0.05) {
  cat("Conclusió: Diferència estadísticament significativa (p < 0.05)\n")
} else {
  cat("Conclusió: No hi ha diferència significativa (p >= 0.05)\n")
}

# ------------------------------------------------------------
# Model lineal simple
# ------------------------------------------------------------
cat("\n=== Model lineal: Diff ~ Nodes + Density ===\n")
model <- lm(Diff ~ Nodes + Density, data = data)
print(summary(model))

cat("\nAnàlisi completada correctament\n")
###############################################################
# Fi de l'script
###############################################################

