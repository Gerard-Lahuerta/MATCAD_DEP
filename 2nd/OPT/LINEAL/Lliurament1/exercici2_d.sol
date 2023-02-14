Problem:    exercici2_d
Rows:       7
Columns:    8
Non-zeros:  24
Status:     OPTIMAL
Objective:  cost = 53780 (MINimum)

   No.   Row name   St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 cost         B          53780                             
     2 res_env_ciu[1]
                    NS           500           500             =         148.2 
     3 res_env_ciu[2]
                    NS           400           400             =          58.2 
     4 res_reb_inc[A]
                    NU           500                         500         -87.2 
     5 res_reb_inc[B]
                    B            400                         600 
     6 res_env_inc[A]
                    NS             0            -0             =           105 
     7 res_env_inc[B]
                    NS             0            -0             =           141 

   No. Column name  St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 x[1,A]       B            500             0               
     2 x[1,B]       B              0             0               
     3 x[2,A]       NL             0             0                         180 
     4 x[2,B]       B            400             0               
     5 y[A,N]       B            100             0               
     6 y[B,N]       NL             0             0                          12 
     7 y[A,S]       NL             0             0                           9 
     8 y[B,S]       B             80             0               

Karush-Kuhn-Tucker optimality conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.DE: max.abs.err = 1.42e-14 on column 3
        max.rel.err = 6.54e-17 on column 3
        High quality

KKT.DB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
