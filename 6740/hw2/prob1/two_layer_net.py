
# coding: utf-8

# # Implementing a Neural Network
# In this exercise we will develop a neural network with fully-connected layers to perform 
#classification, and test it out on the CIFAR-10 dataset.

# A bit of setup

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['figure.figsize'] = (10.0, 8.0) # set default size of plots
plt.rcParams['image.interpolation'] = 'nearest'
plt.rcParams['image.cmap'] = 'gray'


def rel_error(x, y):
  """ returns relative error """
  return np.max(np.abs(x - y) / (np.maximum(1e-8, np.abs(x) + np.abs(y))))

###############################################################################
# The neural network parameters will be stored in a dictionary (`model` below), 
#where the keys are the parameter names and the values are numpy arrays. Below, 
#we initialize toy data and a toy model that we will use to verify your implementations.


# Create some toy data to check your implementations
input_size = 4
hidden_size = 10
num_classes = 3
num_inputs = 5

def init_toy_model():
  model = {}
  model['W1'] = np.linspace(-0.2, 0.6, num=input_size*hidden_size).reshape(input_size, hidden_size)
  model['b1'] = np.linspace(-0.3, 0.7, num=hidden_size)
  model['W2'] = np.linspace(-0.4, 0.1, num=hidden_size*num_classes).reshape(hidden_size, num_classes)
  model['b2'] = np.linspace(-0.5, 0.9, num=num_classes)
  return model

def init_toy_data():
  X = np.linspace(-0.2, 0.5, num=num_inputs*input_size).reshape(num_inputs, input_size)
  y = np.array([0, 1, 2, 2, 1])
  return X, y

model = init_toy_model()
X, y = init_toy_data()

###############################################################################
# # Forward pass: compute scores
# Open the file `cs231n/classifiers/neural_net.py` and look at the function 
#`two_layer_net`. This function takes the data and weights and 
#computes the class scores, the loss, and the gradients on the parameters. 
# 
# Implement the first part of the forward pass which uses the weights and biases 
# to compute the scores for all inputs.

#get_ipython().magic('reload_ext autoreload')
from cs231n.classifiers.neural_net import two_layer_net

scores = two_layer_net(X, model)
print(scores)
correct_scores = [[-0.5328368, 0.20031504, 0.93346689],
 [-0.59412164, 0.15498488, 0.9040914 ],
 [-0.67658362, 0.08978957, 0.85616275],
 [-0.77092643, 0.01339997, 0.79772637],
 [-0.89110401, -0.08754544, 0.71601312]]

# the difference should be very small. We get 3e-8
print('Difference between your scores and correct scores:')
print(np.sum(np.abs(scores - correct_scores)))

###############################################################################
# # Forward pass: compute loss
# In the same function, implement the second part that computes the data and regularizaion loss.

reg = 0.1
loss, _ = two_layer_net(X, model, y, reg)
correct_loss = 1.38191946092

# should be very small, we get 5e-12
print('Difference between your loss and correct loss:')
print(np.sum(np.abs(loss - correct_loss)))

###############################################################################
# # Backward pass
# Implement the rest of the function. This will compute the gradient of the loss with respect to the variables `W1`, `b1`, `W2`, and `b2`. Now that you (hopefully!) have a correctly implemented forward pass, you can debug your backward pass using a numeric gradient check:

from cs231n.gradient_check import eval_numerical_gradient

# Use numeric gradient checking to check your implementation of the backward pass.
# If your implementation is correct, the difference between the numeric and
# analytic gradients should be less than 1e-8 for each of W1, W2, b1, and b2.

loss, grads = two_layer_net(X, model, y, reg)

# these should all be less than 1e-8 or so
for param_name in grads:
  param_grad_num = eval_numerical_gradient(lambda W: two_layer_net(X, model, y, reg)[0], model[param_name], verbose=False)
  print('%s max relative error: %e' % (param_name, rel_error(param_grad_num, grads[param_name])))
  
###############################################################################

# # Train the network
# To train the network we will use SGD with Momentum. 
#Open the file `classifier_trainer.py` and familiarize yourself with the `ClassifierTrainer` 
#class. It performs optimization given an arbitrary cost function data, and model. 
#By default it uses vanilla SGD, which you need to implement. 
#First, run the optimization below using Vanilla SGD:

from cs231n.classifier_trainer import ClassifierTrainer

model = init_toy_model()
trainer = ClassifierTrainer()
# call the trainer to optimize the loss
# Notice that we're using sample_batches=False, so we're performing Gradient Descent (no sampled batches of data)
best_model, loss_history, _, _ = trainer.train(X, y, X, y,
                                             model, two_layer_net,
                                             reg=0.001,
                                             learning_rate=1e-1, momentum=0.0, learning_rate_decay=1,
                                             update='sgd', sample_batches=False,
                                             num_epochs=100,
                                             verbose=False)
print('Final loss with vanilla SGD: %f' % (loss_history[-1], ))

###############################################################################
# Now read the momentum update rule inside 
#the `train` function, and run the same optimization as above but with the momentum 
#update. You should see a much better result in the final obtained loss:


model = init_toy_model()
trainer = ClassifierTrainer()
# call the trainer to optimize the loss
# Notice that we're using sample_batches=False, so we're performing Gradient 
#Descent (no sampled batches of data)
best_model, loss_history, _, _ = trainer.train(X, y, X, y,
                                             model, two_layer_net,
                                             reg=0.001,
                                             learning_rate=1e-1, momentum=0.9, learning_rate_decay=1,
                                             update='momentum', sample_batches=False,
                                             num_epochs=100,
                                             verbose=False)
correct_loss = 0.494394
print('Final loss with momentum SGD: %f. We get: %f' % (loss_history[-1], correct_loss))

###############################################################################
# The **RMSProp** update step is given as follows:
# 
# ```
# cache = decay_rate * cache + (1 - decay_rate) * dx**2
# x += - learning_rate * dx / np.sqrt(cache + 1e-8)
# ```
# 
# Here, `decay_rate` is a hyperparameter and typical values are [0.9, 0.99, 0.999].
# 
# Read the update rule inside the `train` function and rerun the optimization:


model = init_toy_model()
trainer = ClassifierTrainer()
# call the trainer to optimize the loss
# Notice that we're using sample_batches=False, so we're performing Gradient Descent 
#(no sampled batches of data)
best_model, loss_history, _, _ = trainer.train(X, y, X, y,
                                             model, two_layer_net,
                                             reg=0.001,
                                             learning_rate=1e-1, momentum=0.9, learning_rate_decay=1,
                                             update='rmsprop', sample_batches=False,
                                             num_epochs=100,
                                             verbose=False)
correct_loss = 0.439368
print('Final loss with RMSProp: %f. We get: %f' % (loss_history[-1], correct_loss))

###############################################################################
# # Load the data
# Now that you have implemented a two-layer network that passes gradient checks, 
#it's time to load up our favorite CIFAR-10 data so we can use it to train a classifier.


from cs231n.data_utils import load_CIFAR10

def get_CIFAR10_data(num_training=49000, num_validation=1000, num_test=1000):
    """
    Load the CIFAR-10 dataset from disk and perform preprocessing to prepare
    it for the two-layer neural net classifier.
    """
    # Load the raw CIFAR-10 data
    cifar10_dir = 'cs231n/datasets/cifar-10-batches-py'
    X_train, y_train, X_test, y_test = load_CIFAR10(cifar10_dir)
        
    # Subsample the data
    mask = range(num_training, num_training + num_validation)
    X_val = X_train[mask]
    y_val = y_train[mask]
    mask = range(num_training)
    X_train = X_train[mask]
    y_train = y_train[mask]
    mask = range(num_test)
    X_test = X_test[mask]
    y_test = y_test[mask]

    # Normalize the data: subtract the mean image
    mean_image = np.mean(X_train, axis=0)
    X_train -= mean_image
    X_val -= mean_image
    X_test -= mean_image

    # Reshape data to rows
    X_train = X_train.reshape(num_training, -1)
    X_val = X_val.reshape(num_validation, -1)
    X_test = X_test.reshape(num_test, -1)

    return X_train, y_train, X_val, y_val, X_test, y_test


# Invoke the above function to get our data.
X_train, y_train, X_val, y_val, X_test, y_test = get_CIFAR10_data()
print('Train data shape: ', X_train.shape)
print('Train labels shape: ', y_train.shape)
print('Validation data shape: ', X_val.shape)
print('Validation labels shape: ', y_val.shape)
print('Test data shape: ', X_test.shape)
print('Test labels shape: ', y_test.shape)

###############################################################################
# # Train a network
# To train our network we will use SGD with momentum. In addition, we will 
#adjust the learning rate with an exponential learning rate schedule as optimization 
#proceeds; after each epoch, we will reduce the learning rate by multiplying it by a decay rate.


from cs231n.classifiers.neural_net import init_two_layer_model

model = init_two_layer_model(32*32*3, 50, 10) # input size, hidden size, number of classes
trainer = ClassifierTrainer()
best_model, loss_history, train_acc, val_acc = trainer.train(X_train, y_train, X_val, y_val,
                                             model, two_layer_net,
                                             num_epochs=5, reg=1.0,
                                             momentum=0.9, learning_rate_decay = 0.95,
                                             learning_rate=1e-5, verbose=True)


###############################################################################
# # Debug the training
# With the default parameters we provided above, you should get a validation 
#accuracy of about 0.37 on the validation set. This isn't very good.
# 
# One strategy for getting insight into what's wrong is to plot the loss function 
#and the accuracies on the training and validation sets during optimization.
# 
# Another strategy is to visualize the weights that were learned in the first 
#layer of the network. In most neural networks trained on visual data, the first 
#layer weights typically show some visible structure when visualized.


# Plot the loss function and train / validation accuracies
plt.figure(1)
plt.subplot(2, 1, 1)
plt.plot(loss_history)
plt.title('Loss history')
plt.xlabel('Iteration')
plt.ylabel('Loss')
plt.subplot(2, 1, 2)
plt.plot(train_acc)
plt.plot(val_acc)
plt.legend(['Training accuracy', 'Validation accuracy'], loc='lower right')
plt.xlabel('Epoch')
plt.ylabel('Clasification accuracy')
plt.show()


###############################################################################

from cs231n.vis_utils import visualize_grid

# Visualize the weights of the network

def show_net_weights(model):
    plt.figure(2)
    plt.imshow(visualize_grid(model['W1'].T.reshape(-1, 32, 32, 3), padding=3).astype('uint8'))
    plt.gca().axis('off')
    plt.show()

show_net_weights(model)

###############################################################################
# # Tune your hyperparameters
# 
# **What's wrong?**. Looking at the visualizations above, we see that the loss 
#is decreasing more or less linearly, which seems to suggest that the learning rate may be too low. Moreover, there is no gap between the training and validation accuracy, suggesting that the model we used has low capacity, and that we should increase its size. On the other hand, with a very large model we would expect to see more overfitting, which would manifest itself as a very large gap between the training and validation accuracy.
# 
# **Tuning**. Tuning the hyperparameters and developing intuition for how they 
#affect the final performance is a large part of using Neural Networks, so we 
#want you to get a lot of practice. Below, you should experiment with different 
#values of the various hyperparameters, including hidden layer size, learning 
#rate, numer of training epochs, and regularization strength. You might also 
#consider tuning the momentum and learning rate decay parameters, but you should 
#be able to get good performance using the default values.
# 
# **Approximate results**. You should be aim to achieve a classification accuracy 
#of greater than 50% on the validation set. Our best network gets over 56% on the validation set.
# 
# **Experiment**: You goal in this exercise is to get as good of a result on 
#CIFAR-10 as you can, with a fully-connected Neural Network. For every 1% above 56% 
#on the Test set we will award you with one extra bonus point. Feel free implement 
#your own techniques (e.g. PCA to reduce dimensionality, or adding dropout, or 
#adding features to the solver, etc.).



best_model = None # store the best model into this 

#################################################################################
# TODO: Tune hyperparameters using the validation set. Store your best trained  #
# model in best_model.                                                          #
#                                                                               #
# To help debug your network, it may help to use visualizations similar to the  #
# ones we used above; these visualizations will have significant qualitative    #
# differences from the ones we saw above for the poorly tuned network.          #
#                                                                               #
# Tweaking hyperparameters by hand can be fun, but you might find it useful to  #
# write code to sweep through possible combinations of hyperparameters          #
# automatically like we did on the previous assignment.                         #
#################################################################################
# input size, hidden size, number of classes
model = init_two_layer_model(32*32*3, 100, 10) # input size, hidden size, number of classes
trainer = ClassifierTrainer()
best_model, loss_history, train_acc, val_acc = trainer.train(X_train, y_train, X_val, y_val,
                                             model, two_layer_net,
                                             update='momentum',
                                             num_epochs=15, reg=0.02,
                                             momentum=0.9, learning_rate_decay = 0.95,
                                             learning_rate=1e-4, verbose=True)

#################################################################################
#                               END OF YOUR CODE                                #
#################################################################################

#Plot the loss function and train / validation accuracies
plt.figure(1)
plt.subplot(2, 1, 1)
plt.plot(loss_history)
plt.title('Loss history')
plt.xlabel('Iteration')
plt.ylabel('Loss')
plt.subplot(2, 1, 2)
plt.plot(train_acc)
plt.plot(val_acc)
plt.legend(['Training accuracy', 'Validation accuracy'], loc='lower right')
plt.xlabel('Epoch')
plt.ylabel('Clasification accuracy')
plt.show()
 
# visualize the weights
show_net_weights(best_model)

###############################################################################
# # Run on the test set
# When you are done experimenting, you should evaluate your final trained network on the test set.

scores_test = two_layer_net(X_test, best_model)
print('Test accuracy: ', np.mean(np.argmax(scores_test, axis=1) == y_test))

