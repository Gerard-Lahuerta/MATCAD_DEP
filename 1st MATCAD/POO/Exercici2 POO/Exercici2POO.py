# -*- coding: utf-8 -*-
"""
Created on Thu Apr  1 10:46:14 2021

@author: onasa
"""





from abc import ABC, abstractmethod

class Observable(ABC):
    
    @abstractmethod
    
    def __init__(self, nom_observable):
        self.list_noms = []
        self.nom_observable = nom_observable #Reuters i EFE
        
    def add_observer(self,observer):
        self.list_noms.append(observer) #la vanguardia, el país, joan

    def publish_news(self, event):
        for observer in self.list_noms:
            observer.update(observer, self.nom_observable, event)
            
 
class NewsAgency(Observable):
    
    def __init__(self, nom_observable):
        super().__init__(nom_observable)
    
    def add_observer(self, observer):
        super().add_observer((observer))
        
    def publish_news(self,event):
        super().publish_news(event)
        

class Observer():
    
    def __init__(self, observer):
        pass
    def update(self, observer, nom_observable, event):
        pass
    def __str__(self):
        pass
    
    
class Newspaper(Observer):
    
    def __init__(self, observer):
        self.observer = observer
    
    def __str__(self):
        return self.observer
        
    def update(self, observer, nom_observable, event):
        print(str(observer) + " ( " + nom_observable + " ) : " + event)
        

class SMS(Observer):
    
    def __init__(self,telefon):
        self.telefon=telefon
        
    def __str__(self):
        return str(self.telefon)
    
    def update(self, telefon, nom_observable, event):
        print(str(telefon) + " ( " + nom_observable + " ) : " + event)  
        

if __name__ == "__main__":
    reuters = NewsAgency('Reuters') #observable
    efe = NewsAgency('EFE') #observable
    la_vanguardia = Newspaper("La Vanguardia") #observador
    el_pais = Newspaper("El Pais") #observador
    joan = SMS(626347612) #observador
    reuters.add_observer(la_vanguardia)
    reuters.add_observer(el_pais)
    efe.add_observer(el_pais)
    reuters.add_observer(joan)
    reuters.publish_news('PSOE winner of european elections in Spain')
    efe.publish_news('El partit socialista guanya les eleccions europees')
    
    
