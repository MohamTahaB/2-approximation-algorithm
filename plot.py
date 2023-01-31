import matplotlib.pyplot as plt
import csv

file = open("output_0.csv")
csvReader = csv.reader(file)
x , y = [] , []
for row in csvReader :
    x.append(float(row[0]))
    y.append(float(row[1]))

plt.scatter(x, y, s=30, marker='+')
plt.title("Elapsed time (microseconds) for graph dymension")

plt.show()

