
f = function(x){
  return( -x * log( x / 2 ) )
}

g = function(x){
  return ( 1 / 2)
}

x = c()
C = f(2*exp(-1))

for (i in seq(0,999)) {
  u = runif(2, 0, 2)
  
  while ( u[1] > f(u[2]) / ( C*g(u[2]) ) ) {
    u = runif(2, 0, 2)
  }
  
  x = c(x,u[2])
    
}

print(mean(x))
print(sd(x))

hist(x)