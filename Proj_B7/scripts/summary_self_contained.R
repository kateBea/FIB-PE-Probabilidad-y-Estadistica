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
# Llegir i preparar les dades
# ------------------------------------------------------------
cat("Comprovant fitxer test_report.csv al directori de treball...\n")
cat("Directori actual: ", getwd(), "\n")
files <- list.files()
cat("Fitxers al directori:\n")
print(files)

if (!file.exists("test_report.csv")) {
  stop("ERROR: no s'ha trobat 'test_report.csv' al directori de treball. Copia el CSV al directori que mostra 'getwd()' o canvia el working directory.")
}

# Mantenim els noms del CSV per evitar problemes amb espais
data <- read.csv("test_report.csv", check.names = FALSE, stringsAsFactors = FALSE)
cat("Fitxer carregat. Nombre de registres:", nrow(data), "\n\n")

# Mostrar columnes detectades
cat("Columnes detectades:\n")
print(colnames(data))
cat("\nMostra de les primeres files:\n")
print(head(data, n = 6))

# Creacio de la columna Diff amb els noms de columnes reals
data$Diff <- data[["DFS Time (ms)"]] - data[["BFS Time (ms)"]]

# ------------------------------------------------------------
# Estadistiques descriptives
# ------------------------------------------------------------
cat("\n=== Estadistiques descriptives ===\n")
print(summary(data[c("Nodes", "Edges", "Density", "BFS Time (ms)", "DFS Time (ms)", "Diff")]))

# Percentatge de casos on BFS es mes rapid
faster_bfs <- mean(data[["BFS Time (ms)"]] < data[["DFS Time (ms)"]], na.rm = TRUE) * 100
cat(sprintf("\nPercentatge de casos on BFS es mes rapid: %.1f%%\n", faster_bfs))

# ------------------------------------------------------------
# Comprovacio de la densitat
# ------------------------------------------------------------
cat("\n=== Comprovacio de la densitat ===\n")
data$Density_check <- data[["Edges"]] / (data[["Nodes"]] * (data[["Nodes"]] - 1))
cor_density <- cor(data[["Density"]], data$Density_check, use = "complete.obs")
cat(sprintf("Correlacio entre densitat del CSV i densitat recalculada: %.6f\n", cor_density))

# Gràfic: densitat CSV vs densitat recalculada
png(filename = "density_check.png", width = 800, height = 600)
plot(data[["Density"]], data$Density_check,
     main = "Comprovacio de la densitat",
     xlab = "Densitat (al CSV)",
     ylab = "Densitat recalculada",
     col = "blue", pch = 19)
abline(0, 1, col = "red", lwd = 2, lty = 2)
dev.off()
cat("S'ha guardat 'density_check.png' amb el graf de comparacio.\n")

# ------------------------------------------------------------
# Distribucio dels temps BFS i DFS
# ------------------------------------------------------------
png(filename = "hist_times.png", width = 1000, height = 400)
par(mfrow = c(1,2))
hist(data[["BFS Time (ms)"]], main = "Distribucio del temps BFS",
     xlab = "Temps BFS (ms)", col = "skyblue", border = "black")
hist(data[["DFS Time (ms)"]], main = "Distribucio del temps DFS",
     xlab = "Temps DFS (ms)", col = "salmon", border = "black")
par(mfrow = c(1,1))
dev.off()
cat("S'ha guardat 'hist_times.png' amb les histogrames BFS i DFS.\n")

# ------------------------------------------------------------
# Comparacio segons densitat (graf multiplu)
# ------------------------------------------------------------
png(filename = "bfs_vs_dfs_by_density.png", width = 800, height = 600)
plot(data[["Density"]], data[["BFS Time (ms)"]],
     col = "blue", pch = 19,
     xlab = "Densitat del graf",
     ylab = "Temps d'execucio (ms)",
     main = "Comparacio BFS vs DFS segons densitat")
points(data[["Density"]], data[["DFS Time (ms)"]], col = "red", pch = 19)
legend("topleft", legend = c("BFS", "DFS"), col = c("blue", "red"), pch = 19)
dev.off()
cat("S'ha guardat 'bfs_vs_dfs_by_density.png'.\n")

# ------------------------------------------------------------
# Diferencia de temps
# ------------------------------------------------------------
png(filename = "diff_hist.png", width = 800, height = 600)
hist(data$Diff, main = "Diferencia de temps (DFS - BFS)",
     xlab = "Diferencia (ms)", col = "lightgreen", border = "black")
dev.off()
cat("S'ha guardat 'diff_hist.png'.\n")

# ------------------------------------------------------------
# Relacio Nodes - Diferencia
# ------------------------------------------------------------
png(filename = "nodes_vs_diff.png", width = 800, height = 600)
plot(data[["Nodes"]], data$Diff, col = "purple", pch = 19,
     main = "Relacio Nodes vs Diferencia (DFS - BFS)",
     xlab = "Nombre de nodes", ylab = "Diferencia de temps (ms)")
lin <- lm(Diff ~ Nodes, data = data)
abline(lin, col = "black", lwd = 2)
dev.off()
cat("S'ha guardat 'nodes_vs_diff.png'.\n")

# ------------------------------------------------------------
# Correlacions
# ------------------------------------------------------------
cat("\n=== Matriu de correlacions ===\n")
corr_matrix <- cor(data[c("Nodes", "Edges", "Density", "BFS Time (ms)", "DFS Time (ms)", "Diff")], use = "complete.obs")
print(round(corr_matrix, 4))

# ------------------------------------------------------------
# Test t parellat (BFS vs DFS)
# ------------------------------------------------------------
cat("\n=== Test t parellat (BFS vs DFS) ===\n")
t_res <- t.test(data[["DFS Time (ms)"]], data[["BFS Time (ms)"]], paired = TRUE)
print(t_res)
if (!is.na(t_res$p.value) && t_res$p.value < 0.05) {
  cat("Conclusio: Diferencia estadisticament significativa (p < 0.05)\n")
} else {
  cat("Conclusio: No hi ha diferencia significativa (p >= 0.05) o no hi ha prou dades)\n")
}

# ------------------------------------------------------------
# Model lineal simple
# ------------------------------------------------------------
cat("\n=== Model lineal: Diff ~ Nodes + Density ===\n")
model <- lm(Diff ~ Nodes + Density, data = data)
print(summary(model))

cat("\nAnalisi completada. Fitxers generats: density_check.png, hist_times.png, bfs_vs_dfs_by_density.png, diff_hist.png, nodes_vs_diff.png\n")
