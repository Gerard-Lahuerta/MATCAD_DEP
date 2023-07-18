
FInversa = function(x, mu = 1.5, s = 0.25, truncar) {
  if (x <= truncar){
    return( 0 )
  }
  return( mu - s * log( 1/x - 1 ) )
}

FLogistica = function(x, mu = 1.5, s = 0.25){
  return( 1 / ( 1 + exp( (mu - x)/s ) ) )
}

f1 = FLogistica(1)

x = c()
y = 0
for ( i in seq(0,99) ) {
  while ( y == 0 ) {
    u = runif(1, 0, 1)
    y = FInversa(u, truncar = f1)
  }
  x = c(x, y)
  y = 0
}

print(mean(x))
print(sd(x))

hist(x)
