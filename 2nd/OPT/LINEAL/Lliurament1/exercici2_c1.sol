Problem:    exercici2_c1
Rows:       7
Columns:    8
Non-zeros:  24
Status:     OPTIMAL
Objective:  cost = 115940 (MINimum)

   No.   Row name   St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 cost         B         115940                             
     2 res_env_ciu[1]
                    NS           500           500             =         141.6 
     3 res_env_ciu[2]
                    NS           400           400             =         159.6 
     4 res_reb_inc[A]
                    NU           500                         500          -8.6 
     5 res_reb_inc[B]
                    B            400                         600 
     6 res_env_inc[A]
                    NS             0            -0             =            15 
     7 res_env_inc[B]
                    NS             0            -0             =            18 

   No. Column name  St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 x[1,A]       B            200             0           300 
     2 x[1,B]       NU           300             0           300           -48 
     3 x[2,A]       B            300             0           300 
     4 x[2,B]       B            100             0           300 
     5 y[A,N]       B            100             0               
     6 y[B,N]       NL             0             0                           9 
     7 y[A,S]       NL             0             0                           9 
     8 y[B,S]       B             80             0               

Karush-Kuhn-Tucker optimality conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.DE: max.abs.err = 0.00e+00 on column 0
        max.rel.err = 0.00e+00 on column 0
        High quality

KKT.DB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
