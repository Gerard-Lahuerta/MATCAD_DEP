\* Problem: exemple3_vis *\

Minimize
 cost_total: + 0.8 x(a1) + 0.6 x(a2) + 0.2 x(a3)

Subject To
 r1: + 3600 x(a1) + 2000 x(a2) + 1600 x(a3) <= 18000
 r1_1: + 3600 x(a1) + 2000 x(a2) + 1600 x(a3) >= 16000
 r2: + 0.25 x(a1) + 0.35 x(a2) + 0.15 x(a3) >= 2
 r3: + 0.7 x(a1) + 0.4 x(a2) + 0.25 x(a3) >= 3

End
