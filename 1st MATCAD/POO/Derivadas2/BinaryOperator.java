abstract class BinaryOperator implements Expression{
    Expression left;
    Expression right;
    public BinaryOperator (Expression expr1, Expression expr2){
        left = expr1;
        right = expr2;
    }
}