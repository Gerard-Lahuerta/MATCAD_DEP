
### FUNCTIONS (exercici 18-19)

generateArrival = function(){
  nextArrival = clock + rexp(rate = 60/45, n = 1)
  if (nextArrival <= finalTime){
    newEvent = data.frame(time = nextArrival, event = "arribada")
    FEL <<- rbind(FEL, newEvent)
    FEL <<- FEL[order(FEL$time),]
  }
}

beginService = function(tService){ # Passem tService com a paràmetre per a l'apartat d)
  occupationOfService <<- 1
  nextCompletion = clock + rexp(rate = 60/tService, n = 1)
  newEvent = data.frame(time = nextCompletion, event = "sortida")
  FEL <<- rbind(FEL, newEvent)
  FEL <<- FEL[order(FEL$time),]
}

arrival = function(tService){ # Passem tService com a paràmetre per a l'apartat d)
  if(occupationOfService == 0){
    beginService(tService)
  }
  else{ # occupationOfService > 0
    customersInQueue <<- customersInQueue + 1
  }
  generateArrival()
  maxQueue <<- max(maxQueue, customersInQueue) # Apartat a)
}

exit = function(tService){ # Passem tService com a paràmetre per a l'apartat d)
  if(customersInQueue > 0){
    customersInQueue <<- customersInQueue - 1
    beginService(tService)
  }
  else{ # occupationOfService == 0
    occupationOfService <<- 0
  }
}

### MAIN

# Creem una funció encarregada de simular la cua (exercici 18-19)
# Passem tService com a paràmetre per a l'apartat d)
# Posem una variable show_results que determina si mostra els resultats de la simulació
simulacio = function(tService = 36, show_results = FALSE){ 
  generateArrival()
  while ( length(FEL$time) > 0 ) {
    if (show_results ) {
      print("----------------")
      cat("Queue: ", customersInQueue, "\n")
      print(FEL)
    }
    clock <<- FEL[1,1]
    
    if( FEL[1,2] == "arribada"){
      FEL <<- FEL[-1,]
      arrival(tService) # Passem tService com a paràmetre per a l'apartat d)
    }
    else{ # FEL[1,2] == "sortida"
      FEL <<- FEL[-1,]
      exit(tService) # Passem tService com a paràmetre per a l'apartat d)
    }
  }
  
  if (show_results){
    print("----------------")
    cat("Queue: ", customersInQueue, "\n")
  } 
}

set.seed(0)

# Apartat d)
for (tService in seq(36,20)) { # Bucle que variarà el valor del temps de servei (tService)

  maxQueueList = c() # Cua on guardem els resultats de les simulacions
  
  # Fem 100 simulacions 
  for (i in seq(1:100)) { # Apartat b)
    
    customersInQueue = 0
    occupationOfService = 0
    clock = 0
    finalTime = 60 
    
    time = numeric()
    event = character()
    FEL = data.frame(time, event)
    
    # Creem una variable on guardarém el valor de la cua maxim assolit durant la simulació
    maxQueue = 0 # Apartat a)
    
    simulacio(tService) # Passem tService com a paràmetre per a l'apartat d)
    
    # Guardem el valor registrar a la llista per a fer l'interval de confiança
    maxQueueList = c(maxQueueList, maxQueue) 
    
  }
  
  # Apartat c)
  # Calculem la mitjana i variança de la simulació i calculem l'interval de confiança del 95%
  meanMaxQueue = mean(maxQueueList)
  varMaxQueue = sqrt(var(maxQueueList)/length(maxQueueList))
  z = qnorm(0.95)
  inf = meanMaxQueue - z*varMaxQueue
  sup = meanMaxQueue + z*varMaxQueue
  cat("Interval de confiança de maxQueue amb tService =",tService, ": [",inf,",",sup,"]","\n")
}

### OBSERVACIÓ DELS RESULTATS
# Podem observar que a mida que reduïm el temps de servei (tService) l'interval de 
# confiança és va reduïnt; aixó es degut a que, a mida que és redueix el temps de servei,
# hi ha menys gent a la vegada a la cua al ser atesa més rapidament, fent que la variança 
# de la variable maxQueue decreixi.
#
# Aquest efecte seria el contrari si el tServei incrementes; ja que donaria més 
# temps a que arribessin persones a la cua i la llargaria màxima d'aquesta incrementes.
#
# Comentar, també, que hi han petites oscil·lacions en resultats propers, com per exemple
# amb tService = 32 i tService = 31; on els intervals augmenten lleugerament. 
# Aixó és degut als fenomens aleatoris pero la tendencia general és a decreixer. 


