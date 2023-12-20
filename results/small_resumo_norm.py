import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

df = pd.read_csv('./small_result_TP_0_2_5000.csv',sep=';')

df['token_name'] = df['token_name'].map(lambda x: x.upper())
df['mapFilename'] = df['mapFilename'].map(lambda x: Path(x).name.upper())
df['taskFilename'] = df['taskFilename'].map(lambda x: Path(x).name.upper())

df2 = df[["mapFilename","taskFilename","token_name","current_step","energy expenditure",'time(s)', 'finished_tasks']]




df2["mapFilename"]= df2["mapFilename"].str.split("-", n = 2, expand = False).str[1]
df2["taskFilename"]= df2["taskFilename"].str.split("-", n = 1, expand = False).str[1].str.strip(".TASK")

df2 = df2.rename(
    columns={
        'mapFilename': 'Agentes', 
        'taskFilename': 'Frenq.', 
        'current_step': 'Makespan', 
        'energy expenditure': 'Energia',
        'token_name': 'Token',
        'time(s)':'time',
        'finished_tasks':'tasks'
        })


s = df2.groupby(["Frenq.", "Agentes"])['Makespan'].transform('max')
e = df2.groupby(["Frenq.", "Agentes"])['Energia'].transform('max')

df2["eq"] = (df2["Makespan"] / s +  df2["Energia"] / e) / 2

print(df2)


df4 = df2.groupby(["Frenq.","Agentes","Token"]).agg({'eq':np.median, 'time': np.median, 'tasks':np.median}).round(9).reset_index()

df5 = df4.pivot_table(["eq","time", 'tasks'], ["Frenq.", "Agentes"], 'Token')

print(df5)

df5.to_csv('./small_resumo_norm_TP_0_2_5000.csv',';')