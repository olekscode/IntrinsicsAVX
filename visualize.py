import pandas as pd 
import matplotlib.pyplot as plt 

df = pd.read_csv('results.csv', header=None)

plt.plot(df[0], df[1])
plt.plot(df[0], df[2])
plt.savefig('time.png')
plt.show()