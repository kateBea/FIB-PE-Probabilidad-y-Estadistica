############################################################
#
# Introducción a R - PiE CFIS
#
############################################################

############################################################
#
# R como calculadora
#
############################################################
2+3               # operaciones aritmeticas
3^2               # potencias
log(5)            # logaritmo natural
a <- 2*5          # asignaciones
a

############################################################
#
# Objetos por defecto
#
############################################################
pi                # valor de pi
letters           # letras minusculas
LETTERS           # letras mayusculas
month.name        # nombre de los meses


############################################################
#
# Tipos de objetos
#
############################################################
############################################################
# Vectores: serie ordenada de valores del mismo tipo --> c()
############################################################
##-- Ejemplos
v1 <- c(32,40,61,71,39)       # edades de personas (vector numerico)
v1
v2 <- 1:5                     # enteros del 1 al 5 (vector entero)
v2
v3 <- c("h","m","h","m","m")  # genero de personas (vector de caracteres)
v3
class(v1)
class(v2)
class(v3)

##-- Acceso y modificacion
v1[3]          # Ver 3r elemento
v1[3] <- 62    # Modificar 3r elemento
v1[3]          # Ver 3r elemento

v1[2:4]        # Ver los elementos del 2 al 4
v1[c(1,2,4)]   # Ver los elementos 1,2 y 4

############################################################
# Matrices: serie de columnas numericas ordenadas y de igual longitud --> matrix 
############################################################
##-- Ejemplos
m1 <- matrix(c(10,20,30,40),nrow=2,byrow=TRUE)
m1
m2 <- cbind(v1,v2)
m2

##-- Acceso y modificacion
m1[2,1]       # Elemento de la fila 2, columna 1       
m1[2,]        # Elementos de la fila 2
m1[,1]        # Elementos de la columna 1
m1[2,1] <- 35 # Modificar el elemento de la fila 2, columna 1
m1

colnames(m2) <- c("edad","orden")  # Modificar el nombre de las columnas 
rownames(m2) <- letters[1:5]       # Modificar el nombre de las filas
colnames(m2)                       # Consultar el nombre de las columnas 
rownames(m2)                       # Consultar el nombre de las filas
m2

############################################################
# Data.frames: serie de columnas de cualquier tipo ordenadas y de igual longitud --> data.frame
############################################################
##-- Ejemplos
df1 <- data.frame(x=letters[1:10],y=1:10)   # Letras del 1 al 10 y numeros del 1 al 10
df1
df2 <- data.frame(v3,v1)
df2

##-- Acceso y modificacion
df1$x          # Columna llamada "x"
df1[,"x"]      # Columna llamada "x" (alternativa a la anterior opcion)
df1[,1]        # Primera columna     (alternativa a la anterior opcion)

df1[2,1]       # Elemento de la fila 2, columna 1
df1$x[2]       # Elemento de la fila 2, columna 1 (alternativa a la anterior opcion)

df1[2,]        # Elementos de la fila 2
df1[2,2] <- 35 # Modificar el elemento de la fila 2, columna 2
df1

df1[3,1] <- "k" # Atencion: Modificar el elemento de la fila 3, columna 1
df1

df1[3,1] <- "j" # Modificar el elemento de la fila 3, columna 1. Ahora si, porque "j" es un nivel del factor.
df1

############################################################
# Listas: cualquier combinacion de los elementos anteriores --> list 
############################################################
##-- Ejemplos 
l1 <- list(v1,m1,df1)
l1

##-- Acceso y modificacion
l1[1]               # Un corchete para simplificar la lista
class(l1[1])

l1[[1]]             # Doble corchete para acceder al elemento
class(l1[[1]])


##-- Ejercicio 1.1:
# 1. Crear un data.frame con los siguientes datos
#     marca modelo consumo
#      audi     A3     6.8
# volswagen   Polo     6.8
#       bmw     X5    12.5
#   renault megane     4.7
#      fiat  punto     5.0
# 2. Modificar el consumo del fiat punto a 4.5



############################################################
#
# Instrucciones utiles
#
############################################################
##-- Ayuda
?mean                         # interrogante para pedir ayuda de una funcion
??geometric                   # doble interrogante para pedir ayuda general --> equivalente a help.search("geometric")

##-- Busqueda
which(df1$x=='e')
which.max(df1$y)
which.min(df1$y)

##-- Programacion
if(v1[1]==32) print("La edad del primer individuo es 32")  # if --> condicion logica
for(i in 1:5) print(v1[i])                                 # Bucle 


##-- Gestion de objetos
ls()              # Objetos en memoria
object.size(v1)   # Tamanyo que ocupa un objeto en memoria
rm(v1)            # Borrar (remove) algun objeto en memoria
rm(list=ls())     # Borrar todos los objetos en memoria


############################################################
#
# Simular datos
#
############################################################
n <- 1000                            # Numero de datos a simular

set.seed(12345)
sample_1 <- sample(1:(10*n),n)       # muestra de enteros sin reposicion
sample_2 <- sample(1:n,n,rep=TRUE)   # muestra de enteros con reposicion
sample_3 <- runif(n)                 # muestra de reales en (0,1)
sample_4 <- runif(n,1,5)             # muestra de reales en (1,5)
sample_5 <- rnorm(n)                 # muestra de una Normal(0,1)
sample_6 <- rnorm(n,1,2)             # muestra de una Normal(1,2)

par(mfrow=c(2,3))                    # Ventana para hacer 6 graficos (2 filas x 3 columnas)
hist(sample_1)
hist(sample_2)
hist(sample_3)
hist(sample_4)
hist(sample_5)
hist(sample_6)
