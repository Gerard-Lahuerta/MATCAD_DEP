 public class Component extends Circuit {
   public Circuit(String nom, int numEnt, int numSort) {
     super(nom);
     for (int i = 1; i <= numEnt; i++){
       addEntrada(new Pota("entrada " + i));
      }
     for (int i = 1; i <= numSort; i++){
       addSortida(new Pota("sortida " + i));
      }
   }
}
