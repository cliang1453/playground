## Data and Visual Analytics - Homework 4
## Georgia Institute of Technology
## Applying ML algorithms to detect seizure

import numpy as np
import pandas as pd
import time

from sklearn.model_selection import cross_val_score, GridSearchCV, cross_validate, train_test_split
from sklearn.metrics import accuracy_score, classification_report
from sklearn.svm import SVC
from sklearn.linear_model import LinearRegression
from sklearn.neural_network import MLPClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import StandardScaler, normalize

######################################### Reading and Splitting the Data ###############################################
# XXX
# TODO: Read in all the data. Replace the 'xxx' with the path to the data set.
# XXX
data = pd.read_csv('seizure_dataset.csv')

# Separate out the x_data and y_data.
x_data = data.loc[:, data.columns != "y"]
y_data = data.loc[:, "y"]

# The random state to use while splitting the data.
random_state = 100

# XXX
# TODO: Split 70% of the data into training and 30% into test sets. Call them x_train, x_test, y_train and y_test.
# Use the train_test_split method in sklearn with the paramater 'shuffle' set to true and the 'random_state' set to 100.
# XXX
x_train, x_test, y_train, y_test = train_test_split(np.array(x_data), np.array(y_data), test_size=0.7, shuffle=True, random_state=random_state)

# ############################################### Linear Regression ###################################################
# XXX
# TODO: Create a LinearRegression classifier and train it.
# XXX
reg = LinearRegression().fit(x_train, y_train)

# XXX
# TODO: Test its accuracy (on the training set) using the accuracy_score method.
# TODO: Test its accuracy (on the testing set) using the accuracy_score method.
# Note: Use y_predict.round() to get 1 or 0 as the output.
# XXX
acc_train = accuracy_score(y_train, reg.predict(x_train).round())
acc_test = accuracy_score(y_test, reg.predict(x_test).round())


# ############################################### Multi Layer Perceptron #################################################
# XXX
# TODO: Create an MLPClassifier and train it.
# XXX
mlp = MLPClassifier().fit(x_train, y_train)

# XXX
# TODO: Test its accuracy on the training set using the accuracy_score method.
# TODO: Test its accuracy on the test set using the accuracy_score method.
# XXX
acc_train = accuracy_score(y_train, mlp.predict(x_train))
acc_test = accuracy_score(y_test, mlp.predict(x_test))

# ############################################### Random Forest Classifier ##############################################
# XXX
# TODO: Create a RandomForestClassifier and train it.
# XXX
rfc = RandomForestClassifier(n_estimators=100, max_depth=120).fit(x_train, y_train)

# XXX
# TODO: Test its accuracy on the training set using the accuracy_score method.
# TODO: Test its accuracy on the test set using the accuracy_score method.
# XXX
acc_train = accuracy_score(y_train, rfc.predict(x_train))
acc_test = accuracy_score(y_test, rfc.predict(x_test))

# XXX
# TODO: Tune the hyper-parameters 'n_estimators' and 'max_depth'.
#       Print the best params, using .best_params_, and print the best score, using .best_score_.
# XXX
rf_random = RandomForestClassifier()
param_grid = {
    'max_depth': [70, 80, 90, 100, 110, 120, 130, 150, 200, 300],
    'n_estimators': [50, 80, 90, 100, 200, 300]}

grid_search = GridSearchCV(estimator=rf_random, param_grid=param_grid, 
                          cv = 10, n_jobs = -1, verbose = 2).fit(x_train, y_train)
acc_tuned = accuracy_score(y_test, grid_search.predict(x_test))
print(grid_search.best_params_)
print(grid_search.best_score_)



# ############################################ Support Vector Machine ###################################################
# XXX
# TODO: Pre-process the data to standardize or normalize it, otherwise the grid search will take much longer
# TODO: Create a SVC classifier and train it.
# XXX

scaler = StandardScaler().fit(x_train.astype(np.float64))
x_train_ = scaler.transform(x_train)
x_test_ = scaler.transform(x_test)
x_train_ = normalize(x_train_)
x_test_ = normalize(x_test_)

svc = SVC().fit(x_train_, y_train)

# XXX
# TODO: Test its accuracy on the training set using the accuracy_score method.
# TODO: Test its accuracy on the test set using the accuracy_score method.
# XXX

acc_train = accuracy_score(y_train, svc.predict(x_train_))
acc_test = accuracy_score(y_test, svc.predict(x_test_))


# XXX
# TODO: Tune the hyper-parameters 'C' and 'kernel' (use rbf and linear).
#       Print the best params, using .best_params_, and print the best score, using .best_score_.
# XXX

random_svc = SVC(gamma='auto')
param_grid = {
    'C': [1e-3, 1e-2, 1e-1, 1, 1e2, 1e3],
    'kernel': ['rbf', 'linear']}
grid_search = GridSearchCV(estimator=random_svc, param_grid=param_grid, 
                          cv = 10, n_jobs = -1, verbose = 2).fit(x_train_, y_train)

acc_tuned = accuracy_score(y_test, grid_search.predict(x_test_))
print(grid_search.best_params_)
print(grid_search.best_score_)