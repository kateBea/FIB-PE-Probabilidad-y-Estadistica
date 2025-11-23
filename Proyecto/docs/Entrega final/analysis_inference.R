###############################################################
# Inference analysis for BFS vs DFS experiment
# Author: Hugo Pelayo 
# Purpose: compute descriptive stats, paired t-test, confidence intervals,
#          effect size, simple regression and save outputs & plots.
# Usage: place this script in the same folder as test_report.csv and run:
#        Rscript analysis_inference.R
###############################################################

# 1. Carrega del CSV amb noms intactes
data <- read.csv("test_report.csv",
                 check.names = FALSE,
                 stringsAsFactors = FALSE)

cat("Rows:", nrow(data), " Columns:", ncol(data), "\n")
cat("Detected columns:\n")
print(colnames(data))


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
# 4. Conversió a numèric
###############################################################
to_num <- function(x) {
  if (is.numeric(x)) return(x)
  x <- gsub(",", ".", gsub("\\s+", "", as.character(x)))
  return(as.numeric(x))
}

data[[dfs_col]]     <- to_num(data[[dfs_col]])
data[[bfs_col]]     <- to_num(data[[bfs_col]])
data[[nodes_col]]   <- to_num(data[[nodes_col]])
data[[edges_col]]   <- to_num(data[[edges_col]])
data[[density_col]] <- to_num(data[[density_col]])


###############################################################
# 5. Filtrar files vàlides (totes dues mesures)
###############################################################
paired <- data[!is.na(data[[dfs_col]]) & !is.na(data[[bfs_col]]), ]
n <- nrow(paired)
cat("Paired rows:", n, "\n\n")

if (n < 2) stop("Not enough data for inference.")


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
write.csv(desc, "descriptive_summary.csv", row.names = FALSE)

###############################################################
# 6b. QQ-plots per a normalitat
# Aquesta secció genera QQ-plots per a les columnes DFS i BFS.
# - Objectiu: veure si les dades de temps s'ajusten a una distribució normal.
# - Interpretació:
#   * Si els punts segueixen aproximadament la línia roja, les dades són normalment distribuïdes.
#   * Si hi ha desviacions sistemàtiques (curvatura, cues llargues), les dades no són normals.
# Nota: El QQ-plot comprova l'ajust a la distribució, no si les dades són aleatòries.
###############################################################
png("qqplot_times.png", width = 800, height = 400)
par(mfrow=c(1,2))
qqnorm(paired[[dfs_col]], main="QQ-plot DFS Time (ms)")
qqline(paired[[dfs_col]], col="red", lwd=2)
qqnorm(paired[[bfs_col]], main="QQ-plot BFS Time (ms)")
qqline(paired[[bfs_col]], col="red", lwd=2)
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
  "paired_ttest_results.csv",
  row.names = FALSE
)


###############################################################
# 8. Mida de l’efecte (Cohen's d)
###############################################################
cohen_d <- mean_diff / sd_diff
cat("\nCohen's d:", cohen_d, "\n")

write.csv(
  data.frame(cohen_d = cohen_d),
  "effect_size.csv",
  row.names = FALSE
)


###############################################################
# 9. Intervals de confiança per mitjanes
###############################################################
alpha <- 0.05
tcrit <- qt(1 - alpha/2, df = n - 1)

se_dfs <- sd_dfs / sqrt(n)
se_bfs <- sd_bfs / sqrt(n)

ci_dfs <- c(mean_dfs - tcrit*se_dfs, mean_dfs + tcrit*se_dfs)
ci_bfs <- c(mean_bfs - tcrit*se_bfs, mean_bfs + tcrit*se_bfs)

cis <- data.frame(
  metric = c("DFS_mean", "BFS_mean"),
  mean   = c(mean_dfs, mean_bfs),
  ci_low  = c(ci_dfs[1], ci_bfs[1]),
  ci_high = c(ci_dfs[2], ci_bfs[2])
)
write.csv(cis, "means_confidence_intervals.csv", row.names = FALSE)


###############################################################
# 10. Regressió senzilla corregida amb reformulate()
###############################################################
paired$Diff <- paired[[dfs_col]] - paired[[bfs_col]]

formula_reg <- reformulate(
  termlabels = c(nodes_col, density_col),
  response   = "Diff"
)

reg <- lm(formula_reg, data = paired)
reg_summary <- summary(reg)
print(reg_summary)

capture.output(reg_summary, file = "regression_summary.txt")


###############################################################
# 11. Gràfics base R
###############################################################
png("hist_times.png", width = 1000, height = 400)
par(mfrow=c(1,2))
hist(paired[[bfs_col]], main="BFS time (ms)", xlab="ms", col="lightblue")
hist(paired[[dfs_col]], main="DFS time (ms)", xlab="ms", col="salmon")
dev.off()

png("hist_diff.png", width = 800, height = 600)
hist(paired$Diff, main="DFS - BFS (ms)", xlab="ms", col="lightgreen")
dev.off()

png("density_vs_times.png", width = 800, height = 600)
plot(paired[[density_col]], paired[[bfs_col]],
     col="blue", pch=16, xlab="Density", ylab="Time (ms)")
points(paired[[density_col]], paired[[dfs_col]], col="red", pch=16)
legend("topleft", legend=c("BFS","DFS"), col=c("blue","red"), pch=16)
dev.off()

png("nodes_vs_diff.png", width = 800, height = 600)
plot(paired[[nodes_col]], paired$Diff, pch=16, col="purple",
     xlab="Nodes", ylab="DFS - BFS (ms)",
     main="Nodes vs Diff")
abline(lm(Diff ~ paired[[nodes_col]], data=paired), col="black")
dev.off()


###############################################################
# 12. Missatge final
###############################################################
cat("\nAnalysis complete. Files generated:\n")
print(c(
  "descriptive_summary.csv",
  "paired_ttest_results.csv",
  "effect_size.csv",
  "means_confidence_intervals.csv",
  "regression_summary.txt",
  "hist_times.png",
  "hist_diff.png",
  "density_vs_times.png",
  "nodes_vs_diff.png",
  "qqplot_times.png"
))
###############################################################
