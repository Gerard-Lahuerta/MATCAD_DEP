public class Pota extends Observable implements Observer {
  private boolean estat = false;
  public Pota(String str) {
    nom = str;
    clearChanged(); // from Observable, resets flag
  }
  public boolean isEstat() {
    return estat;
  }
  public void setEstat(boolean nouEstat) {
    estat = nouEstat;
    setChanged();
    notifyObservers(this);
  }
  public void update(Observable arg0, Object arg1) {
    setEstat( ((Pota) arg0).isEstat() );
  }
