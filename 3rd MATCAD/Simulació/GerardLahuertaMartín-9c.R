
# APARTAT a)
Box_Muller = function(){
  u = runif(2, min = 0, max = 1)
  
  c = sqrt(-2*log(u[1]))
  i = 2*u[2]*pi
  
  X = c*cos(i)
  Y = c*sin(i)
  
  return(list(X,Y))
}

################################################################################

# APARTAT b)
Marsaglia_Bray = function(){
  U = runif(1, min = 0, max = 1)
  
  V1 = 1
  V2 = 1
  
  while (V1*V1 + V2*V2 > 1) {
    u = runif(2, min = 0, max = 1)
    
    V1 = 2*u[1]-1
    V2 = 2*u[2]-1
  }

  s = sqrt(V1*V1 + V2*V2)
  c = sqrt(-2*log(U))
  
  X = c* V1 / s
  Y = c* V2 / s

  return(list(X,Y))
}

################################################################################

# APARTAT c)

# Iniciem els processos aleatoris amb la llavor 0
set.seed(0)

# Declarem les llistes on guardarém les 10000 parelles (20000 nombres aleatoris)
llista_BoxMuller = c()
llista_MarsagliaBray = c()
llista_builtin = c()

# Generació de les 10000 parelles amb els 3 metodes
for (i in 0:10000){
  a = Box_Muller()
  b = Marsaglia_Bray()
  c = rnorm(n=2)
  
  llista_BoxMuller = c(llista_BoxMuller, a[[1]])
  llista_BoxMuller = c(llista_BoxMuller, a[[2]])
  
  llista_MarsagliaBray = c(llista_MarsagliaBray, b[[1]])
  llista_MarsagliaBray = c(llista_MarsagliaBray, b[[2]])
  
  llista_builtin = c(llista_builtin, c[[1]])
  llista_builtin = c(llista_builtin, c[[2]])
}

# Guardem els resultats a un dataframe per a fer el boxplot
df = data.frame(
  "Box-Muller" = llista_BoxMuller,
  "Marsaglia-Bray" = llista_MarsagliaBray,
  "Built in" = llista_builtin
)

boxplot(df, col=rainbow(ncol(df))) 

# Comparació dels temps d'execució
library(microbenchmark)
test <- microbenchmark(
  BuiltIn = rnorm(n=2, mean = 0, sd = 1),
  BoxMuller = Box_Muller(),
  MarsagliaBray = Marsaglia_Bray(),
  times = 1000L
  )
print(test)

# ANALISI DELS RESULTATS
# 1. Els tres métodes generen valors segons una normal amb paràmetres (0,1) de 
#    forma correcta (tal i com s'observa al boxplot generat).
# 2. Els métodes tenen temps d'execució diferents sent el propi d'R el més ràpid,
#    seguit de métode de Box-Muller i finalment el que implementa la modificació 
#    de Marsaglia-Bray; sempre parlant en mitjes de temps. 
