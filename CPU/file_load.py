import numpy as np

def file_io_load(filename="large_file.txt"):
    # Generate a large file
    size = np.random.randint(10,10000)
    random_text = "Hello, world! " * size
    with open("text_file.txt","w") as file:
        file.write(random_text)
    
    # Read the file
    with open("text_file.txt","r") as file:
        text = file.read()

    #write one line to the file
    with open("text_file.txt","a") as file:
        file.write("Hello, world!")
    
    #read one line form the file
    with open("text_file.txt","r") as file:
        lines = file.readlines()

    # Delete the file
    import os
    os.remove("text_file.txt")
    return text