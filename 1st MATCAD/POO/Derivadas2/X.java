public class X implements Expression {
    public X() {
        // void
    }
    public double evaluate(double val) {
        return val;
    }
    public DualNumber evaluate(DualNumber dn) {
        return new DualNumber(dn.u, 1.0);
    }
}
