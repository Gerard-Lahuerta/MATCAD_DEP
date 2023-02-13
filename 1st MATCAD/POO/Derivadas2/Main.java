public class Main {
    private static double f(double x) {
        //Funció de la extension
        return (x*x)*(Math.sin(x)*Math.sin(x) + Math.cos(x)); //funció
    }
    private static double df(double x) {
        return 2*x*(Math.sin(x)*Math.sin(x)+Math.cos(x)) + (Math.sin(2*x)-Math.sin(x))*x*x; //expresió de la derivada
    }
    public static void main(String[] args) {
        Expression x = new X();
        Expression expr = new Multiply( new Multiply(x,x), new Add(new Multiply (new Sin(x) , new Sin(x)), (new Cos(x)))); //funció que ha de derivar
        double x0 = 2.0; //x inicial
        DualNumber res = expr.evaluate(new DualNumber(x0, 1.0));
        double valorReal = f(x0);
        double derivadaReal = df(x0);


        System.out.println("real");
        System.out.println("f(" + x0 +") = "+valorReal);
        System.out.println("f'(" + x0 + ") = "+derivadaReal);
        System.out.println("calculat");
        System.out.println("f(" + x0 + ") = "+res.u); //valor funció calculat pel codi nostre
        System.out.println("f'(" + x0 + ") = "+res.uprime); //valor derivada calculada pel codi nostre
        System.out.println("diferencia valor " + (valorReal - res.u)); //error entre el valor real i lo que ens dona
        System.out.println("diferencia derivada " + (derivadaReal - res.uprime));
        System.out.println("derivada per diferencies finites");
        final double[] epsilons = {1e-6,1e-8,1e-10,1e-12};
        for(int i=0; i < epsilons.length; i++){
            double derivadaDifFinities = (f(x0+epsilons[i])-f(x0))/epsilons[i];
            System.out.println("per epsilon" + epsilons[i] + "," + derivadaDifFinities + "," + "diferencia" + (derivadaReal - derivadaDifFinities));
        }


        //FER UNA LLISTA AMB PUNTS QUE ANEM OMBLINT AMB ELS VALORS DE X (GRADIENT CREIXENT I DECREIXENT)
        //A LA LLISTA POSAR EL X0 i la resta buida
        //El problema és: f'(??????)


        /*double E = 0.001;
        for(int k=1; k <100; k++){
            xc[k]=x[k-1]+df()       CREIXENT
            xd[k]=x[k-1]-df()       DECREIXENT
        }*/

        //trobar max i min de la llista.
    }
}
