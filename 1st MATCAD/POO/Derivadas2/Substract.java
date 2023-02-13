public class Substract extends BinaryOperator{

    public Substract(Expression expr1, Expression expr2) {
        super(expr1, expr2); //expr1 left -- expr2 right --> super = anem a la classe mare
    }
    public DualNumber evaluate(DualNumber dn) { //f(x) + g(x) = h(x)
        DualNumber dn1 = left.evaluate(dn); //f(x) = h(x).left (es la part de la esquerra de la expresió)
        DualNumber dn2 = right.evaluate(dn);//g(x) = h(x).right (es la part de la dreta de la expresió)
        return new DualNumber(dn1.u-dn2.u, dn1.uprime - dn2.uprime); //f'(x) + g'(x)
    }
}
