###############################################################
# Inference analysis for BFS vs DFS experiment
# Author: Hugo Pelayo
# Purpose: compute descriptive stats, paired t-test, confidence intervals,
#          effect size, simple regression and save outputs & plots.
# Usage: place this script in the same folder as test_report.csv and run:
#        Rscript analysis_inference.R
###############################################################

###############################################################
# 1. Carrega del CSV amb noms intactes ignora workspace,
# el csv ha d'estar on és l'script
###############################################################
data <- read.csv("test_report.csv",
                 check.names = FALSE,
                 stringsAsFactors = FALSE)

cat("Rows:", nrow(data), " Columns:", ncol(data), "\n")
cat("Detected columns:\n")
print(colnames(data))

###############################################################
# 1.1 Resums
###############################################################
dim(data)
names(data)
head(data)

###############################################################
# 2. Funció auxiliar per detectar columnes sense modificar noms
###############################################################
find_col <- function(patterns) {
  cols <- colnames(data)
  for (p in patterns) {
    m <- grep(p, cols, ignore.case = TRUE, perl = TRUE)
    if (length(m) > 0) return(cols[m[1]])
  }
  return(NA)
}


###############################################################
# 3. Identificació AUTOMÀTICA de columnes
###############################################################
dfs_col     <- find_col(c("DFS Time"))
bfs_col     <- find_col(c("BFS Time"))
nodes_col   <- find_col(c("Nodes"))
edges_col   <- find_col(c("Edges"))
density_col <- find_col(c("Density"))

cat("\nUsing columns:\n")
cat(" DFS =", dfs_col, "\n")
cat(" BFS =", bfs_col, "\n")
cat(" Nodes =", nodes_col, "\n")
cat(" Edges =", edges_col, "\n")
cat(" Density =", density_col, "\n\n")

if (any(is.na(c(dfs_col, bfs_col, nodes_col, edges_col, density_col)))) {
  stop("ERROR: No s'han trobat totes les columnes necessàries.")
}

###############################################################
# 5. Filtrar files vàlides (totes dues mesures)
###############################################################
# Abans de realitzar la comparació estadística, s’han eliminat totes les
# observacions incompletes, mantenint únicament aquelles proves per a les
# quals es disposava tant del temps d’execució de BFS com del de DFS.
# Aquesta selecció garanteix que les dades utilitzades en l’anàlisi
# siguin correctament emparellades.

valid_dfs <- !is.na(data[[dfs_col]])
valid_bfs <- !is.na(data[[bfs_col]])

paired <- data[valid_dfs & valid_bfs, ]

n <- nrow(paired)
cat("Paired rows:", n, "\n\n")

if (n < 2) {
  stop("Not enough data for inference.")
}

# Afegim la variable Diff que ens indica la diferència de temps
# entre DFS i BFS per a cada prova emparellada, per cada fila.
# Si el valor és negatiu significa que DFS ha estat més ràpid que BFS
# (temps DFS < temps BFS) i si és positiu (temps DFS > temps BFS)
# significa que BFS ha estat més ràpid que DFS per a aquesta prova concreta.
paired$Diff <- paired[[dfs_col]] - paired[[bfs_col]]

###############################################################
# 6. Estadística descriptiva
###############################################################
mean_dfs <- mean(paired[[dfs_col]])
mean_bfs <- mean(paired[[bfs_col]])
sd_dfs   <- sd(paired[[dfs_col]])
sd_bfs   <- sd(paired[[bfs_col]])

mean_diff <- mean(paired[[dfs_col]] - paired[[bfs_col]])
sd_diff   <- sd(paired[[dfs_col]] - paired[[bfs_col]])

cat("Mean DFS:", mean_dfs, "\n")
cat("Mean BFS:", mean_bfs, "\n")
cat("Mean (DFS - BFS):", mean_diff, "\n\n")

desc <- data.frame(
  metric = c("mean", "sd"),
  DFS = c(mean_dfs, sd_dfs),
  BFS = c(mean_bfs, sd_bfs)
)
write.csv(desc, "6. descriptive_summary.csv", row.names = FALSE)

###############################################################
# 6b. QQ-plots per a normalitat
# Aquesta secció genera QQ-plots per a les columnes DFS i BFS.
# - Objectiu: veure si les dades de temps s'ajusten a una distribució normal.
# - Interpretació:
#   * Si els punts segueixen aproximadament la línia roja, les dades són normalment distribuïdes.
#   * Si hi ha desviacions sistemàtiques (curvatura, cues llargues), les dades no són normals.
# Nota: El QQ-plot comprova l'ajust a la distribució, no si les dades són aleatòries.
###############################################################
png("6b. qqplot_times.png", width = 1200, height = 400)

# Tres gràfics en una sola fila
par(mfrow = c(1, 3))

# QQ-plot DFS
qqnorm(paired[[dfs_col]], main = "QQ-plot DFS Time (ms)")
qqline(paired[[dfs_col]], col = "red", lwd = 2)

# QQ-plot BFS
qqnorm(paired[[bfs_col]], main = "QQ-plot BFS Time (ms)")
qqline(paired[[bfs_col]], col = "red", lwd = 2)

# QQ-plot Diff
qqnorm(paired$Diff, main = "QQ-plot Diff (DFS - BFS)")
qqline(paired$Diff, col = "red", lwd = 2)

dev.off()


###############################################################
# 7. Test t parellat + Interval de confiança
###############################################################
tres <- t.test(paired[[dfs_col]], paired[[bfs_col]], paired = TRUE, conf.level = 0.95)

cat("Paired t-test:\n")
print(tres)

ci_low  <- tres$conf.int[1]
ci_high <- tres$conf.int[2]

write.csv(
  data.frame(
    mean_DFS  = mean_dfs,
    mean_BFS  = mean_bfs,
    mean_diff = mean_diff,
    t_stat    = tres$statistic,
    df        = tres$parameter,
    p_value   = tres$p.value,
    ci_low    = ci_low,
    ci_high   = ci_high
  ),
  "7. paired_ttest_results.csv",
  row.names = FALSE
)

###############################################################
# 8. Intervals de confiança per mitjanes
# En aquesta secció es calculen intervals de confiança del 95%
# per a les mitjanes dels temps d’execució de DFS i BFS
# de manera independent
###############################################################

# Nivell de significació alpha
# alpha = 0.05 correspon a un interval de confiança del 95%
alpha <- 0.05

# Valor crític de la distribució t de Student
# Es fa servir la distribució t perquè la desviació estàndard
# poblacional és desconeguda i la mida de la mostra és finita
# df = n - 1 són els graus de llibertat
tcrit <- qt(1 - alpha/2, df = n - 1)

# Error estàndard de la mitjana del temps DFS
# Mesura la precisió de l’estimació de la mitjana
se_dfs <- sd_dfs / sqrt(n)

# Error estàndard de la mitjana del temps BFS
se_bfs <- sd_bfs / sqrt(n)

# Càlcul de l’interval de confiança del 95% per a DFS
# Fórmula: mitjana ± t * error estàndard
ci_dfs <- c(
  mean_dfs - tcrit * se_dfs,
  mean_dfs + tcrit * se_dfs
)

# Càlcul de l’interval de confiança del 95% per a BFS
ci_bfs <- c(
  mean_bfs - tcrit * se_bfs,
  mean_bfs + tcrit * se_bfs
)

# Creació d’un data frame amb els resultats
# Inclou la mitjana i els límits inferior i superior
# de l’interval de confiança per a cada algorisme
cis <- data.frame(
  metric  = c("DFS_mean", "BFS_mean"),
  mean    = c(mean_dfs, mean_bfs),
  ci_low  = c(ci_dfs[1], ci_bfs[1]),
  ci_high = c(ci_dfs[2], ci_bfs[2])
)

# Guardar els intervals de confiança en un fitxer CSV
# per a la seva posterior consulta o inclusió en l’informe
write.csv(
  cis,
  "8. means_confidence_intervals.csv",
  row.names = FALSE
)

###############################################################
# 9. Regressió senzilla corregida amb reformulate()
###############################################################
formula_reg <- reformulate(
  termlabels = c(nodes_col, density_col),
  response   = "Diff"
)

reg <- lm(formula_reg, data = paired)
reg_summary <- summary(reg)
print(reg_summary)

capture.output(reg_summary, file = "9. regression_summary.txt")


###############################################################
# 10. Gràfics base R
###############################################################
vars <- list(
  BFS_Time = paired[[bfs_col]],
  DFS_Time = paired[[dfs_col]],
  Diff     = paired$Diff,
  Nodes    = paired[[nodes_col]],
  Edges    = paired[[edges_col]],
  Density  = paired[[density_col]]
)

# Colores fijos por variable
colors <- c(
  BFS_Time = "#4C72B0",   # azul
  DFS_Time = "#DD8452",   # naranja
  Diff     = "#55A868",   # verde
  Nodes    = "#C44E52",   # rojo
  Edges    = "#8172B2",   # violeta
  Density  = "#937860"    # marrón
)

png("10. hist_all_variables.png", width = 1200, height = 800)
par(mfrow = c(2, 3))  # 2 filas x 3 columnas

for (name in names(vars)) {
  hist(
    vars[[name]],
    main = paste("Histogram of", name),
    xlab = name,
    col  = colors[[name]],
    border = "#131111"
  )
}
dev.off()

png("10.1 hist_times.png", width = 1000, height = 400)
par(mfrow=c(1,2))
hist(paired[[bfs_col]], main="BFS time (ms)", xlab="ms", col="lightblue")
hist(paired[[dfs_col]], main="DFS time (ms)", xlab="ms", col="salmon")
dev.off()

png("10.2 hist_diff.png", width = 800, height = 600)
hist(paired$Diff, main="DFS - BFS (ms)", xlab="ms", col="lightgreen")
dev.off()

png("10.3 density_vs_times.png", width = 800, height = 600)
plot(paired[[density_col]], paired[[bfs_col]],
     col="blue", pch=16, xlab="Density", ylab="Time (ms)")
points(paired[[density_col]], paired[[dfs_col]], col="red", pch=16)
legend("topleft", legend=c("BFS","DFS"), col=c("blue","red"), pch=16)
dev.off()

png("10.4 nodes_vs_diff.png", width = 800, height = 600)
plot(paired[[nodes_col]], paired$Diff, pch=16, col="purple",
     xlab="Nodes", ylab="DFS - BFS (ms)",
     main="Nodes vs Diff")
abline(lm(Diff ~ paired[[nodes_col]], data=paired), col="black")
dev.off()

png("10.5 boxplot_bfs_vs_dfs.png", width = 800, height = 600)

boxplot(
  paired[[bfs_col]],
  paired[[dfs_col]],
  names = c("BFS", "DFS"),
  col = c("#4C72B0", "#DD8452"),
  ylab = "Time (ms)",
  main = "Comparació dels temps d'execució BFS i DFS"
)

dev.off()

png("10.6 boxplot_diff.png", width = 600, height = 600)

boxplot(
  paired$Diff,
  col = "#55A868",
  ylab = "DFS - BFS (ms)",
  main = "Distribució de la diferència de temps"
)

abline(h = 0, col = "red", lwd = 2, lty = 2)

dev.off()