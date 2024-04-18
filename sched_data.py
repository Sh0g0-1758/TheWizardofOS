from load_sched import load
import pickle
import pandas as pd
val = load("./Benchmarks/CPU/matmul.exe")
df  = pd.DataFrame(val)
print("actual cpu time : ", df["CPU times"][0])
df.drop("CPU times", axis=1, inplace=True)
model2 = pickle.load(open('model.pkl', 'rb'))
print("pred : ",model2.predict(df)[0])
