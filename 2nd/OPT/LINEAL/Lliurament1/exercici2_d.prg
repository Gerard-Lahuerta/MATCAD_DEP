\* Problem: exercici2_d *\

Minimize
 cost: + 40 x(1,A) + 120 x(1,B) + 130 x(2,A) + 30 x(2,B) + 105 y(A,N)
 + 153 y(B,N) + 114 y(A,S) + 141 y(B,S)

Subject To
 res_env_ciu(1): + x(1,A) + x(1,B) = 500
 res_env_ciu(2): + x(2,A) + x(2,B) = 400
 res_reb_inc(A): + x(1,A) + x(2,A) <= 500
 res_reb_inc(B): + x(1,B) + x(2,B) <= 600
 res_env_inc(A): - 0.2 x(1,A) - 0.2 x(2,A) + y(A,N) + y(A,S) = -0
 res_env_inc(B): - 0.2 x(1,B) - 0.2 x(2,B) + y(B,N) + y(B,S) = -0

End
