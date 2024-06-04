# stl-algorithms
Practice tutorial of various stl algorithms that read and manipulate data from a file

A C++ practice tutorial, creating a module that accepts a command-line argument name of a file containing records corresponding to a collection of songs. 

The records contain information about the title, artist, album, year, length, and release year of various songs. There is no delimiting characters, instead the module reads them via token substrings corresponding to specific positions in the record string. 

The challenge was to write several different functions that would parse the file stream data correctly, assigning it to a list of structs contained within a class. STL algorithms are used to iterate through the list and present the information to the user, sorting it by various fields, modifying the data, searching for specific artists in the collection and retrieving their songs, and displaying the total track time of the output. The condition was to avoid reusing any of the algorithms in more than one function.
