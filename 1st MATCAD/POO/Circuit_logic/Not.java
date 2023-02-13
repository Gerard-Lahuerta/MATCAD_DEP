public class Not extends Circuit {
 // default constructor without parameters
   public Not() {
     this.Not("or");
   }
   // alternative constructors
   public Not(String nom) {
     super(nom);
     addEntrada(new Pota("entrada 1"));
     addEntrada(new Pota("entrada 2"));
     addSortida(new Pota("sortida"));
   }
   public Not(String nom, int numEntrades) {
     super(nom);
     for (int i = 1; i <= numEntrades; i++) {
       addEntrada(new Pota("entrada " + i));
     }
     addSortida(new Pota("sortida"));
   }

   public void processa() {
     for (Pota potaEntrada : entrades) {
        if (potaEntrada.isEstat()==true){
          resultat = false;
        } else {resultat = true;}
     } setEstatSortida(resultat);
  }

}
