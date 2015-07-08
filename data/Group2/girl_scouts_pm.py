import pandas as pd
import matplotlib.pyplot as plt

pm = pd.read_csv('DATAFILE.TXT',names=['occupancy','ratio','con','Day','Time'], index_col='Time')
rng = pd.date_range('7/8/2015 10:00:00', periods=len(pm), freq='30s')
pm = pm.set_index(rng)
pm[['con']].plot(legend=None)
plt.title('Group 2')
plt.xlabel('Time')
plt.ylabel('Particle Concentration')
plt.show()