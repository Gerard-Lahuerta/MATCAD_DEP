
public class Constant implements Expression {
    protected double value;
    public Constant(double variable){
        this.value = variable;
    }
    public DualNumber evaluate(DualNumber dn){
        return new DualNumber (value, 0);
    }
}