all: main main_dfs main_bfs

main: main.cc graph.hh procedures.hh
	g++ -o main main.cc graph.hh procedures.hh

main_dfs: main.cc graph.hh procedures.hh
	g++ -o main_dfs main_dfs.cc graph.hh procedures.hh

main_bfs: main.cc graph.hh procedures.hh
	g++ -o main_bfs main_bfs.cc graph.hh procedures.hh

clean:
	rm main main_dfs main_bfs