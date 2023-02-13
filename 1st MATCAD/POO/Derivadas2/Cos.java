public class Cos implements Expression{

    Expression expresio; //com que no sabem que li passem al sinus posem la expresió (general). DECLARACIÓ DE LES VARIABLES ABANS!!!!!!!!!!!!!!!!!!!!!!
    public Cos(Expression expresio){
        this.expresio = expresio;
    }

    public DualNumber evaluate(DualNumber variable){ //DualNumber = tipus, variable = nom

        return new DualNumber(Math.cos(this.expresio.evaluate(variable).u), this.expresio.evaluate(variable).uprime*(-Math.sin(this.expresio.evaluate(variable).u))); //cuando e
    }
}
