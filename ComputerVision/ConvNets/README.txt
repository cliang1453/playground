# EE569 Homework Assignment #2
# Date: Apr 23 2017
# Name: Chen Liang
# ID: 7314-6343-96
# email: lian455@usc.edu
#
# Compiled on linux, Ubuntu (64 bit) 14.04, Compiled using g++
# Define compiler and linker

=======================================================================================
Problem I, Part I

Detailed instructions on how to get started available at: http://tensorflow.org/tutorials/deep_cnn/. The code source was acknowledged by https://github.com/tensorflow/models/tree/master/tutorials/image/cifar10.

Train model using: python cifar10_train.py

Evaluate training accuracy using: python cifar10_eval.py
Using parameter setting below
tf.app.flags.DEFINE_string('eval_dir', './tmp/cifar10_eval_train',
                           """Directory where to write event logs.""")
tf.app.flags.DEFINE_string('eval_data', 'train_eval',
                           """Either 'test' or 'train_eval'.""")
tf.app.flags.DEFINE_string('checkpoint_dir', './tmp/cifar10_train',
                           """Directory where to read model checkpoints.""")
tf.app.flags.DEFINE_integer('eval_interval_secs', 0.01,
                            """How often to run the eval.""")
tf.app.flags.DEFINE_integer('num_examples', 10000,
                            """Number of examples to run.""")
tf.app.flags.DEFINE_boolean('run_once', True,
                         """Whether to run eval only once.""")

Evaluate testing accuracy using: python cifar10_eval.py
Using parameter setting below
tf.app.flags.DEFINE_string('eval_dir', './tmp/cifar10_eval_test',
                           """Directory where to write event logs.""")
tf.app.flags.DEFINE_string('eval_data', 'test',
                           """Either 'test' or 'train_eval'.""")
tf.app.flags.DEFINE_string('checkpoint_dir', './tmp/cifar10_train',
                           """Directory where to read model checkpoints.""")
tf.app.flags.DEFINE_integer('eval_interval_secs', 0.01,
                            """How often to run the eval.""")
tf.app.flags.DEFINE_integer('num_examples', 10000,
                            """Number of examples to run.""")
tf.app.flags.DEFINE_boolean('run_once', True,
                         """Whether to run eval only once.""")

If you directly train the model, the parameters setting is set to be
MOVING_AVERAGE_DECAY = 0.9999     # The decay to use for the moving average.
NUM_EPOCHS_PER_DECAY = 350.0      # Epochs after which learning rate decays.
LEARNING_RATE_DECAY_FACTOR = 0.1  # Learning rate decay factor.
INITIAL_LEARNING_RATE = 0.1       # Initial learning rate.
tf.contrib.layers.xavier_initializer(uniform=False, seed=None, dtype=tf.float32))
batch_size = 128
max_steps = 100000 

=======================================================================================
Problem I, Part II

Code reference was from 

Train model: python cifar10_train.py

Evaluate training accuracy using: python cifar10_eval.py
Using parameter setting below
tf.app.flags.DEFINE_string('eval_dir', './tmp/cifar10_eval_train',
                           """Directory where to write event logs.""")
tf.app.flags.DEFINE_string('eval_data', 'train_eval',
                           """Either 'test' or 'train_eval'.""")
tf.app.flags.DEFINE_string('checkpoint_dir', './tmp/cifar10_train',
                           """Directory where to read model checkpoints.""")
tf.app.flags.DEFINE_integer('eval_interval_secs', 0.01,
                            """How often to run the eval.""")
tf.app.flags.DEFINE_integer('num_examples', 10000,
                            """Number of examples to run.""")
tf.app.flags.DEFINE_boolean('run_once', True,
                         """Whether to run eval only once.""")

Evaluate testing accuracy using: python cifar10_eval.py
Using parameter setting below
tf.app.flags.DEFINE_string('eval_dir', './tmp/cifar10_eval_test',
                           """Directory where to write event logs.""")
tf.app.flags.DEFINE_string('eval_data', 'test',
                           """Either 'test' or 'train_eval'.""")
tf.app.flags.DEFINE_string('checkpoint_dir', './tmp/cifar10_train',
                           """Directory where to read model checkpoints.""")
tf.app.flags.DEFINE_integer('eval_interval_secs', 0.01,
                            """How often to run the eval.""")
tf.app.flags.DEFINE_integer('num_examples', 10000,
                            """Number of examples to run.""")
tf.app.flags.DEFINE_boolean('run_once', True,
                         """Whether to run eval only once.""")

If you directly train the model, the parameters setting is set to be
MOVING_AVERAGE_DECAY = 0.9999     # The decay to use for the moving average.
NUM_EPOCHS_PER_DECAY = 350.0      # Epochs after which learning rate decays.
LEARNING_RATE_DECAY_FACTOR = 0.1  # Learning rate decay factor.
INITIAL_LEARNING_RATE = 0.1       # Initial learning rate.
KMEAN_ITERATION = 200		      # Total kmean update iteration
NUM_PATCH_EACH_IMAGE = 20         # Number of random patches cropped from each image
NUM_IMAGE_EACH_BATCH = 50         # Number of random images choosed from each batch 
batch_size = 128
max_steps = 100000 

=======================================================================================
Problem II

This code was used for experiments with Wide Residual Networks (BMVC 2016) http://arxiv.org/abs/1605.07146 by Sergey Zagoruyko and Nikos Komodakis.
This code reference was from https://github.com/szagoruyko/wide-residual-networks. 

Download the cifar10 whitened dataset at https://yadi.sk/d/em4b0FMgrnqxy to datasets folder

Train the model with depth=40 and widen_factor=4 with: model=wide-resnet widen_factor=4 depth=40 ./scripts/train_cifar.sh

Log is saved to logs/wide-resnet_$RANDOM$RANDOM folder with json entries for each epoch and can be visualized with itorch/ipython later.

Then using ipython notebook (jupiter notebook) to visualize the data with sweeper.py and the data saved in log file. 

Implementation details
The code evolved from https://github.com/szagoruyko/cifar.torch. To reduce memory usage we use @fmassa's optimize-net, which automatically shares output and gradient tensors between modules. This keeps memory usage below 4 Gb even for our best networks. Also, it can generate network graph plots as the one for WRN-16-2 in the end of this page.

Acknowledgements
We thank startup company VisionLabs and Eugenio Culurciello for giving us access to their clusters, without them ImageNet experiments wouldn't be possible. We also thank Adam Lerer and Sam Gross for helpful discussions. Work supported by EC project FP7-ICT-611145 ROBOSPECT.


