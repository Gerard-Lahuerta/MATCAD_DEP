public class Sin implements Expression{
    Expression expresio; //com que no sabem que li passem al sinus posem la expresió (general). DECLARACIÓ DE LES VARIABLES ABANS!!!!!!!!!!!!!!!!!!!!!!
    public Sin(Expression expresio){
        this.expresio = expresio;
    }

    public DualNumber evaluate(DualNumber variable){ //DualNumber = tipus, variable = nom
        return new DualNumber(Math.sin(this.expresio.evaluate(variable).u), this.expresio.evaluate(variable).uprime * Math.cos(this.expresio.evaluate(variable).u)); //calcula el sinus de lo que hi ha dins (expresio defineix a quin evaluate va), derivada
    }
}