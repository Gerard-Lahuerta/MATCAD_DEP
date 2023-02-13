public class Or extends Circuit {
 // default constructor without parameters
   public Or() {
     this.Or("or");
   }
   // alternative constructors
   public Or(String nom) {
     super(nom);
     addEntrada(new Pota("entrada 1"));
     addEntrada(new Pota("entrada 2"));
     addSortida(new Pota("sortida"));
   }
   public Or(String nom, int numEntrades) {
     super(nom);
     for (int i = 1; i <= numEntrades; i++) {
       addEntrada(new Pota("entrada " + i));
     }
     addSortida(new Pota("sortida"));
   }

   public void processa() {
     boolean resultat = true ;
     for (Pota potaEntrada : entrades) {
       resultat = resultat || potaEntrada.isEstat();
     }
    setEstatSortida(resultat);
  }
}
