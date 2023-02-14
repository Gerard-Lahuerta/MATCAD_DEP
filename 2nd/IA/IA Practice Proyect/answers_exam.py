from SearchAlgorithm import *
from SubwayMap import *
from utils import *

if __name__=="__main__":
    ROOT_FOLDER = '../CityInformation/Barcelona_City/'
    map = read_station_information(os.path.join(ROOT_FOLDER, 'Stations.txt'))
    connections = read_cost_table(os.path.join(ROOT_FOLDER, 'Time.txt'))
    map.add_connection(connections)

    infoVelocity_clean = read_information(os.path.join(ROOT_FOLDER, 'InfoVelocity.txt'))
    map.add_velocity(infoVelocity_clean)



    ### BELOW HERE YOU CAN CALL ANY FUNCTION THAT yoU HAVE PROGRAMED TO ANSWER THE QUESTIONS OF THE EXAM ###
    ### this code is just for you, you won't have to upload it after the exam ###


    #this is an example of how to call some of the functions that you have programed

    example_path=uniform_cost_search(9, 3, map, 1)
    print_list_of_path_with_cost([example_path])
    #print(example_path.g)
    """
    PREGUNTA 1 EXEMPLES: CAMÍ UTILITZANT CERCA EN AMPLADA DE SAGRADA FAMILIA L2 A MARINA L1

    #puc mirar directament a la pregunta
    route1 = breadth_first_search(11, 6, map)
    #print(route1.route)

    PREGUNTA 2 EXEMPLES: QUIN SERÀ EL COST TOTAL G PEL CRITERI DISTANCIA DE LA RUTA AMB A* DESDE [44,139] FINS [293,101]
    optimal_path = Astar([228, 195], [111, 194], map, 1)
    #print(optimal_path.g)




     ---> TROBAR ID A PARTIR DEL NOM 
    estacions = map.stations
    #print(estacions[25]['name'])
    for estacio in estacions.keys():
        if estacions[estacio]['name'] == "Selva de Mar":
            id_sagrada = estacio
            #print(estacio)
            break;

    for estacio in estacions.keys():
        if map.stations[estacio]['name'] == "Marina":
            id_marina = estacio
            #print(estacio)
            break;

    stationID = coord2station([29, 104], map)
    #print([stationID])

    list_of_path = Path([4])
    updated_paths = expand(list_of_path, map)
    #for path in updated_paths:
        #print(path.route)

    #print(map.stations[5]['name'])
    #print(map.stations[3]['name'])


    route = uniform_cost_search(22,24,map,1)
    #print(route.route)
    #print(route.g)
    route = uniform_cost_search(19, 22, map, 1)
    # print(route.route)
    #print(route.g)

    list_of_path = [Path([10,23,22,21])]
    # arribar a 8 desde 12

    optimal_path = Astar([82, 217], [140, 27], map, 2)
    expand = [Path([5, 4, 7]), Path([5, 4, 3]), Path([5, 4, 0]), Path([5,4,1,2])]
    list_of_path = [Path([5, 4, 7]), Path([5, 4, 3]), Path([5, 4, 0]), Path([5,4,1,2])]
    list_of_path = [Path([10, 23, 22, 21])]
    remove_redundant_paths()
    """
