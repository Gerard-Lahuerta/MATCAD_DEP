Problem:    exercici2
Rows:       7
Columns:    8
Non-zeros:  24
Status:     OPTIMAL
Objective:  cost = 107200 (MINimum)

   No.   Row name   St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 cost         B         107200                             
     2 res_env_ciu[1]
                    NS           500           500             =          93.6 
     3 res_env_ciu[2]
                    NS           400           400             =           151 
     4 res_reb_inc[A]
                    B            400                         500 
     5 res_reb_inc[B]
                    B            500                         600 
     6 res_env_inc[A]
                    NS             0            -0             =            15 
     7 res_env_inc[B]
                    NS             0            -0             =            18 

   No. Column name  St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 x[1,A]       NL             0             0                        39.4 
     2 x[1,B]       B            500             0               
     3 x[2,A]       B            400             0               
     4 x[2,B]       NL             0             0                         8.6 
     5 y[A,N]       B             80             0               
     6 y[B,N]       NL             0             0                           9 
     7 y[A,S]       NL             0             0                           9 
     8 y[B,S]       B            100             0               

Karush-Kuhn-Tucker optimality conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.DE: max.abs.err = 1.42e-14 on column 2
        max.rel.err = 7.55e-17 on column 2
        High quality

KKT.DB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
