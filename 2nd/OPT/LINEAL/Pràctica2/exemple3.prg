\* Problem: exemple3 *\

Minimize
 cost_total: + 30 x(A,1) + 70 x(A,2) + 10 x(A,3) + 20 x(B,1) + 20 x(B,2)
 + 60 x(B,3)

Subject To
 r_capacitat(A): + x(A,1) + x(A,2) + x(A,3) <= 800
 r_capacitat(B): + x(B,1) + x(B,2) + x(B,3) <= 1500
 r_demanda(1): + x(A,1) + x(B,1) >= 1000
 r_demanda(2): + x(A,2) + x(B,2) >= 700
 r_demanda(3): + x(A,3) + x(B,3) >= 600

End
