###############################################################
# Anàlisi inferencial de BFS vs DFS en funció de la profunditat
# Autor: Hugo Pelayo, Sergio Borque i Daniel Hergueta
#
# Objectiu de l’estudi:
#   Analitzar si l’algorisme DFS és més ràpid que BFS
#   quan augmenta la profunditat de cerca en el graf.
###############################################################

###############################################################
# 0. Càrrega segura de llibreries necessàries
# (només s’instal·len si no estan disponibles)
###############################################################
if (!requireNamespace("ggplot2", quietly = TRUE)) {
  install.packages("ggplot2")
}
if (!requireNamespace("reshape2", quietly = TRUE)) {
  install.packages("reshape2")
}

library(ggplot2)
library(reshape2)

###############################################################
# 1. Carregar el fitxer CSV
# El fitxer test_report.csv ha d’estar al mateix directori
# que l’script.
###############################################################
data <- read.csv(
  "report.csv",
  check.names = FALSE,
  stringsAsFactors = FALSE
)

cat("Files:", nrow(data), " Columnes:", ncol(data), "\n")
print(colnames(data))

###############################################################
# 2. Funció auxiliar per detectar automàticament columnes
# segons un patró de text (sense modificar els noms originals)
###############################################################
find_col <- function(pattern) {
  cols <- colnames(data)
  m <- grep(pattern, cols, ignore.case = TRUE)
  if (length(m) == 0) stop(paste("Columna no trobada:", pattern))
  cols[m[1]]
}

###############################################################
# 3. Identificació de columnes rellevants
###############################################################
dfs_col        <- find_col("DFS Time")
bfs_col        <- find_col("BFS Time")
nodes_col      <- find_col("Nodes")
edges_col      <- find_col("Edges")
density_col    <- find_col("Density")
dfs_depth_col  <- find_col("DFS Path Length")
bfs_depth_col  <- find_col("BFS Path Length")

###############################################################
# 4. Filtrar només observacions aparellades
# (files amb temps BFS i DFS disponibles)
###############################################################
paired <- data[
  !is.na(data[[dfs_col]]) & !is.na(data[[bfs_col]]),
]

###############################################################
# 5. Variables derivades
###############################################################

# Diferència de temps entre DFS i BFS
# Valor negatiu -> DFS més ràpid
# Valor positiu -> BFS més ràpid
paired$Diff <- paired[[dfs_col]] - paired[[bfs_col]]

# Profunditat mitjana de la cerca
# Variable clau de l’estudi
paired$Depth <- (
  paired[[dfs_depth_col]] + paired[[bfs_depth_col]]
) / 2

###############################################################
# 6. Histogrames de totes les variables
# Exploració inicial de les distribucions per validar normalitat
###############################################################
png("6_histograms_all_variables.png", width = 1200, height = 800)

par(mfrow = c(2, 4))

hist(paired[[dfs_col]],  col="#4C72B0", main="Temps DFS",  xlab="ms")
hist(paired[[bfs_col]],  col="#DD8452", main="Temps BFS",  xlab="ms")
hist(paired$Diff,        col="#55A868", main="DFS - BFS",  xlab="ms")

hist(paired[[bfs_depth_col]], col="#8172B2",
     main="Profunditat BFS", xlab="nivells")
hist(paired[[dfs_depth_col]], col="#8172B2",
     main="Profunditat DFS", xlab="nivells")

hist(paired[[nodes_col]],   col="#C44E52", main="Nodes",   xlab="")
hist(paired[[edges_col]],   col="#937860", main="Arestes", xlab="")
hist(paired[[density_col]], col="#8C8C8C", main="Densitat", xlab="")

dev.off()

###############################################################
# 6b. Gràfic de Bland–Altman
# Validació del tipus de diferència (additiva vs multiplicativa)
###############################################################

# Mitjana dels temps d’execució dels dos algorismes
paired$MeanTime <- (
  paired[[dfs_col]] + paired[[bfs_col]]
) / 2

png("6b_bland_altman_bfs_vs_dfs.png", width = 800, height = 600)

plot(
  paired$MeanTime,
  paired$Diff,
  pch = 16,
  col = rgb(44, 123, 182, 80, maxColorValue = 255),
  xlab = "Mitjana del temps (DFS i BFS) [ms]",
  ylab = "Diferència (DFS − BFS) [ms]",
  main = "Gràfic de Bland–Altman: BFS vs DFS"
)

# Mitjana de la diferència
abline(
  h = mean(paired$Diff),
  col = "blue",
  lwd = 2,
  lty = 2
)

# Límits d’acord (± 1.96 desviacions estàndard)
abline(
  h = mean(paired$Diff) + 1.96 * sd(paired$Diff),
  col = "red",
  lwd = 2,
  lty = 3
)
abline(
  h = mean(paired$Diff) - 1.96 * sd(paired$Diff),
  col = "red",
  lwd = 2,
  lty = 3
)

# Línia de referència (igualtat)
abline(h = 0, col = "black", lwd = 2)

dev.off()

###############################################################
# 7. Gràfic exploratori: diferència vs profunditat
###############################################################
png("7_diff_vs_depth.png", width = 800, height = 600)

plot(
  paired$Depth,
  paired$Diff,
  pch = 16,
  col = rgb(44, 123, 182, 80, maxColorValue = 255),
  xlab = "Profunditat mitjana de la cerca",
  ylab = "DFS - BFS (ms)",
  main = "Diferència de temps segons la profunditat"
)

abline(h = 0, col = "red", lwd = 2, lty = 2)
abline(lm(Diff ~ Depth, data = paired), col = "black", lwd = 2)

dev.off()

###############################################################
# 8. Model de regressió lineal
# S’analitza si la profunditat explica la diferència de temps
###############################################################
depth_model <- lm(Diff ~ Depth, data = paired)
summary(depth_model)

capture.output(
  summary(depth_model),
  file = "8_regression_diff_vs_depth.txt"
)

###############################################################
# 9. Boxplot BFS vs DFS agrupat per quartils de profunditat
###############################################################

# Crear grups de profunditat (quartils)
paired$DepthGroup <- cut(
  paired$Depth,
  breaks = quantile(paired$Depth, probs = seq(0, 1, 0.25)),
  include.lowest = TRUE
)

# Convertir dades a format llarg
long_times <- melt(
  paired,
  id.vars = "DepthGroup",
  measure.vars = c(dfs_col, bfs_col),
  variable.name = "Algorisme",
  value.name = "Temps"
)

png("9_boxplot_bfs_dfs_by_depth.png", width = 900, height = 600)

ggplot(
  long_times,
  aes(x = DepthGroup, y = Temps, fill = Algorisme)
) +
  geom_boxplot(alpha = 0.7, outlier.size = 0.8) +
  labs(
    title = "Temps d'execució BFS vs DFS segons la profunditat",
    x = "Grup de profunditat (quartils)",
    y = "Temps d'execució (ms)",
    fill = "Algorisme"
  ) +
  theme_minimal()

dev.off()

###############################################################
# 10. Test t aparellat (comparació global BFS vs DFS)
###############################################################
tres <- t.test(
  paired[[dfs_col]],
  paired[[bfs_col]],
  paired = TRUE
)

print(tres)

# Guardar la sortida del test en un fitxer de text
capture.output(
  print(tres),
  file = "10_paired_t_test_output.txt"
)

# Guardar els resultats numèrics en CSV
write.csv(
  data.frame(
    mean_DFS = mean(paired[[dfs_col]]),
    mean_BFS = mean(paired[[bfs_col]]),
    mean_diff = mean(paired$Diff),
    t_stat = tres$statistic,
    df = tres$parameter,
    p_value = tres$p.value,
    ci_low = tres$conf.int[1],
    ci_high = tres$conf.int[2]
  ),
  "10_paired_t_test_results.csv",
  row.names = FALSE
)

###############################################################
# FI DE L’SCRIPT
###############################################################
