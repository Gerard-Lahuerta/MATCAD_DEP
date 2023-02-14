Problem:    exercici2_c3
Rows:       11
Columns:    8
Non-zeros:  28
Status:     OPTIMAL
Objective:  cost = 108060 (MINimum)

   No.   Row name   St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 cost         B         108060                             
     2 res_env_ciu[1]
                    NS           500           500             =          93.6 
     3 res_env_ciu[2]
                    NS           400           400             =         159.6 
     4 res_reb_inc[A]
                    B            300                         500 
     5 res_reb_inc[B]
                    B            600                         600 
     6 res_env_inc[A]
                    NS             0            -0             =            15 
     7 res_env_inc[B]
                    NS             0            -0             =            18 
     8 max_viat[1,A]
                    B              0                         300 
     9 max_viat[1,B]
                    B            500                         900 
    10 max_viat[2,A]
                    NU           300                         300          -8.6 
    11 max_viat[2,B]
                    B            100                         300 

   No. Column name  St   Activity     Lower bound   Upper bound    Marginal
------ ------------ -- ------------- ------------- ------------- -------------
     1 x[1,A]       NL             0             0                        39.4 
     2 x[1,B]       B            500             0               
     3 x[2,A]       B            300             0               
     4 x[2,B]       B            100             0               
     5 y[A,N]       B             60             0               
     6 y[B,N]       NL             0             0                           9 
     7 y[A,S]       NL             0             0                           9 
     8 y[B,S]       B            120             0               

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
