AUTORS:

* Sofia Di Capua Martín Mas : 1603685
* Andrea Gonzalez Aguilera : 1603921
* Ona Sánchez Núñez : 1601181


COMENTARIS DISSENY UML:

* Tenim una classe principal RandomForest, que és abstracta, i es relaciona mitjançant composition amb les interfícies Impurity i Node. A més, estableix una relació d’herència mitjançant un template method amb les classes RandomForestClassifier i RandomForestRegressor, que són els seus fills.

* La interfície Impurity s’implementa a les classes Gini, Entropy i MSE, és a dir, aquestes 3 classes comparteixen la mateixa estructura, això permet que es puguin afegir més criteris per calcular impureses en un futur de forma senzilla. S’ha utilitzar el patró de disseny strategy.

* D’altra banda la interfície Node es implementada per les classes Leaf i Parent. També, la classe Parent té dues associacions amb la classe Node anomenades leftChild i rightChild.

* Per últim, la classe Dataset està aïllada de les altres perquè no depèn de cap altre classe. La relació amb les altres classes es que les altres classes utilitzen objectes dataset però no tenen relació amb la classe en si. 


INSTRUCCIONS PER EXECUTAR EL CODI:


1. Descarregar tots els fitxer que contenen el codi: Practica_POO_2.py, dataset.py, impurity.py i dataset.py.
2. Estant al fitxer Practica_POO_2.py, el main ja està preparat per poder ser executat utilitzant iris dataset pel classificador i gini com criteri, sumSins pel regressor i MSE per calcular l’error.
3. Canviar la configuració del Run per evitar errors clicant: Run > Configuration per file… > Execute in an external system terminal.
4. Es demanarà a l’usuari client que introdueixi S o -, depenent de si vol o no fer Cross Validation. També es demanarà de la mateixa manera si es vol fer o no multiprocessing, tant pel classificador com pel regresor. 

Apunts importants:

* Al classificador es pot canviar el criteri per calcular la impuresa. Està el Gini per defecte, però es pot canviar per Entropy o afegir un nou criteri al fitxer impurity.py.
* Al regressor tenim dues opcions implementades per crear el dataset. Per defecte està el sumSins, que es pot canviar a sin.


OPTIMITZACIÓ

(Sense Cross Validation)

Classifier, Gini, Iris. Temps multiprocessing: 0.021. Temps optimitzant make_parent: 0.0020
Classifier, Gini, Entropy. Temps multiprocessing: 0.021. Temps optimitzant make_parent: 0.0018
Regressor, MSE, sumSins. Temps multiprocessing: 0.0217. Temps optimitzant make_parent: 0.0048
Regressor, MSE, sin. Temps multiprocessing: 0.022. Temps optimitzant make_parent: 0.0036
	
Classifier, Gini, Iris --> accuracy: 93%
Classifier, Entropy, Iris --> accuracy: 97%
Regressor, MSE, sumSins --> error: 0.36
Regressor, MSE, sin --> error: 0.29

A la taula surt el temps que es triga per arbre.
Es pot veure que triga més el multiprocessing que en seqüencial optimitzat, això pot ser degut a que treballem amb un dataset petit o a que l’estem executant en Windows. En un futur es podria considerar el “binder”.