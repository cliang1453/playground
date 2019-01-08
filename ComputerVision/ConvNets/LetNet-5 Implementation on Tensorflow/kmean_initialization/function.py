import tensorflow as tf
import numpy as np

NUM_PATCH_EACH_IMAGE = 20
NUM_IMAGE_EACH_BATCH = 50

def pre_processing(shape, i_size, images):
  final_samples = tf.placeholder(tf.float32)

  for i in xrange(NUM_IMAGE_EACH_BATCH):
    print(i)
    begin = [i, 0, 0, 0]
    size = [1, i_size[1], i_size[2], i_size[3]]
    size[0] = 1
    extracted_image = tf.slice(images, begin, size)
    extracted_image = tf.reshape(extracted_image, [i_size[1], i_size[2], i_size[3]])
    patch_3d = tf.random_crop(extracted_image, [shape[0], shape[1], shape[2]], seed=None, name=None)
    mean, var = tf.nn.moments(patch_3d, axes = [0,1], shift=None, name=None, keep_dims=False)
    patch_3d = tf.nn.l2_normalize(patch_3d, [0,1], epsilon=1e-12, name=None)
    patch_stack = tf.expand_dims(patch_3d, 0)
    var_stack = tf.expand_dims(tf.norm(var, ord='euclidean', axis = None, keep_dims=False, name=None), 0)

    for j in xrange(NUM_PATCH_EACH_IMAGE-1):
      patch_3d = tf.random_crop(extracted_image, [shape[0], shape[1], shape[2]], seed=None, name=None)
      mean, var = tf.nn.moments(patch_3d, axes = [0,1], shift=None, name=None, keep_dims=False)
      # patch_3d = tf.nn.batch_normalization(patch_3d, mean, 2*var, 0, 1, variance_epsilon=1e-12, name=None)
      patch_3d = tf.nn.l2_normalize(patch_3d, [0,1], epsilon=1e-12, name=None)
      patch_4d = tf.expand_dims(patch_3d, 0)
      var_2d = tf.expand_dims(tf.norm(var, ord='euclidean', axis = None, keep_dims=False, name=None), 0)
      patch_stack = tf.concat([patch_stack, patch_4d], 0)
      var_stack = tf.concat([var_stack, var_2d], 0)

    values, indices = tf.nn.top_k(var_stack, k = 5, sorted=True, name=None)
    begin = [indices[0], 0, 0, 0]
    size = [1, shape[0], shape[1], shape[2]]
    size[0] = 1
    selected_patch = tf.slice(patch_stack, begin, size)
    samples = tf.expand_dims(tf.reshape(selected_patch, [-1]), 0)

    for k in xrange(4):
      begin = [indices[k+1], 0, 0, 0]
      size = [1, shape[0], shape[1], shape[2]]
      size[0] = 1
      selected_patch = tf.slice(patch_stack, begin, size)
      selected_patch_1d = tf.expand_dims(tf.reshape(selected_patch, [-1]), 0)
      samples = tf.concat([samples, selected_patch_1d], 0)

    pred = tf.equal(tf.constant(i), tf.constant(0))
    sess = tf.Session()
    if sess.run(pred):
      final_samples = samples
    else:
      final_samples = tf.concat([final_samples, samples], 0)
    sess.close()

  return final_samples

def choose_random_centroids(samples, n_clusters):
    # Step 0: Initialisation: Select `n_clusters` number of random point

    n_samples = tf.shape(samples)[0]
    random_indices = tf.random_shuffle(tf.range(0, n_samples))
    begin = [0,]
    size = [n_clusters,]
    size[0] = n_clusters
    centroid_indices = tf.slice(random_indices, begin, size)
    initial_centroids = tf.gather(samples, centroid_indices) #lihai haiyouzhezhonga 
    return initial_centroids

def assign_to_nearest(samples, centroids):
    # Finds the nearest centroid for each sample
    # START from http://esciencegroup.com/2016/01/05/an-encounter-with-googles-tensorflow/
    expanded_vectors = tf.expand_dims(samples, 0)
    expanded_centroids = tf.expand_dims(centroids, 1)
    distances = tf.reduce_sum(tf.square(tf.subtract(expanded_vectors, expanded_centroids)), 2)
    mins = tf.argmin(distances, 0)
    # END from http://esciencegroup.com/2016/01/05/an-encounter-with-googles-tensorflow/
    nearest_indices = mins
    return nearest_indices

def update_centroids(samples, nearest_indices, n_clusters):
    # Updates the centroid to be the mean of all samples associated with it.
    nearest_indices = tf.to_int32(nearest_indices)
    partitions = tf.dynamic_partition(samples, nearest_indices, n_clusters)
    new_centroids = tf.concat([tf.expand_dims(tf.reduce_mean(partition, 0), 0) for partition in partitions], 0)
    return new_centroids