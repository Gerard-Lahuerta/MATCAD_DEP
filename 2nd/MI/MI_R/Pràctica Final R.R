################################################################################

# EXPLICACIONS NECESSARIES PER A ENTENDRE EL CODI:
#
# Per tal de fer mes facil l'execució del programa he decidit posar les linies
# que s'emcarreguen de fer una mateixa cosa dintre de "blocs" fets per claus {}.
#
# Si executes la linia on es troba la clau d'inici del bloc ('{') executarà
# totes les linies dintre del bloc.
#
# He separat cada exercici en 2 blocs: Obtenicó del resultat i 
# comprovació/conclusions dels resultats obtinguts.
#
# En tots els blocs està explicat mitjançant comentaris l'utilitat de les
# comandes utilitzades. També s'han col·locat les explicacions amb prints
# per a que es mostrin si s'executa tot el bloc de comandes els pasos que es 
# segueixen.
#
# Afegir que per a certs casos s'ha necesitat l'utilització de paquets externs 
# als que s'instal·len per defecte amb l'R-studio. Aquest paquets els he anat
# indicats (per a la seva instal·lació en cas de ser necesaria) a inici d'aquest
# codi (just després d'aquesta explicació).
# 
# Cada exercici esta separat per dos linies de '#' (la de finalització de
# l'exercici i la d'inici del següent).

################################################################################



################################################################################

#IMPORTS --> executar en cas de no tenir el paquet latex2exp

{# instal·lació del paquet latex2exp per als plots. 
  install.packages("latex2exp")
  library(latex2exp)
}

################################################################################



################################################################################

# EXERCICI 1:
# Apartat b)

{ # Executar aquesta linia per a obtenir el resultat de l'apartat b).
  
  # Declaració de la funció log-versemblança
  logLik.trPoiss=function(p, x){
    n = length(x)
    
    # Calculem els termes que conformen la log-versemblança
    t1 = log(p)*sum(x)
    t2 = n*log(exp(p) - 1)
    t3 = sum( log(factorial(x) ) )
   
    # res =  ceiling(t1 - t2 - t3) <--ceiling arrodona al primer enter superior
    
    res =  t1 - t2 - t3
     
    return( -res ) # Com que la funció té un màxim y en l'infinit (positiu i 
                   # negatiu) tendeix a -infinit --> cambiem el signe per a que
                   # e¡"es doni la volta" i "el màxim es torni un minim"
  }
  
  # Sel·lecionem per pantalla el fitxer amb les dades, concretament l'Snakes.txt
  print("----------------------------------------------------------------------")
  print("Seleccionem l'archiu Snakes.txt i el guardem a data:")
  data = read.delim(file.choose(),sep="\n")
  print("Mostrem part de les dades agafadem:")
  print(c(data$N_days[1:10],"..."))
  print("----------------------------------------------------------------------")
  
  # Calculem mitjançant la funció nlm el maxim de la funció log-versemblança
  print("Calculem amb la funció nlm el maxim de la funció log-versemblança:")
  resultat = nlm(f = logLik.trPoiss, p = c(mean(data$N_days)), 
                 data$N_days, hessian = TRUE)
  print("Mostrem ara el resultat obtingut per la funció:")
  lambda = resultat$estimate
  show(lambda)
  print("----------------------------------------------------------------------")
  
  # Caculem l'interval de confiança de Wald del 95% 
  # Com que tenim més de 30 mostres --> aproximem per la normal per el CTL.
  print("Calculem ara l'interval de confiança de Wald del 95% aprofitant que tenim més de 30 mostres (130 concretament) aproximant el resultat mitjançant l'interval de la normal pel Teorema Central del Límit")
  I = function(x,lambda){
    n = length(x)
    e = exp(lambda)
    return( n*( mean(x)/ ( lambda*lambda) - (e/( (e-1)*(e-1) ) ) ) )
  }
  Q = qnorm(0.975) # Ens dona el quantil 0.975
  S = 1 / sqrt( I(data$N_days, lambda) )
  inf = lambda - Q*S
  sup = lambda + Q*S
  Wald = c(inf,sup)
  print("Obtenim el límit superior i el límit inferior: de l'interval de confiança de Wald:")
  show(Wald)
  print("----------------------------------------------------------------------")
}  

{ # Executar aquesta linia per a obtenir les conclusions i 
  # Comprovacions dels resultats
  
  # Fem un gràfic per a observar si els calculs tenen sentit
  dom = seq(0,10,0.1)
  plot(dom,-logLik.trPoiss(dom,data$N_days), type = "l", 
            xlab = TeX("domini seleccionat (valors de $\\lambda$)"),
            ylab = TeX("Imatge de la funció"),
            main = TeX("Gràfica de la log-versemblança $l(x| \\lambda)= \\log( \\lambda)
                        \\sum_{i=1}^{n}x_i - n \\log(e^ {\\lambda} -1) - \\sum_{i=1}^{n}
                        \\log(x_i!)$"), col = "blue" )
  points(lambda, -logLik.trPoiss(lambda, data$N_days), pch=10, col = "green")
  text(lambda, -logLik.trPoiss(lambda, data$N_days), 
       label = TeX("Estimació $\\bar{\\lambda}$"), pos = 4, cex = 0.6, 
       col = "red")
  print("Observem per tant que el valor obtingut per al paràmetre lambda s'apropa molt a ser el màxim de la funció log-versemblança, el valor obtingut és : ") 
  print(lambda)
  print("----------------------------------------------------------------------")
  print("Per altra banda, l'interval de confiança trobat és que el paràmetre lamda estarà en el 95% dels casos entre:")
  print(Wald)
}


# Apartat c)
# Argumentació: Amb un nivell de significació del 5% observem que els valors del
# paràmetre lamda estàn entre 1.59 i 2.12 pel que afirmar l'hipotesis H0 seria 
# tenir un error de tipus 2 molt elevat (ja que en el 97,5% dels casos lambda 
# estarà en aquest interval), pel que el millor es acceptar l'hipotesis H1.

print("Argumentació: Amb un nivell de significació del 5% observem que els valors delparàmetre lamda estàn entre 1.89 i 2.51 pel que afirmar l'hipotesis H0 seria tenir un error de tipus 2 molt elevat(ja que en el 97,5% dels casos lambda estarà en aquest interval), pel que el millor es acceptar l'hipotesis H1.")

################################################################################



################################################################################

# EXERCICI 2
# Apartat a) i b)

{ # Executar aquesta linia per a compilar les funcions dels apartats a) i b)
  
  var.test = function(S, n, alpha){ # Comprovem amb un test si podem asumir
                                    # les sigmes iguals o no
    Wx = (S[1]*S[1])/(S[2]*S[2])
    Q = c(qf(alpha/2,n[1]-1,n[2]-1), qf(1-alpha/2,n[1]-1,n[2]-1))
    return(Wx < Q[1] | Wx > Q[2])
  }
  
  custom.ttest = function(pob1 = c(), pob2 = c(), test.type = "two-sided",
                          true.sigma = FALSE, sigma = NULL, alpha = 0.05, 
                          paired = FALSE){
    
    # Variables necesaries per a fer els calculs
    n = c( length(pob1),length(pob2))
    S = c( var(pob1), var(pob2) )
    
    # Determinem el mode estandar per a fer els calculs del p-value
    dist  = "T-Student"
    
    if(paired){ # Son dependents?
      D = 1/n[1] * sum(pob1-pob2)
      Sd = sd(pob1-pob2)
      llibertat = n[1] - 1
      Wx = ( D*sqrt(n[1]) )/Sd
    } else if(true.sigma){ # Coneixem les variances?
      dist = "Normal"
      Wx = (mean(pob1)-mean(pob2))/(sigma[1]/n[1]+sigma[2]/n[2])
    } else if( var.test(S, n, alpha) ){ # Tenen la mateixa variança?
      llibertat = sum(n) - 2
      Sp = sqrt( (n[1]-1)*S[1]+ (n[2]-1)*S[2] )/ llibertat
      Wx = (mean(pob1)-mean(pob2))/(Sp*sqrt(1/n[1]+1/n[2]))
    } else{ # Cas de descart --> assumim n i m suficientment grans --> 
            # --> arrodonim per la normal amb el Criteri de Pas al Límit
      Wx=(mean(pob1)-mean(pob2))/sqrt(S[1]/n[1] + S[2]/n[2])
      if(n[1] > 30 & n[2] > 30){
        dist = "Normal"
      }
      else{
        num = (S[1]/n[1] + S[2]/n[2])*(S[1]/n[1] + S[2]/n[2])
        den = c( (S[1]/n[1])*(S[1]/n[1]), (S[2]/n[2])*(S[2]/n[2]) ) 
        den[1] = den[1]/(n[1]-1)
        den[2] = den[2]/(n[2]-1)
        den = sum(den)
        llibertat = num/den
      }
    }
    
    t.stat = Wx # <-- Guardem el valor obtingut a t.stat
    
    # Calculem el p-avlue segons la distribució del test estadistic
    if(dist == "Normal"){
      switch (test.type,
        "two-sided" = {
          p.value <- pnorm(-abs(Wx)) + 1 - pnorm(abs(Wx))
        },
        "greater" = {
          p.value <- 1-pnorm(Wx)
        },
        "less" = {
          p.value <- pnorm(Wx)
        },
      )
    }
    else{
      switch (test.type,
        "two-sided" = {
          p.value = pt(-abs(Wx), llibertat) + 1 -pt(abs(Wx), llibertat)
        },
        "greater" = {
          p.value = 1-pt(Wx, llibertat)
        },
        "less" = {
          p.value = pt(Wx, llibertat)
        },
      )
    }
    
    cat( "--------------------\n",
         "test statistic = ", t.stat, "\n",
         "p-value = ", p.value, "\n",
         "--------------------\n")
    
    return( c(t.stat, p.value) )
  }
}


{ # Executar aquesta linia per a obtenir les conclusions i 
  # Comprovacions dels resultats
  
  # Cas dels pacients:
  abans = c(115, 112, 107, 119, 115, 138, 126, 105, 104, 115)
  despres = c(128, 115, 106, 128, 122, 145, 132, 109, 102, 117)
  
  # Calculem els resultats obtinguts per la nostra funció i la comparem amb els
  # que obté la funció t.test
  print("Resultats obtinguts per la nostra funció en el test dels pacients:")
  resultats = custom.ttest(abans, despres, paired = TRUE)
  comprovacio = t.test(abans, despres, alternative = "two.sided", paired = TRUE)
  print("Resultats Obtinguts per la funcio t.test")
  cat( "test statistic --> ", comprovacio$statistic, "\n")
  cat( "p-value --> ", comprovacio$p.value, "\n")
  print("--------------------")
  print("Diferencia entre els valors obtinguts: ")
  cat("Diferencia test statistic: ", comprovacio$statistic-resultats[1], "\n")
  cat("Diferencia p-value: ", comprovacio$p.value-resultats[2],"\n\n\n")
  
  # Cas de les maquines
  maq1 = c(137.5, 140.7, 106.9, 175.1, 177.3, 120.4, 77.9, 104.2)
  maq2 = c(103.3, 121.7, 98.4, 161.5, 167.8, 67.3)
  
  # Calculem els resultats obtinguts per la nostra funció i la comparem amb els
  # que obté la funció t.test
  print("Resultats obtinguts per la nostra funció en el test de les màquines:")
  resultats = custom.ttest(maq1, maq2)
  comprovacio = t.test(maq1, maq2, alternative = "two.sided")
  print("Resultats Obtinguts per la funcio t.test")
  cat( "test statistic --> ", comprovacio$statistic, "\n")
  cat( "p-value --> ", comprovacio$p.value, "\n")
  print("--------------------")
  print("Diferencia entre els valors obtinguts: ")
  cat("Diferencia test statistic: ", comprovacio$statistic-resultats[1], "\n")
  cat("Diferencia p-value: ", comprovacio$p.value-resultats[2],"\n")
  
  # Conclusions: 
  print("--------------------")
  print("Només en el primer cas (pacients del tractament) podem descartar l'hipotesis nul·la ja que el valor que obtenim del p-value es inferior al de alpha (0.05); que no es el cas en la segona mostra (rendiment de les màquines)")
  # Només en el primer cas (pacients del tractament) podem descartar 
  # l'hipotesis nul·la ja que el valor que obtenim del p-value es inferior al 
  # de alpha (0.05); que no es el cas en la segona mostra (rendiment de les 
  # màquines)
}
  
################################################################################



################################################################################

# EXERCICI 3
# Apartat a)

{ # Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats a)
  
  # Creem els vector amb l'informació i els agrupem en la variable data
  temperatura = c(24.9, 35, 44.9, 55.1, 65.2, 75.2, 85.2, 95.2)
  viscositat = c(1.1330, 0.9772, 0.8532, 0.755, 0.6723, 0.6021, 0.542, 0.5074)
  data = data.frame(temperatura, viscositat)
  llista = cbind(data$temperatura, data$viscositat)
  
  # mostrem el la gràfica de punts que genera les dades obtingudes
  plot(llista, xlab="Temperatura",
       ylab="Viscositat", main="Relació Temperatura-Viscositat")
  
  # Observem que existeix una certa relació lineal entre la temperatura i la 
  # viscositat que té la substancia quimica estudiada ja que a mida que 
  # s'augmenta la temperatura la viscositat del fluid decreix de manera 
  # casi "proporcional"
  cat("Observem que existeix una certa relació lineal entre la temperatura i",
      "la viscositat que té la substancia quimica estudiada ja que a mida que", 
      "s\'augmenta la temperatura la viscositat del fluid decreix de manera",
      "casi \"proporcional\".")
}


# Apartat b)

{# Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats b) --> ES NECESSARI HAVER EXECUTAT L'APARTAT ANTERIOR
  
  # Apliquem la funció "lm" per a obtenir el model de regressió lineal que 
  # millor refelxa els resultats del nostre experiment i guardem en les 
  # variables beta0 i beta1 els respectius valor que ens torna la funció
  cat("Calculem els valor de beta_0 i beta_1, coeficients de la recta de",
  "regressió expresada com y=beta_0+beta_1 * x (amb y la viscositat i x la",
  "temperatura), mitjançant la funció lm. Mostrem per pantalla els valors",
  "obtinguts i la gràfica amb la recta de regressió.")
  model = lm(viscositat ~ temperatura, data = data)
  beta0 = model$coefficients[1]
  beta1 = model$coefficients[2]
  print("------------------------------------------")
  cat(" Valor de beta_0 -->", beta0,"\n",
      "Valor de beta_1 -->", beta1,"\n")
  print("------------------------------------------")
  # Observació, el coeficient de beta_1 és negatiu perque a mida que augmenta
  # el valor de la temperatura la viscositat decreix
  print("Observació: El coeficient de beta_1 és negatiu perque a mida que augmenta el valor de la temperatura la viscositat decreix")
  
  # Mostrem per pantalla la recta de regressió lineal obtinguda sabent que serà
  # y = beta0 + beta1*x on y és la visctositat i x la temperatura
  lines(c(0,100), c(beta0,beta0+beta1*100), lwd = 3, col = "red")
  
}


# Apartat c)

{ # Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats c) --> ES NECESSARI HAVER EXECUTAT L'APARTAT ANTERIOR
  
  # Calculem l'inerval de confiança de Wald per al coeficient beta_1
  cat("El valor que obtenim per a l'error segons el nostre model lineal és: ")
  error = summary(model)$coefficients[4]
  n = length(viscositat)
  cat(error, "\n")
  Q = qt(0.975, n-1) * error
  interval = c(beta1-Q, beta1+Q)
  cat("L'interval de confiança tal que en el 95% de les mostres, el coeficient",
      "beta_1 estigui en l'interval és (", interval, ") \n")
  cat("Observem que d'intre de l'interval de confiança es troba el valor de",
      "l'hipotesis nul·la, pel que no podem asegurar que l'hipotesis nu·la no",
      "sigui certa \n")
  
}


# Apartat d)
  
{ # Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats c) --> ES NECESSARI HAVER EXECUTAT L'APARTAT a)
  
  # Calculem el valor del coeficient de determinació del model
  in_sum = viscositat-mean(viscositat)
  SST = sum(in_sum*in_sum)
  in_sum = viscositat-beta1*temperatura-beta0
  SSres = sum(in_sum*in_sum)
  R2=1-SSres/SST
  cat("El valor de coeficient de determinació del model obtingut és", R2, "\n",
      "Observem doncs que el valor és molt proper a 1 pel que el model lineal",
      "simple explica molt bé. Recalcar que aquest coeficient en models",
      "de regressió lineal simple coincideix amb el coeficient de Pearson \n")
  
}


# Apartat e)

{ # Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats e) --> ES NECESSARI HAVER EXECUTAT L'APARTAT a)
  
  # A partir del valor obtingut a l'apartat b), calculem sigma^2
  sigma = summary(model)$sigma
  sigma2 =sigma*sigma
  cat("El valor de sigma quadrat obtingut pel nostre model lineal és", sigma2,
      "\n")
  sigma2 = SSres/(n-2)
  cat("El valor de sigma quadrat obtingut pel nostre calcul sobre el ",
      "model lineal és", sigma2, "\n")
}


# Apartat f)

{ # Executar aquesta linia per a obtenir els resultats i conclusions
  # de l'apartats f) --> ES NECESSARI HAVER EXECUTAT L'APARTAT a)
  
  # Calculem el valor de la viscositat per a una temperatura de 90º segons el 
  # nostre model
  visc = predict(model, newdata = data.frame( temperatura = 90))
  cat("El valor (segons el nostre model) de la viscositat per a una",
      "temperatura de 90º és de :", visc, "\n")
  
}
