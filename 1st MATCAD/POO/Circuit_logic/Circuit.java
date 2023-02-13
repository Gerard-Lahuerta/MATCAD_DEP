public abstract class Circuit {
  private List<Pota> entrades = new ArrayList<Pota>();
  public void addEntrada(Pota pota) {
    entrades.add(pota);
 public Pota getPotaEntrada(int numPota) {
   return entrades.toArray()[numPota-1];
  }
  public boolean isEstatEntrada(int numEntrada) {
    return getPotaEntrada(numEntrada).isEstat();
  }
  public boolean setEstatEntrada(int numEntrada, boolean estat) {
    return getPotaEntrada(numEntrada).setEstat(estat);
  }
  public abstract void processa();
}
