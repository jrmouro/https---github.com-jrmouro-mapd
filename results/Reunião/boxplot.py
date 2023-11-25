import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

df = pd.read_csv('./small_closed_result_ep_0_2_10.csv',sep=';')

df['token_name'] = df['token_name'].map(lambda x: x.upper())
df['mapFilename'] = df['mapFilename'].map(lambda x: Path(x).name.upper())

mapsSet = set(df['mapFilename'])
tokensSet = set(df['token_name'])

tfn_set = set(df['taskFilename'])

tfn_es = {}
tfn_ms = {}

for tsn in tfn_set:
    df_filtered = df.query('taskFilename=="'+ tsn +'"')
    tfn_es[tsn] = df_filtered['energy expenditure'].max() 
    tfn_ms[tsn] = df_filtered['current_step'].max()

for index, row in df.iterrows():
    key = row['taskFilename']
    row['energy expenditure'] = row['energy expenditure'] / tfn_es[key]
    row['current_step'] = row['current_step'] / tfn_ms[key]

for m in mapsSet:

    df_filtered = df.query('mapFilename=="'+ m +'"')

    lim_y = {}
    lim_y['makespan'] = [df_filtered['current_step'].min(), df_filtered['current_step'].max() ]
    lim_y['energy'] = [df_filtered['energy expenditure'].min(), df_filtered['energy expenditure'].max()]

    data = {}
    data['makespan'] = {}
    data['energy'] = {}

    for t in tokensSet:
        df_map_token = df_filtered.query('token_name=="'+ t +'"')
        data['makespan'][t] = df_map_token['current_step']
        data['energy'][t] = df_map_token['energy expenditure']

    fig, axes = plt.subplots(2, sharey=False)
    fig.tight_layout(pad=3.0)
    # fig.subplots_adjust(wspace=0)
    fig.suptitle(m, fontsize=16)
    

    for ax, name in zip(axes, ['makespan', 'energy']):
        ax.set_title(name)
        ax.boxplot([data[name][item] for item in tokensSet])
        ax.set(xticklabels=tokensSet)
        ax.margins(0.001)       
    

    plt.show()
    

    

# import matplotlib.pyplot as plt
# import numpy as np
# import random

# data = {}
# data['dataset1'] = {}
# data['dataset2'] = {}
# data['dataset3'] = {}

# n = 500
# for k,v in data.iteritems():
#     upper = random.randint(0, 1000)
#     v['A'] = np.random.uniform(0, upper, size=n)
#     v['B'] = np.random.uniform(0, upper, size=n)
#     v['C'] = np.random.uniform(0, upper, size=n)

# fig, axes = plt.subplots(ncols=3, sharey=True)
# fig.subplots_adjust(wspace=0)

# for ax, name in zip(axes, ['dataset1', 'dataset2', 'dataset3']):
#     ax.boxplot([data[name][item] for item in ['A', 'B', 'C']])
#     ax.set(xticklabels=['A', 'B', 'C'], xlabel=name)
#     ax.margins(0.05) # Optional

# plt.show()
