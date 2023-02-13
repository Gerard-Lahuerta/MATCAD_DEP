public class Main{
  public static void main(String[] args) {
      And and1 = new And("and1"); //des d'aqui
      And and2 = new And("and2"); //Se crean objetos And
      Or or1 = new Or("or");
      Not not1 = new Not("not"); //fins aquí, és per crear el XOR
      Component xor = new Component("xor");//Per crear una nova component
      xor.addCircuit(and1); // order will matter for simulation!
      xor.addCircuit(not1);
      xor.addCircuit(or1);
      xor.addCircuit(and2);
      // better than xor.getCircuits().add(and1) etc.
      new Conexio(xor.getEntrada(1), and1.getEntrada(1));
      new Conexio(xor.getEntrada(1), or.getEntrada(1));
      new Conexio(xor.getEntrada(2), and1.getEntrada(2));
      new Conexio(xor.getEntrada(2), or.getEntrada(2));
      new Conexio(or.getSortida(1) , and2.getEntrada(1));
      new Conexio(and1.getSortida(1), not.getEntrada(1));
      new Conexio(not.getSortida(1), and2.getEntrada(2));
      new Conexio(and2.getSortida(1), xor.getSortida(1));

    }
}
