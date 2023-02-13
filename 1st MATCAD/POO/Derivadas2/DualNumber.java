public class DualNumber{
    double u;
    double uprime;
    public DualNumber(double u, double uprime){ //mètode constructor + mètode return
        this.u = u;
        this.uprime = uprime;
    }
    public String toString(){
        String str_u = String.valueOf(this.u); //també podem fer String str_u = (type).toString(u)
        String str_uprime = String.valueOf(this.uprime);
        return str_u + " " + str_uprime;
    }
}
