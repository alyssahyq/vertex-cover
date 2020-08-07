import re
import sys

#Feb 12 11:46 fixed format issue
#Feb 7 14:28 add ,sep=''
#[verified] regular expressions of 4 kinds of command
re_add = '(\s)*a(\s)+"([a-zA-z]|(\s))*"(\s)+(\()(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?,(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?(\))((/s)|(\()(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?,(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?(\)))*'
re_change = '(\s)*c(\s)+"([a-zA-z]|(\s))*"(\s)+(\()(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?,(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?(\))((/s)|(\()(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?,(\s)*[-]?([0-9]|(\s))*(\.([0-9]|(\s))*)?(\)))*'
re_remove = '(\s)*r(\s)+"([a-zA-z]|(\s))*"(\s)*'
re_generate = '(\s)*g(\s)*'

#store original datas from input
org_data = dict()
# store all the intersection
all_intersection = set()

def str_to_num(string):
    if ('.' in string):
        return float(string)
    else:
        return int(string)

def cal_equation(cdnt1,cdnt2):#verified
    x1 = cdnt1[0]
    y1 = cdnt1[1]
    x2 = cdnt2[0]
    y2 = cdnt2[1]
    if x1 == x2:
        if y1 == y2:
            print("Error: Repeated input.",file=sys.stderr)
            return [0,0,0]
        else:
            a = 1
            b = 0
            c = -x1
    else:
        if y1 == y2:
            a = 0
            b = 1
            c = -y1
        else:
            a = (y2 - y1)/(x1-x2)
            b = 1
            c = (x2 * y1 - x1 * y2)/(x1-x2)
    return [a,b,c]

def g_edge(org_data):
    all_edge = list()
    for str in org_data.values():
        #print(str)
        for i in range(len(str)-1):
            this_edge = list()
            two_vertices = set()
            two_vertices.add(str[i])
            two_vertices.add(str[i+1])
            equation = cal_equation(str[i],str[i+1])
            this_edge.append(two_vertices)
            this_edge.append(equation)
            all_edge.append(this_edge)
    return all_edge

def g_graph(all_edge):
    for i in range(0, len(all_edge)):
        k = i + 1
        for j in range(k, len(all_edge)):
            if len(all_edge[i][0].intersection(all_edge[j][0])) == 2: #overlap completely [verified]
                del all_edge[j]
                return g_graph(all_edge)
            if len(all_edge[i][0].intersection(all_edge[j][0])) == 1: #shared one point
                if ((all_edge[i][1][0]==all_edge[j][1][0])and (all_edge[i][1][1]==all_edge[j][1][1])and(all_edge[i][1][2]==all_edge[j][1][2])): # whether partly overlap
                    all_edge_i = all_edge[i][0].copy()
                    all_edge_j = all_edge[j][0].copy()
                    shared_vertex = all_edge_i.intersection(all_edge_j).pop()
                    all_edge_i.discard(shared_vertex)
                    all_edge_j.discard(shared_vertex)
                    shared_equation = all_edge[i][1]
                    another_vertex_i = all_edge_i.pop()
                    another_vertex_j = all_edge_j.pop()
                    temp_list = [shared_vertex,another_vertex_i,another_vertex_j]
                    temp_list.sort(key=lambda x: (x[0], x[1]))
                    ############
                    if (shared_vertex == temp_list[0]):
                        all_intersection.add(temp_list[1])
                        pair1 = set()
                        pair1.add(shared_vertex)
                        pair1.add(temp_list[1])
                        pair2 = set()
                        pair2.add(temp_list[1]) # another_vertec_i is intersection
                        pair2.add(temp_list[2])
                        edge1 = list()
                        edge1.append(pair1)
                        edge1.append(shared_equation)
                        edge2 = list()
                        edge2.append(pair2)
                        edge2.append(shared_equation)
                        del all_edge[i]
                        del all_edge[j - 1]
                        all_edge.append(edge1)
                        all_edge.append(edge2)
                        return g_graph(all_edge)
                    elif(shared_vertex == temp_list[2] ):
                        all_intersection.add(temp_list[1])
                        pair1 = set()
                        pair1.add(shared_vertex)
                        pair1.add(temp_list[1])
                        pair2 = set()
                        pair2.add(temp_list[1])  # another_vertecx_j is intersection
                        pair2.add(temp_list[0])
                        edge1 = list()
                        edge1.append(pair1)
                        edge1.append(shared_equation)
                        edge2 = list()
                        edge2.append(pair2)
                        edge2.append(shared_equation)
                        del all_edge[i]
                        del all_edge[j - 1]
                        all_edge.append(edge1)
                        all_edge.append(edge2)
                        return g_graph(all_edge)
                    else:
                        continue
                else:
                    continue
            if len(all_edge[i][0].intersection(all_edge[j][0])) == 0:
                #"may have intersection
                a1 = all_edge[i][1][0]
                b1 = all_edge[i][1][1]
                c1 = all_edge[i][1][2]
                a2 = all_edge[j][1][0]
                b2 = all_edge[j][1][1]
                c2 = all_edge[j][1][2]
                l1_v1_x = list(all_edge[i][0])[0][0] #x of vertex 1 in line 1
                l1_v1_y = list(all_edge[i][0])[0][1]
                l1_v2_x = list(all_edge[i][0])[1][0]
                l1_v2_y = list(all_edge[i][0])[1][1]
                l2_v1_x = list(all_edge[j][0])[0][0] # x of vertex 1 in line 2
                l2_v1_y = list(all_edge[j][0])[0][1]
                l2_v2_x = list(all_edge[j][0])[1][0]
                l2_v2_y = list(all_edge[j][0])[1][1]
                l1_v1 = ((l1_v1_x, l1_v1_y))
                l1_v2 = ((l1_v2_x, l1_v2_y))
                l2_v1 = ((l2_v1_x, l2_v1_y))
                l2_v2 = ((l2_v2_x, l2_v2_y))
                if (a1 == a2) and (b1 == b2):
                    if c1 == c2:  # overlap
                        #on the same straight line
                        if ((min(l1_v1_x,l1_v2_x) > max(l2_v1_x,l2_v2_x)) or (min(l2_v1_x,l2_v2_x) > max(l1_v1_x,l1_v2_x)) or (min(l1_v1_y,l1_v2_y) > max(l2_v1_y,l2_v2_y))or(min(l2_v1_y,l2_v2_y) > max(l1_v1_y,l1_v2_y))):
                            continue
                        else:
                            new_edge_list = [l1_v1, l1_v2, l2_v1, l2_v2]
                            new_edge_list.sort(key=lambda x: (x[0], x[1]))
                            new_edge1 = [{new_edge_list[0], new_edge_list[1]}, [a1, b1, c1]]
                            new_edge2 = [{new_edge_list[1], new_edge_list[2]}, [a1, b1, c1]]
                            new_edge3 = [{new_edge_list[2], new_edge_list[3]}, [a1, b1, c1]]
                            del all_edge[i]
                            del all_edge[j - 1]
                            all_edge.append(new_edge1)
                            all_edge.append(new_edge2)
                            all_edge.append(new_edge3)
                            all_intersection.add(new_edge_list[1])
                            all_intersection.add(new_edge_list[2])
                            return g_graph(all_edge)
                    else: #c1!=c2 --> parallel
                        continue
                else:#may have intersection
                    #coordinates of intersection
                    l1_v1 = ((l1_v1_x, l1_v1_y))
                    l1_v2 = ((l1_v2_x, l1_v2_y))
                    l2_v1 = ((l2_v1_x, l2_v1_y))
                    l2_v2 = ((l2_v2_x, l2_v2_y))
                    x_i = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)
                    y_i = -(a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1)
                    itst = (x_i,y_i)
                    #whether a T-cross
                    if x_i == l1_v1_x and y_i == l1_v1_y:
                        if min(l1_v1_x, l1_v2_x) <= x_i <= max(l1_v1_x, l1_v2_x) and min(l2_v1_x,l2_v2_x) <= x_i <= max(l2_v1_x,
                                                                                                                l2_v2_x) and min(
                                l1_v1_y, l1_v2_y) <= y_i <= max(l1_v1_y, l1_v2_y) and min(l2_v1_y,
                                                                                          l2_v2_y) <= y_i <= max(
                                l2_v1_y, l2_v2_y):
                            del all_edge[j]
                            new_edge1 = [{l1_v1,l2_v1},cal_equation(l1_v1,l2_v1)]
                            new_edge2 = [{l1_v1,l2_v2},cal_equation(l1_v1,l2_v2)]
                            all_edge.append(new_edge1)
                            all_edge.append(new_edge2)
                            all_intersection.add(l1_v1)
                            return g_graph(all_edge)
                        else:
                            continue
                    if x_i == l1_v2_x and y_i == l1_v2_y:
                        if min(l1_v1_x, l1_v2_x) <= x_i <= max(l1_v1_x, l1_v2_x) and min(l2_v1_x,
                                                                                         l2_v2_x) <= x_i <= max(l2_v1_x,
                                                                                                                l2_v2_x) and min(
                                l1_v1_y, l1_v2_y) <= y_i <= max(l1_v1_y, l1_v2_y) and min(l2_v1_y,
                                                                                          l2_v2_y) <= y_i <= max(
                                l2_v1_y, l2_v2_y):
                            del all_edge[j]
                            new_edge1 = [{l1_v2, l2_v1}, cal_equation(l1_v2, l2_v1)]
                            new_edge2 = [{l1_v2, l2_v2}, cal_equation(l1_v2, l2_v2)]
                            all_edge.append(new_edge1)
                            all_edge.append(new_edge2)
                            all_intersection.add(l1_v2)
                            return g_graph(all_edge)
                        else:
                            continue
                    if x_i == l2_v1_x and y_i == l2_v1_y:
                        if min(l1_v1_x, l1_v2_x) <= x_i <= max(l1_v1_x, l1_v2_x) and min(l2_v1_x,
                                                                                         l2_v2_x) <= x_i <= max(l2_v1_x,
                                                                                                                l2_v2_x) and min(
                                l1_v1_y, l1_v2_y) <= y_i <= max(l1_v1_y, l1_v2_y) and min(l2_v1_y,
                                                                                          l2_v2_y) <= y_i <= max(
                                l2_v1_y, l2_v2_y):
                            del all_edge[i]
                            new_edge1 = [{l2_v1, l1_v1}, cal_equation(l2_v1, l1_v1)]
                            new_edge2 = [{l2_v1, l1_v2}, cal_equation(l2_v1, l1_v2)]
                            all_edge.append(new_edge1)
                            all_edge.append(new_edge2)
                            all_intersection.add(l2_v1)
                            return g_graph(all_edge)
                        else:
                            continue
                    if x_i == l2_v2_x and y_i == l2_v2_y:
                        if min(l1_v1_x,l1_v2_x) <= x_i <= max(l1_v1_x,l1_v2_x) and min(l2_v1_x,l2_v2_x) <= x_i <= max(l2_v1_x,l2_v2_x) and min(l1_v1_y,l1_v2_y) <= y_i <= max(l1_v1_y,l1_v2_y) and min(l2_v1_y,l2_v2_y) <= y_i <= max(l2_v1_y,l2_v2_y) :
                            del all_edge[i]
                            new_edge1 = [{l2_v2, l1_v1}, cal_equation(l2_v2, l1_v1)]
                            new_edge2 = [{l2_v2, l1_v2}, cal_equation(l2_v2, l1_v2)]
                            all_edge.append(new_edge1)
                            all_edge.append(new_edge2)
                            all_intersection.add(l2_v2)
                            return g_graph(all_edge)
                        else:
                            continue
                    if min(max(l1_v1_x,l1_v2_x),max(l2_v1_x,l2_v2_x)) >= x_i >= max(min(l1_v1_x,l1_v2_x),min(l2_v1_x,l2_v2_x)) and min(max(l1_v1_y,l1_v2_y),max(l2_v1_y,l2_v2_y)) >= y_i >= max(min(l1_v1_y,l1_v2_y),min(l2_v1_y,l2_v2_y)):
                        del all_edge[i]
                        del all_edge[j-1]
                        new_edge1 = [{itst, l1_v1}, cal_equation(itst, l1_v1)]
                        new_edge2 = [{itst, l1_v2}, cal_equation(itst, l1_v2)]
                        new_edge3 = [{itst, l2_v1}, cal_equation(itst, l2_v1)]
                        new_edge4 = [{itst, l2_v2}, cal_equation(itst, l2_v2)]
                        all_edge.append(new_edge1)
                        all_edge.append(new_edge2)
                        all_edge.append(new_edge3)
                        all_edge.append(new_edge4)
                        all_intersection.add((x_i,y_i))
                        return g_graph(all_edge)
                    else:
                        continue
            else:
                print("Error: unkown error")
            j = j + 1
        i = i + 1
    return all_edge


def add():
    add_strName = re.findall('"(.+?)"',command)[0]
    add_cdnt = re.findall('\((.+?)\)', command)
    add_index = 0
    cdnt_list = list()
    cdnt_set = set() #whether repeated vertices is input
    for key in org_data.keys():
        if re.fullmatch(add_strName,key,re.IGNORECASE): 
            print("Error: This street is already exists.",file=sys.stderr)
            return 0
        else:
            continue
    while add_index < (len(add_cdnt)):
        #remove all the white-space in the coordinates
        temp = re.sub('\s','', add_cdnt[add_index])
        #temp = x,y
        if re.findall('(.*),',temp)[0]:
            x = str_to_num(re.findall('(.*),',temp)[0])
        else:
            print("Error: Wrong input, missing digit",file=sys.stderr)
            return 0
        if re.findall(',(.*)',temp)[0]:
            y = str_to_num(re.findall(',(.*)',temp)[0])
        else:
            print("Error: Wrong input, missing digit",file=sys.stderr)
            return 0
        crdt_single = (x,y)
        if crdt_single in cdnt_set:
            add_index = add_index + 1
            continue
        else:
            cdnt_set.add(crdt_single)
            cdnt_list.append(crdt_single)
            add_index = add_index + 1
    cdnt_tuple = tuple(cdnt_list)
    org_data[add_strName]=cdnt_tuple

def remove():
    remove_strName = re.findall('"(.+?)"',command)[0]
    flag = 0
    for key in org_data.keys():
        if re.match(remove_strName,key,re.IGNORECASE):
            flag = 1
            break
        else:
            continue
    if flag:
        del org_data[key]
    else:
        print("Error: 'c' or 'r' specified for a street that does not exist.",file=sys.stderr)

def change():
    remove()
    add()

def generate():
    all_intersection.clear()
    for key_i in org_data.keys():
        for key_j in org_data.keys():
            if key_i == key_j:
                continue
            else:
                for cdnt_i in org_data[key_i]:
                    for cdnt_j in org_data[key_j]:
                        if cdnt_i == cdnt_j:
                            shared_point = cdnt_i
                            all_intersection.add(shared_point)
                        else:
                            continue
    all_edge = g_edge(org_data)
    result = g_graph(all_edge)
    selected_result = list()
    for single_result in result:
        if single_result[0].intersection(all_intersection):
            selected_result.append(single_result)
        else:
            continue
    vertex_set = set()
    vertex_index = list()
    for edge in selected_result:
        temp_list = list(edge[0])
        if (temp_list[0] in vertex_set) == False:
            vertex_set.add(temp_list[0])
            vertex_index.append(temp_list[0])
        if (temp_list[1] in vertex_set) == False:
            vertex_set.add(temp_list[1])
            vertex_index.append(temp_list[1])
    print("V = {")
    for ind in range(len(vertex_index)):
        print(" ",ind,":  (","{0:.2f}".format(float(vertex_index[ind][0])),",","{0:.2f}".format(float(vertex_index[ind][1])),")",sep='')
    print("}")
    print("E = {")
    for edge in selected_result:
        temp_edge = list(edge[0])
        if selected_result.index(edge) == (len(selected_result)-1):
            print(" <",vertex_index.index(temp_edge[0]),",",vertex_index.index(temp_edge[1]),">",sep='')
        else:
            print(" <", vertex_index.index(temp_edge[0]), ",", vertex_index.index(temp_edge[1]), ">,",sep='')
    print("}")

while True:
    try:
        command = sys.stdin.readline()
        if command == '':
           break
        if re.match(re_add,command):
            add()
            continue
        elif re.match(re_change,command):
            change()
            continue
        elif re.match(re_remove, command):
            remove()
            continue
        elif re.match(re_generate, command):
            generate()
            continue
        else:
            print('Error: Wrong input.',file=sys.stderr)
    except EOFError:
        sys.exit(0)
