public class Observable(){
  public  __init__(this, nom_observable){
    this.list_noms = [];
    this.nom_observable = nom_observable;
  }
  public add_observer(this,observer){
      this.list_noms.append(observer);
  }
  public publish_news(this, event){
    for (Observer observer : this.list_noms){
     observer.update(observer, this.nom_observable, event);
    }
  }
}
