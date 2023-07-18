
FInversa = function(x, mu = 1.5, s = 0.25) {
  return( mu - s * log( 1/x - 1 ) )
}

u = runif(1000,0,1)
x = FInversa(u)

print(mean(x))
print(sd(x))

hist(x)
