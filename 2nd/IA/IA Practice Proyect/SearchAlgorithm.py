# This file contains all the required routines to make an A* search algorithm.
#
__authors__ = '1601181'
__group__ = 'TO_BE_FILLED'
# _________________________________________________________________________________________
# Intel.ligencia Artificial
# Curs 2020- 2021
# Universitat Autònoma de Barcelona
# _______________________________________________________________________________________

from SubwayMap import *
from utils import *
import os
import math
import copy

"""
EXPAND:
REP:
Un path pare i map.
RETORNA:
Paths igual al path pare però afegint els nodes connecten amb path pare.
CRIDA: 
expand([12,13,14], MAP)

A DINS FA:
estem és l'ultim node de la llista de paths
connexió té totes les connexions amb l'últim node del path

nou_nou_path té la ruta del path pare i li afegim la estació que connecta
connexions.keys() --> estacions amb que connecta una estació
guarda el cost amb path.g
"""

def expand(path, map):
    """
     It expands a SINGLE station and returns the list of class Path.
     Format of the parameter is:
        Args:
            path (object of Path class): Specific path to be expanded
            map (object of Map class):: All the information needed to expand the node
        Returns:
            path_list (list): List of paths that are connected to the given path.
    """
    estem = path.last
    connexions = map.connections[estem]
    path_list = []
    nou_path = path.route

    for clau in connexions.keys():
        nou_nou_path = Path(nou_path.copy())
        nou_nou_path.add_route(clau)
        nou_nou_path.g = path.g
        path_list.append(nou_nou_path)

    return path_list


"""
REMOVE CYCLES:
REP:
Llista de paths
RETORNA:
Llista de paths sense repeticions
CRIDA: 
expand([12,13,14], [11,12,13])

A DINS FA:
per cada cami dins del path, per cada element del camí, mira que no hi hagi repeticons
si no hi ha cap repetició, afegeig en camí a la llista que retorna.
"""
def remove_cycles(path_list):
    """
     It removes from path_list the set of paths that include some cycles in their path.
     Format of the parameter is:
        Args:
            path_list (LIST of Path Class): Expanded paths
        Returns:
            path_list (list): Expanded paths without cycles.
    """
    lista = []
    for cami in path_list:
        error = False
        for i in cami.route:
            contador = 0
            for j in cami.route:
                if i == j:
                    contador += 1
                    if contador > 1:
                        contador = -1
                        break
            if contador == -1:
                error = True
                break
        if len(cami.route) and not error:
            lista.append(cami)
    return lista

"""
INSERT DAVANT!:
REP:
Llista de paths expandits, llista del paths
FA:
Uneix les llistes afegint els nou path davant.
"""

def insert_depth_first_search(expand_paths, list_of_path):
    """
     expand_paths is inserted to the list_of_path according to DEPTH FIRST SEARCH algorithm
     Format of the parameter is:
        Args:
            expand_paths (LIST of Path Class): Expanded paths
            list_of_path (LIST of Path Class): The paths to be visited
        Returns:
            list_of_path (LIST of Path Class): List of Paths where Expanded Path is inserted
    """

    return expand_paths + list_of_path


def depth_first_search(origin_id, destination_id, map):
    """
     Depth First Search algorithm
     Format of the parameter is:
        Args:
            origin_id (int): Starting station id
            destination_id (int): Final station id
            map (object of Map class): All the map information
        Returns:
            list_of_path[0] (Path Class): the route that goes from origin_id to destination_id
    """

    llista = [Path([origin_id])]

    while len(llista):
        llista = insert_depth_first_search( remove_cycles(expand(llista[0], map)), llista[1:])

        if llista[0].last == destination_id:
            return llista[0]

    return "No existeix solució"

"""
INSERT DARRERE!:
REP:
Llista de paths expandits, llista del paths
FA:
Uneix les llistes afegint els nou path darrere.
"""
def insert_breadth_first_search(expand_paths, list_of_path):
    """
        expand_paths is inserted to the list_of_path according to BREADTH FIRST SEARCH algorithm
        Format of the parameter is:
           Args:
               expand_paths (LIST of Path Class): Expanded paths
               list_of_path (LIST of Path Class): The paths to be visited
           Returns:
               list_of_path (LIST of Path Class): List of Paths where Expanded Path is inserted
    """

    return list_of_path + expand_paths


def breadth_first_search(origin_id, destination_id, map):
    """
     Breadth First Search algorithm
     Format of the parameter is:
        Args:
            origin_id (int): Starting station id
            destination_id (int): Final station id
            map (object of Map class): All the map information
        Returns:
            list_of_path[0] (Path Class): The route that goes from origin_id to destination_id
    """

    llista = [Path([origin_id])]

    while len(llista):
        llista = insert_breadth_first_search(remove_cycles(expand(llista[0], map)), llista[1:])

        if llista[0].last == destination_id:
            return llista[0]

    return "No existeix solució"


"""
CALCULATE COST:
REP:
llista paths fills, map, criteri --> parades, temps, distancia, transbords
RETORNA:
llista paths fills amb path.update_g(cost)

FA:
---> Parades:
            cost 1, com a mínim queda 1 parada fins el final

---> Temps:
            cost map.connections[path.penultimate][path.last], donat per la matriu

---> Distancia:
            mirem que ultim i penultim siguin diferents amb:
                map.stations[path.penultimate]["x"]
            distancia = temps * velocitat 
            temps --> map.connections[path.penultimate][path.last]
            velocitat --> map.stations[path.last]["velocity"]
            
---> Transbords:
            mirem que les línies siguin diferents amb:
                map.stations[path.penultimate]["line"]
            cost 1
"""

def calculate_cost(expand_paths, map, type_preference=0):
    """
         Calculate the cost according to type preference
         Format of the parameter is:
            Args:
                expand_paths (LIST of Paths Class): Expanded paths
                map (object of Map class): All the map information
                type_preference: INTEGER Value to indicate the preference selected:
                                0 - Adjacency
                                1 - minimum Time
                                2 - minimum Distance
                                3 - minimum Transfers
            Returns:
                expand_paths (LIST of Paths): Expanded path with updated cost
    """

    if type_preference==0 :
        for path in expand_paths:
            path.update_g(1)
    elif type_preference==1 :
        for path in expand_paths:
            time = map.connections[path.penultimate][path.last]
            path.update_g(time)
    elif type_preference==2 :
        for path in expand_paths:
            """distancia = velocitat * temps"""
            if map.stations[path.penultimate]["x"] != map.stations[path.last]["x"] or map.stations[path.penultimate]["y"] != map.stations[path.last]["y"]:
                time = map.connections[path.penultimate][path.last]
                velocity = map.stations[path.last]["velocity"]
                distance = time * velocity
                path.update_g(distance)
    else:
        for path in expand_paths:
            if map.stations[path.penultimate]["line"] != map.stations[path.last]["line"]:
                path.update_g(1)

    return expand_paths

"""
INSERT SEGONS COST G:
REP:
Llista de paths expandits, llista del paths
FA:
Uneix les llistes de path a nou_path
agafa tots els costos, els posa a una llista i els ordena amb sort()
Afegeix els path en ordre de costos (sense repetir paths!)

"""


def insert_cost(expand_paths, list_of_path):
    """
        expand_paths is inserted to the list_of_path according to COST VALUE
        Format of the parameter is:
           Args:
               expand_paths (LIST of Path Class): Expanded paths
               list_of_path (LIST of Path Class): The paths to be visited
           Returns:
               list_of_path (LIST of Path Class): List of Paths where expanded_path is inserted according to cost
    """
    """
    --> podria fer servir el sorted! tipo:
    def tururu(path):
        return path.g
        
    return sorted(list_of_path+expand_paths, key=tururu)
    """
    nou_path = list_of_path + expand_paths
    final = []
    for path in nou_path:
        final.append(path.g)

    final.sort()

    lista = []
    for cost in final:
        for path in nou_path:
            if path.g == cost and path not in lista:
                lista.append(path)

    return lista

def uniform_cost_search(origin_id, destination_id, map, type_preference=0):
    """
     Uniform Cost Search algorithm
     Format of the parameter is:
        Args:
            origin_id (int): Starting station coordenades
            destination_id (int): Final station coordenades
            map (object of Map class): All the map information
        Returns:
            list_of_path[0] (Path Class): The route that goes from origin_id to destination_id
    """
    inici = Path(origin_id)
    llista = [inici]

    while llista[0].last != destination_id and len(llista) != 0:
        c = llista[0]
        E = expand(c, map)
        Eaux = remove_cycles(E)
        Emod = calculate_cost(Eaux, map, type_preference)
        llista = insert_cost(Emod, llista[1:])

    if len(llista) != 0:
        return llista[0]

    return "No existeix Solucio"


"""
CALCULATE HEURISTICS:
REP:
llista paths fills, map, destí, criteri --> parades, temps, distancia, transbords
RETORNA:
llista paths fills amb path.update_h(cost_heuristica)

FA:
---> Parades:
            path.update_h(1), com a mínim queda 1 parada fins el final

---> Temps:
            Temps = Distancia/velocitat
            Velocitat: maxima velocitat que hi ha --> map.stations[clau]["velocity"]
            Distancia: euclidia de path.last a destination_id

---> Distancia:
            Distancia: euclidia de path.last a destination_id
            Es fa amb map.stations[path.last]["x"]...

---> Transbords:
            Mirem que linies diferents path.update(1)
"""


def calculate_heuristics(expand_paths, map, destination_id, type_preference=0):
    """
     Calculate and UPDATE the heuristics of a path according to type preference
     WARNING: In calculate_cost, we didn't update the cost of the path inside the function
              for the reasons which will be clear when you code Astar (HINT: check remove_redundant_paths() function).
     Format of the parameter is:
        Args:
            expand_paths (LIST of Path Class): Expanded paths
            map (object of Map class): All the map information
            type_preference: INTEGER Value to indicate the preference selected:
                            0 - Adjacency
                            1 - minimum Time
                            2 - minimum Distance
                            3 - minimum Transfers
        Returns:
            expand_paths (LIST of Path Class): Expanded paths with updated heuristics
    """

    if type_preference == 0:
        for path in expand_paths:
            if path.last != destination_id:
                path.update_h(1)

    elif type_preference == 1:
        for path in expand_paths:

            maxvelocity = 0
            for node in map.stations:
                if map.stations[node]["velocity"] > maxvelocity:
                    maxvelocity = map.stations[node]["velocity"]


            distancia = ((map.stations[destination_id]['x'] - map.stations[path.last]['x']) ** 2 + (map.stations[destination_id]['y'] - map.stations[path.last]['y']) ** 2) ** (1 / 2)
            path.update_h(distancia/maxvelocity)

    elif type_preference == 2:
        for path in expand_paths:
            distancia = ((map.stations[destination_id]['x'] - map.stations[path.last]['x']) ** 2 + (map.stations[destination_id]['y'] - map.stations[path.last]['y']) ** 2) ** (1 / 2)
            path.update_h(distancia)
    else:
        for path in expand_paths:
            if map.stations[path.last]["line"] != map.stations[destination_id]["line"]:
                path.update_h(1)

    return expand_paths

"""
REP: llista de paths fills
RETORNA: llista de paths fills
FA: per cada path fa un update de la f (ell sol ja suma g + h)
"""
def update_f(expand_paths):
    """
      Update the f of a path
      Format of the parameter is:
         Args:
             expand_paths (LIST of Path Class): Expanded paths
         Returns:
             expand_paths (LIST of Path Class): Expanded paths with updated costs
    """
    for path in expand_paths:
        path.update_f()

    return expand_paths

"""
REP: 
Paths fills
Paths que tenim
Diccionari amb els costos parcials

RETORNA:
Paths fills sense camins pitjors als que ja teniem
Paths que tenim sense camins pitjors als trobats
Diccionari de costos actualitzats dels nodes visitats

DICCIONARI --> clau = path.last
               conté = path.g

FA:
---> Pels fills:
        Si el node no al diccionari (no passat), l'afegim a la llista de camins fills bons i guardem el cost al diccionari
        Si el node ja és al diccionari, si el cost és millor, l'afegim a la llista de camins fills bons i el guardem al dic
---> Pels paths:
        Si el node no al diccionari (no passat), l'afegim a la llista de camins bons i guardem a dic.
        Si el node ja és al diccionari, si el cost és millor, l'afegim a la llista de camins bons i el guardem al dic.
"""

def remove_redundant_paths(expand_paths, list_of_path, visited_stations_cost):
    """
      It removes the Redundant Paths. They are not optimal solution!
      If a station is visited and have a lower g in this moment, we should remove this path.
      Format of the parameter is:
         Args:
             expand_paths (LIST of Path Class): Expanded paths
             list_of_path (LIST of Path Class): All the paths to be expanded
             visited_stations_cost (dict): All visited stations cost
         Returns:
             new_paths (LIST of Path Class): Expanded paths without redundant paths
             list_of_path (LIST of Path Class): list_of_path without redundant paths
    """
    new_paths = []
    list_of_path_without_red = []

    for path in expand_paths:
        if path.last not in visited_stations_cost.keys():
            new_paths.append(path)
            visited_stations_cost[path.last] = path.g
        elif path.g <= visited_stations_cost[path.last]:
            new_paths.append(path)
            visited_stations_cost[path.last] = path.g

    for path in list_of_path:
        if path.last not in visited_stations_cost.keys():
            list_of_path_without_red.append(path)
            visited_stations_cost[path.last] = path.g
        elif path.g <= visited_stations_cost[path.last]:
            list_of_path_without_red.append(path)
            visited_stations_cost[path.last] = path.g

    return new_paths, list_of_path_without_red, visited_stations_cost

"""
INSERT COST F:
REP: llista fills
     llista paths
RETORNA: Unió de les llistes ordenat segons la f

FA: ordena segons la f
            sorted(llista a ordenar, key = per quin valor volem ordenar)
"""

def order(x):
    return x.f
def insert_cost_f(expand_paths, list_of_path):
    """
        expand_paths is inserted to the list_of_path according to f VALUE
        Format of the parameter is:
           Args:
               expand_paths (LIST of Path Class): Expanded paths
               list_of_path (LIST of Path Class): The paths to be visited
           Returns:
               list_of_path (LIST of Path Class): List of Paths where expanded_path is inserted according to f
    """

    return sorted(list_of_path + expand_paths, key=order)

"""
COORD2STATION
REP: coordenades
     map
RETORNA: llista id estacions més properes

FA:
Guarda totes les estacions del mapa
Per cada estació calcula la distancia euclidia entre les coordenades d'on estem i l'estació
    --> posar-ho com [x,y]
    --> Ens quedem amb el minim
Afegim a la llista que retornem els ids (claus del diccionari) que estan a dist minima.

"""


def coord2station(coord, map):
    """
        From coordinates, it searches the closest station.
        Format of the parameter is:
        Args:
            coord (list):  Two REAL values, which refer to the coordinates of a point in the city.
            map (object of Map class): All the map information
        Returns:
            possible_origins (list): List of the Indexes of stations, which corresponds to the closest station
    """

    estacions = map.stations
    minim = 1000000000
    llista = []
    for estacio in estacions.keys():
        coord_estacio = [estacions[estacio]['x'], estacions[estacio]['y']]
        distancia = euclidean_dist(coord, coord_estacio)
        if (distancia <= minim):
            minim = distancia

    for estacio in estacions.keys():
        coord_estacio = [estacions[estacio]['x'], estacions[estacio]['y']]
        distancia = euclidean_dist(coord, coord_estacio)
        if (distancia == minim):
            llista.append(estacio)

    return llista

"""
IMPORTANT --> hi haurà sempre 1 sola origin_id, però pot haver molts final_id
          --> cal un diccionari dic per fer el redundant path, l'inicialitzem amb dic = dict()
"""
def Astar(origin_coor, dest_coor, map, type_preference=0):
    """
     A* Search algorithm
     Format of the parameter is:
        Args:
            origin_id (list): Starting station coordenades
            destination_id (int): Final station coordenades
            map (object of Map class): All the map information
            type_preference: INTEGER Value to indicate the preference selected:
                            0 - Adjacency
                            1 - minimum Time
                            2 - minimum Distance
                            3 - minimum Transfers
        Returns:
            list_of_path[0] (Path Class): The route that goes from origin_id to destination_id
    """


    origin_id = coord2station(origin_coor, map)
    destination_id = coord2station(dest_coor, map)[0]
    llista = []

    for i in origin_id:
        inici = Path(i)
        llista.append(inici)
    dic = dict()

    while llista[0].last != destination_id and len(llista) != 0:
        c = llista[0]
        E = expand(c, map)

        Eaux = remove_cycles(E)

        Eaux = calculate_cost(Eaux, map, type_preference)

        Eaux, llista, dic = remove_redundant_paths(Eaux, llista, dic)

        Eaux = calculate_heuristics(Eaux, map, destination_id, type_preference)
        Eaux = update_f(Eaux)
        llista = insert_cost_f(Eaux, llista[1:])

    if len(llista) != 0:
        return llista[0]

    return "No existeix Solució"