f <- 1.55 #tasa de fallos
t <- 100 #por cuantas palabras (palabras en total


#1. Vamos a poner a prueba Noodle con un texto que consta de 440 palabras.
#Calcule la probabilidad de que el número de fallos que se produzca sea menor que 14.
lambda <- (f/t)*410
ppois(7, lambda) #restamos 1 al valor pedido


#2. 2. Calcule la probabilidad de que en este texto se produzcan 16 fallos
#de traducción exactamente.
dpois(16, lambda)
#3. ¿Cuánto valdría la variancia del número de palabras que hay entre
#dos errores de Noodle?
#en este caso calculamos la variancia
lambda_exp <- f/t
var <- 1/(lambda_exp)^2
var
#4. Para un texto que se ha de tratar, ¿cuál es la probabilidad de
traducircorrectamente al menos 44 palabras seguidas, antes de producir un
nuevo fallo?
1-pexp(44, lambda_exp)
#5 Para ver si lo hemos comprendido bien: introduzca un intervalo de dos
números para el que podamos decir que hay una probabilidad P (0.25 < P <
0.39) de que el traductor consiga un número de palabras consecutivas
correctamente traducidas que se sitúe en tal intervalo. Los dos números en
orden natural, y separados por un blanco.
qexp(0.25, lambda_exp) #nos da un valor aproximado
qexp(0.39, lambda_exp)
pexp(15, lambda_exp) - pexp(5, lambda_exp) #probamos valores hasta que de
un valor dentro del intervalo
#6. Noodle va a ser utilizado para traducir un conjunto de 20 textos de
longitud y características similares, de los que se sabe que hay una
probabilidad 0.1 de no observar ningún error de traducción. Halle la
probabilidad de producir más de 19 textos sin errores.
1-pbinom(19, 20, 0.1)
#7. ¿Cuál de estas figuras corresponde a la variable aleatoria que
representa cuántos documentos no presentarán errores de traducción?
Responde con el número (1, 2, 3, 4) que hay sobre la figura.
mfrow=t(1)
plot(0:20, dbinom(0:20, 20, 0.1), type = "h")
Reservados todos los derechos. No se permite la explotación económica ni la transformación de esta obra. Queda permitida la impresión en su totalidad.
undefined-10084303