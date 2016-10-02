# Data Structures

The purpose of this project was to compare the time complexity and runtime behavior of some basic search algorithms. The `data.csv` file contains a large list of Companies which make up the input search set. The search algorithms were classified on id based searches, using an integer as an index and string based searches where the search was performed against strings.

## Project Architecture

The project follows a simple [Observer](https://en.wikipedia.org/wiki/Observer_pattern) pattern. Every search algorithm acts as an Observer on the data set ( Subject ). In that manner new algorithms could  be added seamlessly and be automatically notified to update their structures upon an insert - delete operation.

ID based algorithms  
* Linear search
* Binary search
* Binary Interpolation search
* AVL tree search

String based algorithms  
* Linear search
* Digital tree ( trie )

## Measurements

The `report.pdf` currently in greek, contains the theoretical and experimental results for every search algorithm in various set sizes, and a more in depth description of the class hierarchy of the project.