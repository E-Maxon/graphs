# graphs

exe-файл в папке graphs/Debug

Код программы в папке graphs/graphs

Чтобы запустить программу, нужно скачать всю папку graphs/Debug и запустить файл graphs.exe

Далее откроется программа. Можно вводить граф как графическим путем: при клике ЛКМ появляется новая вершина, кликами можно соединить 2 вершины ребром и затем, после ввода квадрата веса ребра, нажать Enter. Также можно двигать вершины. Либо можно задавать текстовым путем в формате: v u w через строку, где v, u -- вершины, w -- вес ребра(v->u) между этими вершинами. 

При нажатии на кнопку "GO!" начинается симуляция движения точек. Если нажать на кнопку "Pause", симуляция остановится. Если нажать еще раз, снова возобновиться. Чтобы вернуться во ввод, нажать "Stop".

Чтобы посмотреть график -- нажать "View plot". Чтобы выйти из просмотра графика -- нажать Esc.

Esc в остальных режимах - выход из программы

_____________________________________________________________________________________

Consider a metric graph. At the initial moment of time, points appear from the source s along all edges incident to s and move at unit speed. When k points, where k can take values from 1 to the degree of vertex v, are at the vertex of the graph, v new points appear, going along all edges incident to vertex v. Let N (T) be the number of points that move along the chart at time T. The functionality of the program allows you to enter a small graph and see the plot of the number of points versus time.

The code of the program lies in "./graphs/graphs path". You have to download the "graphs" folder and launch "./graphs/Debug/graphs.exe" file to run the program.

Next, the program will open. You can enter the graph graphically: when you click LMB, a new vertex appears, by clicking you can connect 2 vertices with an edge and then, after entering the square of the weight of the edge, press Enter. You can also move vertices. Or you can set it in a text way in the format: v u w through a line, where v, u are vertices, w is the weight of the edge (v->u) between these vertices. It is important to remember that the weights must be prime and distinct numbers.

When you click on the "GO!" the simulation of point movement starts. If you click on the "Pause" button, the simulation will stop. If you press again, it will resume again. To return to input, press "Stop".

To view the graph, click "View plot". To exit the chart view, press Esc.

Esc in other modes - exit the program
