import sys

def fileRead(edges):
    fileName = sys.argv[1]

    fp = open(fileName,'r')
    
    for line in fp:
        if len(line)>=6:
            edges.append(line.split())

def mkList(edges):
    adjList=dict()
    
    for edge in edges:
        if edge[0] in adjList:
            adjList.get(edge[0]).append((edge[1],edge[2]))
        else:
            adjList[edge[0]] = [(edge[1],edge[2])]
        if edge[1] in adjList:
            adjList.get(edge[1]).append((edge[0],edge[2]))
        else:
            adjList[edge[1]] = [(edge[0],edge[2])]
        
    return adjList

def extractMin(arr,dist):
    minDis = 9999

    for vert in arr:
        if vert in dist.keys():
            if dist.get(vert)<minDis:
                minDis = dist.get(vert)
                retVert = vert

    return retVert
    
def printPath(dist,prev,src,dest):
    target = list()
    while src!=dest:
        target.append((dest,dist[dest]-dist[prev[dest]]))
        dest = prev[dest]
    
    target.reverse()

    print src,
    for x in target:
        print "->",
        print "("+str(x[1])+")",
        print "->",
        print x[0],
    return

def dijkstra(adjList,src,dest):
    Q = set()
    dist = dict()
    prev = dict()
    
    for vert in adjList.keys():
        Q.add(vert)

    dist[src] = 0

    while Q:
        node = extractMin(Q,dist)
        Q.remove(node)
        for vert in adjList.get(node):
            if vert[0] in Q:
                alt = dist[node] + int(vert[1])
                if vert[0] in dist.keys():
                    if alt<dist[vert[0]]:
                        dist[vert[0]] = alt
                        prev[vert[0]] = node
                else:
                    dist[vert[0]] = alt
                    prev[vert[0]] = node

    printPath(dist,prev,src,dest)
        
if __name__ == "__main__":
    edges = []
    fileRead(edges)
    adjList = mkList(edges)
    src=raw_input("what is source vertex? ")
    dest=raw_input("what is destination vertex? ")
    if src in adjList.keys():
        if dest in adjList.keys():
            dijkstra(adjList, src, dest)
            exit()
    print "input exist vertex"
