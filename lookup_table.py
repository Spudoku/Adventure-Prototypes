entries_count = 144
entry_interval = 120
with open("lookup_table.txt","w") as file:
    file.write("{")
    for i in range(entries_count):
        file.write(str(entry_interval * i) + ", ")
    
    file.write("};")