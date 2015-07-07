import pandas as pd
import matplotlib.pyplot as plt

pm = pd.read_csv('DATAFILE.TXT',names=['occupancy','ratio','con','Day','Time'], index_col='Time')

pm[['con','ratio','occupancy']].plot()
plt.show()