\* Problem: exercici2_c1 *\

Minimize
 cost: + 130 x(1,A) + 90 x(1,B) + 148 x(2,A) + 156 x(2,B) + 15 y(A,N)
 + 27 y(B,N) + 24 y(A,S) + 18 y(B,S)

Subject To
 res_env_ciu(1): + x(1,A) + x(1,B) = 500
 res_env_ciu(2): + x(2,A) + x(2,B) = 400
 res_reb_inc(A): + x(1,A) + x(2,A) <= 500
 res_reb_inc(B): + x(1,B) + x(2,B) <= 600
 res_env_inc(A): - 0.2 x(1,A) - 0.2 x(2,A) + y(A,N) + y(A,S) = -0
 res_env_inc(B): - 0.2 x(1,B) - 0.2 x(2,B) + y(B,N) + y(B,S) = -0

Bounds
 0 <= x(1,A) <= 300
 0 <= x(1,B) <= 300
 0 <= x(2,A) <= 300
 0 <= x(2,B) <= 300

End
