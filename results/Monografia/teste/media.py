import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path


df = pd.read_csv('./Monografia/teste/test_result.csv',sep=';')

df['token_name'] = df['token_name'].map(lambda x: x.upper())
df['mapFilename'] = df['mapFilename'].map(lambda x: Path(x).name.upper())
df['taskFilename'] = df['taskFilename'].map(lambda x: Path(x).name.upper())

df2 = df[["mapFilename","taskFilename","token_name","current_step","energy expenditure"]]

df2["energy expenditure"] = df2["energy expenditure"] / df2["energy expenditure"].max() 

df4 = df2.groupby(["mapFilename","taskFilename","token_name"]).agg({'current_step':np.median, 'energy expenditure':np.median}).round(3).reset_index()

df5 = df4.pivot_table(["current_step","energy expenditure"], ["mapFilename","taskFilename"], 'token_name')

print(df5)

df5.to_csv('./Monografia/teste/test_resumo.csv',';')