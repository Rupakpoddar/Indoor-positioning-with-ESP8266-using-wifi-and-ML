from sklearn.svm import SVC
from sklearn.utils import shuffle
from sklearn_porter import Porter

X = []
y = []

file = open("Data.tsv", 'r')

for line in file:
    Include = True
    data = line.rstrip("\n").split("\t")
    for i in range(len(data)):
        data[i] = float(data[i])
        if data[i] == 0:
            Include = False
    if Include:
        X.append(data[:-1])
        y.append(data[-1])

# print(X)
# print(y)

X, y = shuffle(X, y, random_state=0)

clf = SVC(kernel='linear', gamma=0.001, tol=1e-4)
clf.fit(X, y)

porter = Porter(clf, language='C')
output = porter.export(embed_data=True)
print(output)
