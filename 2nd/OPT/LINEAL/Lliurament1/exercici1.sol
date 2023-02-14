Problem:    exercici1
Rows:       6
Columns:    2 (2 integer, 0 binary)
Non-zeros:  12
Status:     INTEGER OPTIMAL
Objective:  benefici = 56000 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 benefici                56000                             
     2 pressupost              56000                       56000 
     3 pressupost_llaunes
                               22400                      140000 
     4 peix_llaunes            78400                      196000 
     5 temps_enllaunament
                                 140                         600 
     6 temps_test                350                         640 

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x[A]         *              0             0               
     2 x[S]         *         280000             0               

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
