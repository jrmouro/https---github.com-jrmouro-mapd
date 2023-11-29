import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

df = pd.read_csv('./Monografia/small/GA/small_ga_result.csv',sep=';')

df['token_name'] = df['token_name'].map(lambda x: x.upper())
df['mapFilename'] = df['mapFilename'].map(lambda x: Path(x).name.upper())
df['taskFilename'] = df['taskFilename'].map(lambda x: Path(x).name.upper())

df2 = df[["mapFilename","taskFilename","token_name","current_step","energy expenditure"]]




df2["mapFilename"]= df2["mapFilename"].str.split("-", n = 2, expand = False).str[1]
df2["taskFilename"]= df2["taskFilename"].str.split("-", n = 1, expand = False).str[1].str.strip(".TASK")

df2 = df2.rename(
    columns={
        'mapFilename': 'Nº Agentes', 
        'taskFilename': 'Frenquência', 
        'current_step': 'Makespan', 
        'energy expenditure': 'Energia',
        'token_name': 'Token'
        })


print(df2)

df2["Energia"] = df2["Energia"] / df2["Energia"].max() 

df4 = df2.groupby(["Nº Agentes","Frenquência","Token"]).agg({'Makespan':np.median, 'Energia':np.median}).round(3).reset_index()

df5 = df4.pivot_table(["Makespan","Energia"], ["Nº Agentes","Frenquência"], 'Token')

print(df5)

df5.to_csv('./Monografia/small/GA/small_tp_resumo.csv',';')