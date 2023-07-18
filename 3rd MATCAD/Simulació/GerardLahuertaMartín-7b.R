
# FUNCIÓ ENCARREGADA DE GENERAR ELS NOMBRES ALEATORIS SEGONS LA FUNCIÓ DE 
# DENSITAT f(x) = 0.5 + x
llei_ex_7b = function(){
  # \int (1/2 + x) = u
  # x/2 +x^2/2 = u 
  # x + x^2 = 2u
  # 1/4 + x + x^2 = 2u + 1/4
  # (x + 1/2)^2 = 2u + 1/4
  # x + 1/2 = sqrt(2u + 1/4)
  # x = sqrt(2u + 1/4) - 1/2
  
  u = runif(1, min = 0, max = 1)
  x = sqrt(2*u + 0.25) - 0.5
  return(x)
}

################################################################################

# FUNCIÓ QUE GENERARÀ ELS HISTOGRAMES AMB LA SEVA FUNCIÓ DE DENSITAT
hist_ex_7b = function (x, P = TRUE, title = "", Y = "Densitat") {
  hist(x, prob = P, main = title, ylab = Y) # Histograma
  
  lines(density(x), col = "red", lwd = 2) # Densitat

  legend("topleft", c("Histograma", "Densitat"), box.lty = 0,
         lty = 1, col = c("black", "red"), lwd = c(1, 2))
}

################################################################################

# MAIN DEL PROGRAMA: GENERA MOSTRES DE LA LLEI, CALCULA LA MITJANA I FA UN HISTORGRAMA 

set.seed(0) # Iniciem els processos aleatoris amb la "seed" 0 

mt = 1/4+1/3 # esperança teórica: E(F(x)) = \int_0^1 x / 2 + x^2 dx = 
             #                            = [x^2 / 4 + x^3 / 3]_0^1 = 1/4 + 1/3

for(max_i in c(100,1000,10000)){ # generem 100, 1000 i 1000 mostres i comparem els resultats
  m = 0
  llista = c()
  
  for( i in seq(0,max_i)){ 
    l = llei_ex_7b()
    m = m + l/max_i
    llista = c(llista, l)
  }
  
  cat("Iteracion: ", max_i, " --> mitjana obtinguda: ", m, "; mitjana teorica: ", mt,"\n")
  hist_ex_7b(llista, title = sprintf("historgrama amb %s iteracions", max_i))
  
  Sys.sleep(1) # esperem 1 segon per a mostrar els resultats 
}

# ANALISI DELS RESULTATS OBTINGUTS:
# 1. S'observa com les mitjanes (a mida que generem més mostres) s'aproximen 
#    millor a la esperança teorica 
# 2. S'observa com (a mida que generem més mostres) els historgrames semblen ser 
#    més precissos amb la funció de densitat donada (la recta 0.5 + x)

