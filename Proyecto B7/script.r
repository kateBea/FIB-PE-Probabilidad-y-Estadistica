data <- read.csv("C:/Users/zanet/Documents/Code/CandC++/b7_proj/data_frame.csv")
Ya <- data$BFS.procedure..µs.
Yb <- data$DFS.procedure..µs.
Difference <- Ya - Yb
nodes <- data$Graph.Degree....node


#Histogramas de muestras poblacionales
##############################################################
#Histograma de la población de tiempos de ejecución del algoritmo BFS
hist(data$BFS.procedure..µs., main = "BFS procedure", xlab = 
"Execution time", ylab = "Frequency", col = "#FFB74D", border = "#E65100")
#Histograma de la población de tiempos de ejecución del algoritmo DFS
hist(data$DFS.procedure..µs., main = "DFS procedure", 
xlab = "Execution time", ylab = "Frequency", col = "#E57373", border = "#B71C1C")  
#Histograma de la población diferencia entre tiempos de ejecución de ambos algoritmos
hist(Difference, main = "Ya-Yb", xlab = "Execution time", 
ylab = "Frequency", col = "#69F0AE", border = "#00C853")


#Gráficos Q-Q Norm de las muestras
##############################################################
qqnorm(Ya, main = "Normal Q-Q Plot:\n Execution time BFS")
qqline(Ya, col=2)

qqnorm(Yb, main = "Normal Q-Q Plot:\n Execution time DFS")
qqline(Yb, col=2)

#Boxplot para las muestras poblacionales
##############################################################
boxplot(Ya, Yb)

#Gráfico modelo lineal
##############################################################
z1 <- lm(Ya~nodes)
z2 <- lm(Yb~nodes)
par(cex.lab=1.2,cex.axis=1.2,las=1,font.lab=2,font.axis=3)
plot(data$BFS.procedure..µs.~nodes,pch=19,col=4,cex=1.2, ylim=c(0,150))
abline(z1,col=2,lwd=3)
plot(data$DFS.procedure..µs.~nodes,pch=19,col=1,cex=1.2)
abline(z2,col=2,lwd=3)


#Cálculos sobre la pureba de hipótesis
##############################################################

#media diferencia de muestras poblacionales
medDiff = mean(Difference)
#asumiendo u0 = u1 (los algoritmos duran lo mismo en promedio)
muDiff = 0
#desviación estándard
Sd = sd(Difference)
#Tamaño de las muestra 
Nd = 40

#Cáculo del estadístico
t = (medDiff - muDiff) / (Sd * sqrt(1/Nd))

#Cálculo de el punto crítico para poner a prueba la hipótesis nula
alfa = 0.05
grados_libertad = Nd-1
qT = qt(0.975, grados_libertad)

if (abs(t) > qT) "Rechazamos la hipótesis nula" else "No rechazamos la hipótesis nula"


