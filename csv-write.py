
import csv

# Data to write to CSV
data = [
    ["John", 25, "Developer"],
    ["Emma", 30, "Designer"],
    ["Alice", 35, "Manager"]
]

# Specify the file path
file_path = "data1.csv"

# Writing data to CSV file
with open(file_path, mode='w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    
    # Write header if needed
    writer.writerow(["Name", "Age", "Occupation"])
    
    # Write data
    for row in data:
        writer.writerow(row)

print("Data has been written to", file_path)




