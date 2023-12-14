import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

df = pd.read_csv('./small_result.csv',sep=';')

df['token_name'] = df['token_name'].map(lambda x: x.upper())
df['mapFilename'] = df['mapFilename'].map(lambda x: Path(x).name.upper())
df['taskFilename'] = df['taskFilename'].map(lambda x: Path(x).name.upper())

df2 = df[["mapFilename","taskFilename","token_name","current_step","energy expenditure",'time(s)', 'finished_tasks']]

df2["mapFilename"]= df2["mapFilename"].str.split("-", n = 2, expand = False).str[1]
df2["taskFilename"]= df2["taskFilename"].str.split("-", n = 1, expand = False).str[1].str.strip(".TASK")

df2 = df2.rename(
    columns={
        'mapFilename': 'Ag', 
        'taskFilename': 'F', 
        'current_step': 'Makespan', 
        'energy expenditure': 'Energia',
        'token_name': 'Token',
        'time(s)':'time',
        'finished_tasks':'tasks'
        })

df4 = df2.groupby(["F", "Ag", "Token"]).agg({'Makespan':np.median, 'Energia':np.median, 'time': np.median, 'tasks':np.median}).round(9).reset_index()

df5 = df4.pivot_table(["Makespan","Energia", "time", 'tasks'], ["F", "Ag"], 'Token')

print(df5)

df5.to_csv('./small_resumo_totais.csv',';')